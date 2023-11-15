; Author: Tim Buetler
; 01.11.2023
;
; - Used registers:
; - rbx 	Input data
; - rax     number of bytes read from input
; - r11     current position in InBuf
; - r12     current position in OutBuf

SECTION .data                   ; Section containing initialised data
	digits:     db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

SECTION .bss                    ; Section containing uninitialized data
	InBufLen:	equ 3
	InBuf: 		resb InBufLen

	OutBufLen:	equ 4
	OutBuf:		resb OutBufLen

SECTION .text                   ; Section containing code

global _start                   ; Linker needs this to find the entry point!

_start:

read:
	; read bytes from the input
	mov rax, 0                  	; sys_read
	mov rdi, 0                  	; file descriptor
	mov rsi, InBuf              	; destination buffer
	mov rdx, InBufLen           	; maximum # of bytes to read
	syscall

	cmp rax, 0						; did we recieved any bytes
	je exit							; if not jump to exit

	; reset registers
	xor rdi, rdi					; input bytes processed
	xor r12, r12					; output bytes written

process:
	; we want to devide and conquer
	; Input 	00000000 00000000 00000000		3 times 8 bits
	; Output 	000000 000000 000000 000000		4 times 6 bits
	; dont forget the big endian

	; reset registers
	xor rbx, rbx
	xor rdx, rdx

	; load 3 bytes in the right order
	mov dh, byte [InBuf + rdi]
	shl rdx, 8
	mov dh, byte [InBuf + rdi + 1]
	mov dl, byte [InBuf + rdi + 2]

	; 1st 6er bit block
	mov rbx, rdx
	shr rbx, 18
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12

	; 2nd 6er bit block
	mov rbx, rdx
	shr rbx, 12
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

	; 3rd 6er bit block
	cmp rax, rdi
	je add_double_equal

	mov rbx, rdx
	shr rbx, 6
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

	; 4th 6er bit block
	cmp rax, rdi
	je add_equal

	mov rbx, rdx
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

	; is there more?
	cmp rdi, rax
	jne read

writeline:
	; write OutBuf to stdout
	mov rax, 1
	mov rdi, 1
	mov rsi, OutBuf
	mov rdx, r12
	syscall

exit:
	; properly terminate the program
	mov rax, 60                 ; sys_exit
	mov rdi, 0                ; return 0 for success
	syscall

	; add some '=' or '==' to fill up
add_double_equal:
	mov rcx, 0x3d
	mov [OutBuf + r12], rcx					; write one '=' to OutBuf
	inc r12

add_equal:
	mov rcx, 0x3d
	mov [OutBuf + r12], rcx					; write one '=' to OutBuf
	inc r12
	jmp writeline
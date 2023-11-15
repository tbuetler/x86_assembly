; Author: Tim Buetler
; 01.11.2023
;
; - Used registers:
; - rbx 	Input data
; - rax     number of bytes read from input
; - r11     current position in InBuf
; - r12     current position in OutBuf

section .data
	digits:     db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

section .bss
	InBufLen:   equ 3
	InBuf:      resb InBufLen

	OutBufLen:  equ 4
	OutBuf:     resb OutBufLen

section .text
	global _start

_start:

read:
	; clear previous input
	mov byte[InBuf + 0], 0x00
	mov byte[InBuf + 1], 0x00
	mov byte[InBuf + 2], 0x00
	mov byte[InBuf + 3], 0x00

	; read bytes from the input
	xor rax, rax                  	; sys_read
	xor rdi, rdi                  	; stdin
	mov rsi, InBuf
	mov rdx, InBufLen
	syscall

	cmp rax, 0						; did we receive any input
	je exit

	; reset registers
	xor rdi, rdi					; input bytes processed
	xor r12, r12					; output bytes written

encode:
	; reset registers
	xor rbx, rbx
	xor rdx, rdx

	; load 3 bytes in the right order
	mov dh, byte [InBuf + rdi]
	shl rdx, 8
	mov dh, byte [InBuf + rdi + 1]
	mov dl, byte [InBuf + rdi + 2]

	; 1st 6-bit block
	mov rbx, rdx
	shr rbx, 18
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12

	; 2nd 6-bit block
	mov rbx, rdx
	shr rbx, 12
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

	; did we process all chars
	cmp rdi, rax
	je add_double_equal

	; 3rd 6-bit block
	mov rbx, rdx
	shr rbx, 6
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

	; did we process all chars
	cmp rdi, rax
	je add_equal

	; 4th 6-bit block
	mov rbx, rdx
	and rbx, 0b111111
	add rbx, digits
	mov bl , byte [rbx]
	mov byte [OutBuf + r12], bl
	inc r12
	inc rdi

write_buffer:
	; write OutBuf to stdout
	mov rax, 1					; sys_write
	mov rdi, 1					; stdout
	mov rsi, OutBuf
	mov rdx, OutBufLen
	syscall

	jmp read

exit:
	; properly terminate the program
	mov rax, 60                 ; sys_exit
	xor rdi, rdi                ; success
	syscall

add_double_equal:
	mov byte [OutBuf + r12], 0x3d
	inc r12

add_equal:
	mov byte [OutBuf + r12], 0x3d
	inc r12
	jmp write_buffer

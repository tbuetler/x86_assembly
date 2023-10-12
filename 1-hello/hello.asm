;	Executable name	:	1-hello
;	Version			:	1.0
;	Created date	:	10.10.2023
;	Last update		:	10.10.2023
;	Author			:	Tim Bütler
; 	Description		:	A simple program to print "Hello world!"
;

SECTION .data           ; Section containing initialised data

	HelloMsg: db "Hello world!", 10	;	Text that should be printed, 10 means new line
	HelloLen: equ $-HelloMsg		;	Compute the length of the string

SECTION .bss            ; Section containing uninitialized data

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!

_start:

    ;; Add *your code* here to make this program print "Hello world"!

	mov rax, 1			; Set RAX to 1 (syscall number for write)
	mov rdi, 1			; Set RDI to 1 (file descriptor for STDOUT)

	mov rsi, HelloMsg	; Set RSI to the address of the HelloMsg string
	mov rdx, HelloLen	; Set RDX to the length of the HelloMsg string
	syscall				; Invoke the syscall to write the string to STDOUT

    mov rax, 60         ; Code for exit
    mov rdi, 0          ; Return a code of zero
    syscall             ; Make kernel call

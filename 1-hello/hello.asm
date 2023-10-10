;	Executable name	:	1-hello
;	Version			:	1.0
;	Created date	:	10.10.2023
;	Last update		:	10.10.2023
;	Author			:	Tim Bütler
; 	Description		:	A simple program to print "Hello world!"
;

SECTION .data           ; Section containing initialised data

	HelloMsg: db "Hello world!", 10	;	Text that should be printed
	HelloLen: equ $-HelloMsg		;	Compute the length of the string

SECTION .bss            ; Section containing uninitialized data

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!

_start:

    ;; Add *your code* here to make this program print "Hello world"!

	mov rax, 1
	mov rdi, 1

	mov rsi, HelloMsg
	mov rdx, HelloLen
	syscall

    mov rax, 60         ; Code for exit
    mov rdi, 0          ; Return a code of zero
    syscall             ; Make kernel call

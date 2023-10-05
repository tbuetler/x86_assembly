SECTION .data           ; Section containing initialised data

SECTION .bss            ; Section containing uninitialized data

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!

_start:

    ;; Add *your code* here to make this program print "Hello world"!

    mov rax, 60         ; Code for exit
    mov rdi, 0          ; Return a code of zero
    syscall             ; Make kernel call

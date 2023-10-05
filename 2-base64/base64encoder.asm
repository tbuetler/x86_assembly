SECTION .data           ; Section containing initialised data
    Base64Table: db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

SECTION .bss            ; Section containing uninitialized data

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!

_start:

    ;; Add *your code* here!

    mov rax, 60         ; Code for exit
    mov rdi, 0          ; Return a code of zero
    syscall             ; Make kernel call

SECTION .data           ; Section containing initialised data
    lookup: db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

SECTION .bss            ; Section containing uninitialized data

    ; Reserve a buffer for reading from stdin
    InBufLen:   equ 3
    InBuf:      resb InBufLen    ; reserve uninitialized memory

    OutBufLen:  equ 4
    OutBuf:     resb InBufLen

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!


_start:
read: 
    ;read chunk from stdin to InBuf
    mov rax, 0                      ; sys_read
    mov rdi, 0                      ; file descriptor: stdin
    mov rsi, InBuf                  ; destination buffer
    mov rdx, InBufLen               ; maximum # of bytes to read
    syscall                         ; reads the next chunk. bytes read are written to InBuf, number of bytes read in rax

    cmp rax, 0                      ; did we receive any bytes?
    je exit                         ; if not: exit the program

    xor r12, r12
    mov rdx, [InBuf]

edit:
    cmp rax, 1
    je add_doubleequal

    cmp rax, 2
    je add_equal


process:
    ;mask & shift to right place in 6 bit blocks

    ;edit 1 byte
    shr rbx, 18                                 ;get bits to the riht place
    and rbx, 0b111111                           ;mask 
    mov rbx, [lookup + rbx]                     ;lookup at digits + 
    mov [OutBuf + r12], rbx                     ;write to OutBuf
    inc r12                                     ;incr OutBuf coutner
    mov rbx, [InBuf]                            ;write input back to rbx
    
    ;edit 2 byte
    shr rbx, 12                                 ;get bits to the riht place
    and rbx, 0b111111                           ;mask 
    mov rbx, [lookup + rbx]                     ;lookup at digits + 
    mov [OutBuf + r12], rbx                     ;write to OutBuf
    inc r12                                     ;incr OutBuf coutner
    mov rbx, [InBuf]                            ;write input back to rbx

    ;edit 3 byte
    shr rbx, 6                                  ;get bits to the riht place
    and rbx, 0b111111                           ;mask 
    mov rbx, [lookup + rbx]                     ;lookup at digits + 
    mov [OutBuf + r12], rbx                     ;write to OutBuf
    inc r12                                     ;incr OutBuf coutner
    mov rbx, [InBuf]                            ;write input back to rbx

    ;edit 4 byte
    and rbx, 0b111111                           ;mask 
    mov rbx, [lookup + rbx]                     ;lookup at digits + 
    mov [OutBuf + r12], rbx                     ;write to OutBuf
    inc r12                                     ;incr OutBuf coutner
    mov rbx, [InBuf]                              ;write input back to rbx

    jmp writeline                               ;jump to writeline
  

writeline:
    ; Write InBuf to stdout
    mov rax, 1                      ; sys_write
    mov rdi, 1                      ; file descriptor: stdout
    mov rsi, OutBuf                ; output buffer
    mov rdx, OutBufLen            ; # of bytes to write
    syscall

    
    jmp read; Go back to read the next bytes from input



exit:
    mov rax, 60         ; Code for exit
    mov rdi, 0          ; Return a code of zero
    syscall             ; Make kernel call


add_doubleequal:
    mov rcx, '='
    mov [InBuf + 1], rcx                    ;write one '=' to OutBuf
  
add_equal:

    mov rcx, '='
    mov [InBuf + 2], rcx                    ;write one '=' to OutBuf

    jmp process
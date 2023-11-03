; Author: Tim Bütler
; 01.11.2023
;
; - Used registers:
; - rax   Input data
; - ecx   number of bytes read from input
; - r11   current position in InBuf
; - r12   current position in OutBuf

SECTION .data                   ; Section containing initialised data
    Digits:     db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

SECTION .bss                    ; Section containing uninitialized data
    InBufLen:   equ 3
    InBuf:      resb InBufLen

    OutBufLen:  equ 5
    OutBuf:     resb OutBufLen

SECTION .text                   ; Section containing code

global _start                   ; Linker needs this to find the entry point!

_start:

read:
    ; read 3 bytes from the input
    mov rax, 0                  ; sys_read
    mov rdi, 0                  ; file descriptor
    mov rsi, InBuf              ; destination buffer
    mov rdx, InBufLen           ; maximum # of bytes to read
    syscall

    ; check the number of bytes read
    cmp rax, 0                  ; did we receive any bytes?
    je exit                     ; if not, exit the program

    ; Prepare registers for loop
    mov ecx, eax                ; save number of bytes read to work with
    xor r11, r11                ; set current position in InBuf to 0
    xor r12, r12                ; set current position in OutBuf to 0

process:
    ; check if we have at least 1 byte to encode
    cmp ecx, 0
    jle writeline

    ; get the 1st 6 bits
    mov rbx, 0                  ; clear rbx
    mov bl, byte [InBuf + r11]  ; load next byte from InBuf
    shr ebx, 2                  ; shift right to get the 1st 6 bits
    and ebx, 0x3F               ; mask the bits
    mov al, byte [Digits + rbx] ; get the corresponding character
    mov byte [OutBuf + r12], al ; store it in OutBuf
    inc r12                     ; increment the Output position
    dec ecx                     ; decrement the byte count
    inc r11                     ; increment the input position

    ; get the 2nd 6 bits
    cmp ecx, 0
    jle add_double_equal        ; add padding if necessary
    mov rbx, 0                  ; clear rbx
    mov bl, byte [InBuf + r11]  ; load next byte from InBuf
    shl ebx, 4                  ; shift left to get the next 6 bits
    shr ebx, 2                  ; shift right to get the 2nd 6 bits
    and ebx, 0x3F               ; mask the bits
    mov al, byte [Digits + rbx] ; get the corresponding character
    mov byte [OutBuf + r12], al ; store it in OutBuf
    inc r12                     ; increment the Output position
    dec ecx                     ; decrement the byte count
    inc r11                     ; increment the input position

    ; get the 3rd 6 bits
    cmp ecx, 0
    jle add_equal               ; add single padding if necessary
    mov rbx, 0                  ; clear rbx
    mov bl, byte [InBuf + r11]  ; load next byte from InBuf
    shl ebx, 2                  ; shift left to get the next 6 bits
    shr ebx, 2                  ; shift right to get the 3rd 6 bits
    and ebx, 0x3F               ; mask the bits
    mov al, byte [Digits + rbx] ; get the corresponding character
    mov byte [OutBuf + r12], al ; store it in OutBuf
    inc r12                     ; increment the Output position
    dec ecx                     ; decrement the byte count
    inc r11                     ; increment the input position

    ; get the 4th 6 bits
    mov rbx, 0                  ; clear rbx
    shl ebx, 6                  ; shift left to get the next 6 bits
    and ebx, 0x3F               ; mask the bits
    mov al, byte [Digits + rbx] ; get the corresponding character
    mov byte [OutBuf + r12], al ; store it in OutBuf
    inc r12                     ; increment the Output position
    dec ecx                     ; decrement the byte count
    inc r11                     ; increment the input position

    jmp process                 ; process the next group of bytes

add_equal:
    ; add single padding
    mov byte [OutBuf + r12], 0x3d ; add the equal sign
    inc r12                      ; increment the Output position
    jmp writeline

add_double_equal:
    ; add double padding
    mov byte [OutBuf + r12], 0x3d ; add the equal sign
    inc r12                      ; increment the Output position
    mov byte [OutBuf + r12], 0x3d ; add the equal sign
    inc r12                      ; increment the Output position
    jmp writeline

writeline:
    ; write OutBuf to stdout
    mov rax, 1
    mov rdi, 1
    mov rsi, OutBuf
    mov rdx, r12
    syscall

    ; Go back to read the next bytes from input
    jmp read

exit:
    ; add new line
    mov byte [OutBuf], 10        ; new line
    mov rax, 1
    mov rdi, 1
    mov rsi, OutBuf
    mov rdx, 1
    syscall

    ; properly terminate the program
    mov rax, 60                 ; sys_exit
    xor rdi, rdi                ; return 0 for success
    syscall

SECTION .data
Base64Table: db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"


SECTION .bss

byteStorage: resb 30000
bytesToReadAtOnce: equ 30000


b64EncStor: resb 40000
b64EncStorLen: equ $ - b64EncStor


SECTION .text

global _start

_start:

;sys read   put everything from the file into the buffer "byteStorage"
mov rax, 0
mov rdi, 0
mov rsi, byteStorage
mov rdx, bytesToReadAtOnce
syscall


xor r11, r11 ; syscall strangely changes r11
xor r12, r12    ;r12 will keep track of index in byteStorage array
mov r13, 0  ;r13 will keep track of index in b64EncStor array


.encodingInProgress:

cmp rax, 0
je .weHaveFinished      ;if no bits remaining, and no extra one or two
                        ;bytes, we simply jump to the end
dec rax
inc r12


mov r8b, [byteStorage + r12 -1]     ; put each input char in a register each

mov r11b, r8b
shr r11b, 2
and r11b, 0x3F

mov r11b, [Base64Table + r11]

mov [b64EncStor + r13], r11b    ; our first char is now encoded
inc r13

cmp rax, 0      ;if rax = 0, rax was one before above decrementation, so we jump
je .oneExtraByte    ;to .oneExtraByte

;char two

dec rax
inc r12

mov r9b, [byteStorage + r12-1]  ; put each input char in a register each

and r8b, 0x3
shl r8b,4
mov r11b, r9b
shr r11b, 4
and r11b, 0xF
add r8b, r11b

mov r8b, [Base64Table + r8]

mov [b64EncStor+r13], r8b   ; second char now encoded
inc r13

cmp rax, 0      ;rax was two before being decremented twice above, so we
je .twoExtraBytes  ;jump to .twoExtraBytes

;char three

dec rax
inc r12

mov r10b, [byteStorage + r12-1] ;put each input char in a register each

and r9b, 0xF
shl r9b, 2
mov r8b, r10b
shr r8b, 6
and r8b, 0x3
add r9b, r8b

mov r9b, [Base64Table +  r9]

mov [b64EncStor+r13], r9b   ; third char now encoded
inc r13

;char four

and r10b, 0x3F

mov r10b, [Base64Table + r10]

mov [b64EncStor+r13], r10b  ; fourth char now encoded
inc r13

jmp .encodingInProgress

;--------

.oneExtraByte:  ;so we need four (and not two !) bits more to reach 12

shl r8b, 4
and r8b, 0x3F   ;only keep six bits from left, the two most right are zero

mov r8b, [Base64Table + r8]

mov [b64EncStor + r13], r8b
inc r13

mov r8b, "="        ;add two extra equal signs

mov [b64EncStor + r13], r8b
inc r13
mov [b64EncStor + r13], r8b
inc r13

jmp .weHaveFinished

;------

.twoExtraBytes: ;so we need two (and not four !) bits more to reach 18

;inc r12

mov r10b, [byteStorage + r12-1] ;put each input char in a register each

shl r10b, 2
and r10b, 0x3F  ;only keep six bits from left, the two most right are zero

mov r10b, [Base64Table + r10]

mov [b64EncStor + r13], r10b
inc r13

mov r8b, "="    ;add one extra equal sign

mov [b64EncStor  + r13], r8b
inc r13

jmp .weHaveFinished

;--------

.weHaveFinished:

;syscall for write, to output the result
mov rax, 1
mov rdi, 1
mov rsi, b64EncStor
mov rdx, r13
syscall


xor r12,r12

mov rax, 60         ; System call for exit
mov rdi, 0
syscall
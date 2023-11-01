; Author: Tim Bütler
; 01.11.2023
; With a bit help of ChatGPT to get the errors away :)

SECTION .data           ; Section containing initialised data
    Base64Table:        db "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

SECTION .bss            ; Section containing uninitialized data
    ChunkSize:          equ     19        ; size in 3-byte work units (57 input bytes)
    ReadBufferSize:     equ     ChunkSize * 3
    OutputBufferSize:   equ     ChunkSize * 4
    OutputBufer:        resb    OutputBufferSize + 16
        ; +16 bytes is padding (mem-access and '='/EOL overwrite zone)

    ReadBuffer:         equ     OutputBufer + ChunkSize + 4
        ; read buffer is shared with output -> advanced just enough to let output
        ; overwrite only source data which were already processed

SECTION .text           ; Section containing code

global _start           ; Linker needs this to find the entry point!

_start:
    ; load some common values into preserved registers (these are "global")
    xor     ebp,ebp             ; rbp = 0
    mov     r12,ReadBuffer
    mov     r13,OutputBufer

.processChunkLoop:
    call    readInput
    test    rax,rax
    jle     .err_or_exit        ; error or zero length -> exit
    call    processChunk        ; leaves size of output in rdi
    call    writeOutput
    jmp     .processChunkLoop   ; loop until full input was processed

.err_or_exit:
    mov     rdi,rax             ; error code = 0 when OK, otherwise err_no of sys_read()
    mov     rax,60              ; sys_exit
    syscall

readInput:      				; out rax = length or error (<0)
    xor     eax,eax             ; rax = 0 = sys_read
    xor     edi,edi             ; rdi = 0 = stdin
    mov     rsi,r12             ; rsi = read buffer for data
    mov     edx,ReadBufferSize  ; rdx = bytes to read (32b value)
    syscall
    ret

writeOutput:    				; in rdi = length of output data to print
    mov     rdx,rdi             ; output size
    mov     rsi,r13             ; OutputBufer
    lea     rax,[rbp+1]         ; rax = 1 (sys_write)
    mov     rdi,rax             ; rdi = 1 (stdout)
    syscall
    ret

processChunk:
								; rax = size of data to process (non zero value)
    mov     rsi,r12             ; rsi = ReadBuffer
    mov     rdi,r13             ; rdi = OutputBufer
    lea     r8,[rsi+rax]        ; r8 = first byte after bytes read (address)
    mov     [r8],ebp            ; make sure input data are padded with 4+ zeroes
    ; convert input data into base64 encoding

.loop:
    mov     eax,[rsi]
    add     rsi,3
    bswap   eax
    shr     eax,8               ; 24 bits (4x6) of input in big-endian order
        						; => b23..b18 is first 6-bits value to output, b5..b0 is last one
    mov     edx,eax             ; copy eax into edx
    shr     eax,6               ; remove 6 from eax because they will already be processed
    and     edx,0x3F            ; keep only last 6 bits
    mov     bh,[Base64Table+rdx]; translate into base64 (fourth output char)
    mov     edx,eax             ; get copy of next 6 bits
    shr     eax,6               ; remove 6 processed bits
    and     edx,0x3F            ; keep only last 6 bits
    mov     bl,[Base64Table+rdx]; translate into base64 (third output char)
    shl     ebx,16              ; shift fourth+third char into upper 16 bits of ebx
    mov     edx,eax             ; get next 6 bits
    shr     eax,6               ; remove 6 processed bits
    and     edx,0x3F            ; keep only last 6 bits
    mov     bh,[Base64Table+rdx]; convert bits into base64 (second output char)
    mov     bl,[Base64Table+rax]; convert bits into base64 (first output char)
    mov     [rdi],ebx           ; write four output bytes
    add     rdi,4
    cmp     rsi,r8
    jb      .loop               ; loop until all input data were processed (rsi >= r8)
   								; patch the output chunk to end with '=' or '==' if (0 != (size%3))
    sub     r8,rsi              ; r8 = 0, -1 (one char extra in output) or -2 (two chars extra)
    mov     word [rdi+r8],'=='  ; overwrite any extra chars in output with '='
   								; add newline after output
    mov     byte [rdi],10
    inc     rdi
    sub     rdi,r13             ; rdi = size of output data
    ret
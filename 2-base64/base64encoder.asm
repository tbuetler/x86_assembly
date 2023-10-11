section .data
    ; Define the base64 encoding table
    base64_table db 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/'

section .bss
    ; Define variables
    input_buffer resb 3
    output_buffer resb 4

section .text
    global _start

_start:
    ; Read the binary input data from standard input
    mov eax, 3      ; File descriptor for standard input (stdin)
    mov ebx, 0      ; File descriptor for standard input (stdin)
    mov ecx, input_buffer    ; Buffer to store the input data
    mov edx, 3      ; Number of bytes to read
    int 0x80        ; Call the read system call

    ; Convert the input data to base64
    mov esi, input_buffer    ; Pointer to the input data
    mov edi, output_buffer   ; Pointer to the output data

    movzx eax, byte [esi]    ; Load the first byte of the input data
    shr eax, 2      ; Shift right by 2 bits to get the first 6 bits
    movzx ebx, byte [base64_table + eax]   ; Look up the corresponding base64 character
    mov byte [edi], bl       ; Store the base64 character in the output buffer

    movzx eax, byte [esi + 1]    ; Load the second byte of the input data
    and eax, 0x03    ; Mask the first 2 bits
    shl eax, 4      ; Shift left by 4 bits to get the next 6 bits
    movzx ebx, byte [esi]    ; Load the first byte of the input data again
    shr ebx, 6      ; Shift right by 6 bits to get the last 2 bits
    or eax, ebx     ; Combine the bits
    movzx ebx, byte [base64_table + eax]   ; Look up the corresponding base64 character
    mov byte [edi + 1], bl       ; Store the base64 character in the output buffer

    movzx eax, byte [esi + 1]    ; Load the second byte of the input data again
    and eax, 0x3F    ; Mask the first 6 bits
    shl eax, 2      ; Shift left by 2 bits to get the next 6 bits
    movzx ebx, byte [esi + 2]    ; Load the third byte of the input data
    shr ebx, 4      ; Shift right by 4 bits to get the first 4 bits
    or eax, ebx     ; Combine the bits
    movzx ebx, byte [base64_table + eax]   ; Look up the corresponding base64 character
    mov byte [edi + 2], bl       ; Store the base64 character in the output buffer

    movzx eax, byte [esi + 2]    ; Load the third byte of the input data again
    and eax, 0x0F    ; Mask the first 4 bits
    shl eax, 6      ; Shift left by 6 bits to get the next 6 bits
    movzx ebx, byte [base64_table + eax]   ; Look up the corresponding base64 character
    mov byte [edi + 3], bl       ; Store the base64 character in the output buffer

    ; Print the base64 representation on standard output
    mov eax, 4      ; File descriptor for standard output (stdout)
    mov ebx, 1      ; File descriptor for standard output (stdout)
    mov ecx, output_buffer   ; Buffer to store the output data
    mov edx, 4      ; Number of bytes to write
    int 0x80        ; Call the write system call

    ; Exit the program
    mov eax, 1      ; Exit system call number
    xor ebx, ebx    ; Exit status
    int 0x80        ; Call the exit system call

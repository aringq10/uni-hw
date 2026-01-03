
    ;
    ; Writes byte values 0 - 255 to given output file
    ;

.model small
.stack 100h
.data
    fn_out db 255 dup(0)
    fd_out dw ?
    buff_out db 256 dup(?)

    err_msg db "Invalid input$"
    usage_msg db "Usage: out256.exe output.file", 0dh, 0ah
              db "Argument filename lengths must be in range [1; 12]$"
    file_err_msg db "Something went wrong with one of the files. Error code: $"
    newl db 0dh, 0ah, '$'

; MACROS
print_string macro string
    mov ah, 09h
    mov dx, offset string
    int 21h
endm

print_char macro char
    mov ah, 02h
    mov dl, char
    int 21h
endm

terminate macro
    mov ah, 4ch
    int 21h
endm

.code
start:
    mov ax, @data
    mov ds, ax
    
    xor cx, cx
    mov cl, es:[80h]

    ; read second arg
    xor si, si
    xor bx, bx
    mov di, offset fn_out
    call get_arg
    cmp bx, 0
    je print_err
    cmp bx, 12
    jg print_err

    ; create or open file 'fn_out' for writing
    mov ax, 3c00h
    xor cx, cx
    mov dx, offset fn_out
    int 21h
    mov fd_out, ax
    jc file_err

    call hex_dump

    ; close both files
    mov ax, 3e00h
    mov bx, fd_out
    int 21h

done:
    terminate

print_err:
    print_string err_msg
    print_string newl
    print_string usage_msg
    terminate

print_usage:
    print_string usage_msg

    terminate

file_err:
    print_string file_err_msg
    mov bl, al
    call byte_to_hex
    print_char bh
    print_char bl
    print_char 'h'
    terminate

hex_dump proc
    ; Loop through input file in blocks of 512 bytes:

    mov cx, 256
    mov di, offset buff_out
    mov bx, 0

    .loop:
        mov [di], bx
        inc di
        inc bx
    loop .loop

    mov cx, 256
    mov ax, 4000h
    mov bx, fd_out
    mov dx, offset buff_out
    int 21h

    .done:
        ret
hex_dump endp

byte_to_hex proc
    ; take byte in bl, output higher 4 bit's
    ; hex value in bh and the lower 4 bit's in bl
    ; *uses hex_string
    push dx
    mov bh, bl
    shr bh, 4
    and bl, 15
    push bx
    xor bh, bh

    mov dl, [hex_string + bx]
    pop bx
    mov bl, bh
    xor bh, bh
    mov bh, [hex_string + bx]
    mov bl, dl
    pop dx
    ret
byte_to_hex endp

get_arg proc
    ; Input:
    ; si - offset from start of arguments string
    ; cx - total length or arguments string at 80h
    ; di - destination address for argument string
    ; Output:
    ; bx - argument's length

    .skip_spaces:
        cmp si, cx
        jge .end_arg
        mov al, es:[81h + si]
        cmp al, ' '
        je .inc_si
        cmp al, 9
        je .inc_si
        jmp .copy
    .inc_si:
        inc si
        jmp .skip_spaces

    .copy:
        cmp si, cx
        jge .end_arg
        mov al, es:[81h + si]
        cmp al, ' '
        je .end_arg
        cmp al, 9
        je .end_arg
        je .end_arg

        mov [di], al
        inc bx
        inc di
        inc si
        jmp .copy

    .end_arg:
        ret
get_arg endp

end start

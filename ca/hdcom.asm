
    ;
    ; A hex dump program written in 8086 assembly: (COM version)
    ; used dosbox, tasm and tlink.
    ;
    ; Usage:
    ; hd.asm input.file output.file
    ;
.model tiny
.code
org 100h
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

start:
    xor cx, cx
    mov cl, ds:[80h]

    ; read first arg
    xor bx, bx
    xor si, si
    mov di, offset fn_in
    call get_arg
    cmp bx, 0
    je print_err
    cmp bx, 12
    jg print_err
    ; check for /?
    cmp bx, 2
    jne continue
    cmp [fn_in], '/'
    jne continue
    cmp [fn_in + 1], '?'
    je print_usage

continue:
    ; read second arg
    xor bx, bx
    mov di, offset fn_out
    call get_arg
    cmp bx, 0
    je print_err
    cmp bx, 12
    jg print_err

    ; open file 'fn_in' for reading
    mov ax, 3d00h
    mov dx, offset fn_in
    int 21h
    mov al, '0'
    mov [hex_string], al
    mov fd_in, ax
    jc file_err
    
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
    mov ax, 3e00h
    mov bx, fd_in
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
    ; fd_in -> buff_in.
    ; Loop through the block and write each byte's hex
    ; value to buff_out and finally buff_out -> fd_out.

    .read_loop:
        ; read from file
        mov dx, offset buff_in
        mov ax, 3f00h
        mov bx, fd_in
        mov cx, 512
        int 21h
        jc file_err

        mov cx, ax
        jcxz .done

        mov si, offset buff_in
        mov di, offset buff_out
        xor dl, dl ; column counter 0-16

        .hex_loop:
            mov bl, [si]
            call byte_to_hex
            mov [di], bh
            mov [di + 1], bl
            mov byte ptr [di + 2], ' '
            add di, 3
            inc si

            inc dl
            cmp dl, 16
            jne .no_nl
                mov byte ptr [di], 0dh
                inc di
                mov byte ptr [di], 0ah
                inc di
                xor dl, dl
            .no_nl:

        loop .hex_loop

        ; write to file, compute byte count to write:
        ; cx = offset buff_out - di
        mov ax, di
        sub ax, offset buff_out
        mov cx, ax
        mov dx, offset buff_out
        mov ax, 4000h
        mov bx, fd_out
        int 21h
        jc file_err

    jmp .read_loop

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
        mov al, ds:[81h + si]
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
        mov al, ds:[81h + si]
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

    fn_in db 255 dup(0)
    fn_out db 255 dup(0)
    buff_in db 512 dup(?)
    buff_out db 1728 dup(?)     ; (512 * 3) + ((512 * 3) / 16) * 2
                                ; One byte is chars [0; f] + space and
                                ; each line of (16) bytes in hex requires 2 bytes for \n
    fd_in dw ?
    fd_out dw ?
    hex_string db "0123456789abcdef$"
    err_msg db "Invalid input$"
    usage_msg db "Usage: dasm.exe input.exe output.asm", 0dh, 0ah
              db "Argument filename lengths must be in range [1; 12]$"
    file_err_msg db "Something went wrong with one of the files. Error code: $"
    newl db 0dh, 0ah, '$'

end start

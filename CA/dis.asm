
    ;
    ; A dissembler for 8086 assembly .COM files:
    ; used dosbox, tasm and tlink.
    ;
    ; Usage:
    ; dis.exe input.com output.asm
    ;

.model small
.stack 100h
.data
    fn_in db 255 dup(0)
    fn_out db 255 dup(0)
    fd_in dw ?
    fd_out dw ?
    buff_in db 512 dup(?)

    ;
    ; STATS FOR EACH CMD:
    ; FIRST BYTE MAY HAVE FLAGS D AND W,
    ; SECOND BYTE IS MADE UP OF MOD REG R/M
    ;                            00 000 000
    ; md - MOD
    ; reg - REG
    ; rm - R/M
    ; 
    ; cmd_off_b - AMOUNT OF OFFSET BYTES
    ;                      (USED FOR COMPUTING ADDRESS)
    ; cmd_offs - ACTUAL OFFSET BYTES
    ;
    cmd_len db 0
    first_byte db 0
    sec_byte db 0
    d db 0
    w db 0
    md db 0
    rm db 0
    reg db 0
    cmd_off_b db 0
    cmd_offs db 0, 0
    cmd_offs_hex db 4 dup(0)

    tmp_address dw 0
    ;
    ; STUFF FOR FORMATING CMD OUTPUT
    ;
    cmd_name db "MOV"
    cmd_hex db 32 dup (?)

    op_1 db 32 dup (?)
    op_1_len db 0
    op_2 db 32 dup (?)
    op_2_len db 0
    op_len db 0

    reg_string_w0l db "ACDBACDB"
    reg_string_w0r db "LLLLHHHH"
    reg_string_w1l db "ACDBSBSD"
    reg_string_w1r db "XXXXPPII"

    mem_struct03 db "BX + SI"
                 db "BX + DI"
                 db "BP + SI"
                 db "BP + DI"
    mem_struct47 db "SI"
                 db "DI"
                 db "BP"
                 db "BX"

    output_line db 255 dup (?)
    output_line_len db 0
    
    hex_string db "0123456789ABCDEF"

    err_msg db "Invalid input$"
    usage_msg db "Usage: dis.exe input.com output.asm", 0dh, 0ah
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

mov2len macro
    mov dl, op_len
    mov op_2_len, dl
endm

mov1len macro
    mov dl, op_len
    mov op_1_len, dl
endm

op_by_dirm macro
    local .d0
    local .end
    cmp d, 0
    je .d0
    mov [op_by_dir + 2], offset op_2
    jmp .end
    .d0:
        mov [op_by_dir], offset op_1
    .end:
endm

.code
start:
    mov ax, @data
    mov ds, ax
    
    xor cx, cx
    mov cl, es:[80h]

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
    mov fd_in, ax
    jc file_err
    
    ; create or open file 'fn_out' for writing
    mov ax, 3c00h
    xor cx, cx
    mov dx, offset fn_out
    int 21h
    mov fd_out, ax
    jc file_err

    call main_loop

    ; close both files
    mov ax, 3e00h
    mov bx, fd_out
    int 21h
    mov ax, 3e00h
    mov bx, fd_in
    int 21h

    terminate

;
; INFO/ERR PRINTING
;
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

;
; MAIN LOOP
;
main_loop proc
    ; Loop through input file in blocks of 512 bytes:
    ; fd_in -> buff_in.
    ; Parse bytes into commands and print each one to
    ; fd_out
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

        .command_parse_loop:
            ;
            ; RESET CMD STATS
            ;
            call reset_cmd_stats

            ;
            ; LOAD TWO BYTES
            ;
            mov al, [si]
            mov first_byte, al
            mov al, [si + 1]
            mov sec_byte, al
            ;
            ; PARSE COMMAND
            ;
            call first_filter

            ;
            ; UPDATE CX AND
            ; FORMAT OUTPUT
            ;
            call format_output
            xor ah, ah
            mov al, cmd_len
            sub cx, ax
            add si, ax
           
            ; 
            ; WRITE TO FILE
            ;
            call write_line_to_file 
            jc file_err

            cmp cx, 0
            jg .command_parse_loop 

    jmp .read_loop
    .done:
        ret
main_loop endp

;
; COMMAND PARSING
;
first_filter proc
    push cx
    push si

    call parse_mov 

    pop si
    pop cx
    ret
first_filter endp

parse_mov proc
    ; uses dh for tmp first_byte storage
    mov dh, first_byte
    shr dh, 4
    cmp dh, 8  ; 1000 10dw or
               ; 1000 11d0 
    jne .further
    call mov_1_6
    ret
    .further:
    cmp dh, 12 ; 1100 011w
    je .mov_2
    cmp dh, 11 ; 1011 w reg
    je .mov_3
    cmp dh, 10 ; 1010 000w or
               ; 1010 001w
    jne .parse_ret
    call mov_4_5
    ret

    .mov_2:
        call parse_2nd_byte
        mov dh, first_byte
        and dh, 1
        mov w, dh
        inc dh
        add cmd_len, dh
        ret
    .mov_3:
        mov dh, first_byte
        shr dh, 3
        and dh, 1
        mov w, dh
        inc dh
        add cmd_len, dh
        mov dh, first_byte
        and dh, 7
        mov reg, dh
        ; form op_1 
        mov di, offset op_1
        call op_from_reg
        mov1len
        ; form op_2
        mov di, offset op_2
        inc si
        call op_from_imm
        mov2len
        ret
    .parse_ret:
        ret
parse_mov endp

mov_1_6 proc
    mov dh, first_byte
    and dh, 4
    cmp dh, 4
    je .mov_6
    .mov_1:
        inc cmd_len
        call parse_2nd_byte
        mov dh, first_byte
        and dh, 1
        mov w, dh
        mov dh, first_byte
        shr dh, 1
        and dh, 1
        mov d, dh

        ;    cmp d, 0
        ;    jne .mov_1_d1
        ;    call op_1_raddress
        ;    call op_2_reg
        ;    ret
        ;    .mov_1_d1:
        ;        call op_1_reg
        ;        call op_2_raddress            
            ret
    .mov_6:
        inc cmd_len
        call parse_2nd_byte
        ret
mov_1_6 endp

mov_4_5 proc
    mov dh, first_byte
    and dh, 2
    cmp dh, 2
    je .mov_5
    .mov_4:
        ; direction: direct memory -> reg
        mov dh, first_byte
        and dh, 1
        mov w, dh
        mov reg, 0
        ; form op_1
        call op_1_reg
        ; form op_2
        inc si
        call op_2_daddress

        mov cmd_len, 3
        ret
    .mov_5:
        ; direction: reg -> direct memory
        mov dh, first_byte
        and dh, 1
        mov w, dh
        mov reg, 0
        ; form op_1
        inc si
        call op_1_daddress
        ; form op_2
        call op_2_reg

        mov cmd_len, 3
        ret
mov_4_5 endp



parse_2nd_byte proc
    ; store mod (md)
    mov al, sec_byte
    shr al, 6
    mov md, al

    ; store reg
    mov al, sec_byte
    shr al, 3
    and al, 7
    mov reg, al

    ; store r/m (rm)
    mov al, sec_byte
    and al, 3
    mov rm, al

    ; store offset length in bytes
    cmp md, 0
    je .md0
    cmp md, 1
    je .md1
    cmp md, 2
    je .md2
    cmp md, 3
    je .md3
    ret
    
    ; store offset bytes in cmd_offs,
    ; their count in cmd_off_b and
    ; their hex value in cmd_offs_hex
    .md0:
        cmp rm, 6
        jne .skip_rm6
        add cmd_len, 2
        mov cmd_off_b, 2
        mov al, [si + 2]
        mov [cmd_offs], al
        mov al, [si + 3]
        mov [cmd_offs + 1], al
        jmp .parse_end
        .skip_rm6:
            mov cmd_off_b, 0 
            mov [cmd_offs], 0
            mov [cmd_offs + 1], 0
        jmp .parse_end
    .md1:
        inc cmd_len
        mov cmd_off_b, 1
        mov al, [si + 2]
        mov [cmd_offs], al
        mov [cmd_offs + 1], 0
        jmp .parse_end
    .md2:
        add cmd_len, 2
        mov cmd_off_b, 2
        mov al, [si + 2]
        mov [cmd_offs], al
        mov al, [si + 3]
        mov [cmd_offs + 1], al
        jmp .parse_end
    .md3:
        mov cmd_off_b, 0 
        mov [cmd_offs], 0
        mov [cmd_offs + 1], 0

    .parse_end:
        cmp cmd_off_b, 1
        je .one_byte_offset
        cmp cmd_off_b, 2
        je .two_byte_offset
        ret

    .one_byte_offset:
        mov bl, [cmd_offs]
        call byte_to_hex
        mov [cmd_offs_hex], bh
        mov [cmd_offs_hex + 1], bl
        ret

    .two_byte_offset:
        mov bl, [cmd_offs]
        call byte_to_hex
        mov [cmd_offs_hex], bh
        mov [cmd_offs_hex + 1], bl       
        mov bl, [cmd_offs]
        call byte_to_hex
        mov [cmd_offs_hex + 2], bh
        mov [cmd_offs_hex + 3], bl
        ret
parse_2nd_byte endp

;
; FORMAT AND OUTPUT
;
op_1_reg proc
    mov di, offset op_1
    call op_from_reg
    mov1len
    ret
op_1_reg endp

op_2_reg proc
    mov di, offset op_2
    call op_from_reg
    mov2len
    ret
op_2_reg endp

op_1_daddress proc
    mov di, offset op_1
    call op_from_daddress
    mov1len
    ret
op_1_daddress endp

op_2_daddress proc
    mov di, offset op_2
    call op_from_daddress
    mov2len
    ret
op_2_daddress endp

op_1_raddress proc
    mov di, offset op_1
    call op_from_raddress
    mov1len
    ret
op_1_raddress endp

op_2_raddress proc
    mov di, offset op_2
    call op_from_raddress
    mov2len
    ret
op_2_raddress endp

op_1_imm proc
    mov di, offset op_1
    call op_from_imm
    mov1len
    ret
op_1_imm endp

op_2_imm proc
    mov di, offset op_2
    call op_from_imm
    mov2len
    ret
op_2_imm endp

op_from_reg proc
    ; Determines register from reg and puts
    ; its text representation into [di]

    ; get reg into bx
    xor bh, bh
    mov bl, reg

    mov op_len, 2
    cmp w, 0
    jne .w1_op1

    .w0_op1:
        mov al, [reg_string_w0l + bx]
        mov [di], al
        mov al, [reg_string_w0r + bx]
        mov [di + 1], al
        ret
    .w1_op1:
        mov al, [reg_string_w1l + bx]
        mov [di], al
        mov al, [reg_string_w1r + bx]
        mov [di + 1], al
        ret

    ;    cmp d, 0
    ;    je .d0

        ; Register is first operand
    ;    .d1:
    ;        mov op_1_len, 2

    ;        cmp w, 0
    ;        jne .w1_op1

    ;        .w0_op1:
    ;            mov al, [reg_string_w0l + bx]
    ;            mov [op_1], al
    ;            mov al, [reg_string_w0r + bx]
    ;            mov [op_1 + 1], al
    ;            ret
    ;        .w1_op1:
    ;            mov al, [reg_string_w1l + bx]
    ;            mov [op_1], al
    ;            mov al, [reg_string_w1r + bx]
    ;            mov [op_1 + 1], al
    ;            ret
    ;    ; Register is second operand
    ;    .d0:
    ;        mov op_2_len, 2

    ;        cmp w, 0
    ;        jne .w1_op2

    ;        .w0_op2:
    ;            mov al, [reg_string_w0l + bx]
    ;            mov [op_2], al
    ;            mov al, [reg_string_w0r + bx]
    ;            mov [op_2 + 1], al
    ;            ret
    ;        .w1_op2:
    ;            mov al, [reg_string_w1l + bx]
    ;            mov [op_2], al
    ;            mov al, [reg_string_w1r + bx]
    ;            mov [op_2 + 1], al
    ;            ret
op_from_reg endp

op_from_daddress proc
    ; Formats 2 byte direct address from [si] and
    ; writes its text representation into [di]

    mov op_len, 7

    mov byte ptr [di], '['
    inc di
    ; first byte
    mov bl, [si + 1]
    call byte_to_hex
    mov [di], bh
    mov [di + 1], bl
    add di, 2
    ; second byte
    mov bl, [si]
    call byte_to_hex
    mov [di], bh
    mov [di + 1], bl
    add di, 2
    
    mov byte ptr [di], 'h'
    mov byte ptr [di + 1], ']'
    ret


       ; cmp d, 0

       ; jne .daddress_op2
       ;     mov di, offset op_1
       ;     mov op_1_len, 7
       ;     jmp .write_daddress

       ; .daddress_op2:
       ;     mov di, offset op_2
       ;     mov op_2_len, 7

       ; .write_daddress:
       ;     mov byte ptr [di], '['
       ;     inc di
       ;     ; first byte
       ;     mov bl, [si + 2]
       ;     call byte_to_hex
       ;     mov [di], bh
       ;     mov [di + 1], bl
       ;     add di, 2
       ;     ; second byte
       ;     mov bl, [si + 1]
       ;     call byte_to_hex
       ;     mov [di], bh
       ;     mov [di + 1], bl
       ;     add di, 2
       ;     
       ;     mov byte ptr [di], 'h'
       ;     mov byte ptr [di + 1], ']'
       ;     ret
op_from_daddress endp

op_from_raddress proc
    ; requires cmd_offs_hex to hold the offset value in hex.
    mov tmp_address, di

    mov byte ptr [di], '[' 
    inc di

    ; get reg into bx
    xor ah, ah
    mov al, rm
    xor cx, cx
    cmp rm, 3
    jg .rm47
    .rm03:
        mov si, offset mem_struct03
        mov bx, 7
        mul bx
        add si, ax
        mov cx, 7
        jmp .write_pre_offset
    .rm47:
        cmp rm, 6   ; skip when r/m = 110 and mod = 00
        jne .skip_rm6_md0
        cmp md, 0
        je .write_pre_offset
        .skip_rm6_md0:
        mov si, offset mem_struct47
        mov bx, 2
        mul bx
        add si, ax
        mov cx, 2
    
    .write_pre_offset:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .write_pre_offset

    xor ch, ch
    mov cl, cmd_off_b 
    add cx, cx
    mov si, offset cmd_offs_hex

    .write_offset:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .write_offset

    mov byte ptr [di], ']'
    inc di

    sub di, offset tmp_address
    mov ax, di
    mov op_len, al
    ret
op_from_raddress endp

op_from_imm proc
    ; op becomes an immediate value in hex
    cmp w, 1
    je .imm_w1
    
    .imm_w0:
        mov bl, [si]
        call byte_to_hex
        mov [di], bh
        mov [di + 1], bl
        mov byte ptr [di + 2], 'h'
        mov op_len, 3
        ret
    .imm_w1:
        mov bl, [si + 1]
        call byte_to_hex
        mov [di], bh
        mov [di + 1], bl

        mov bl, [si]
        call byte_to_hex
        mov [di + 2], bh
        mov [di + 3], bl
        mov byte ptr [di + 4], 'h'
        mov op_len, 5
        ret
op_from_imm endp

format_cmd_hex proc
    xor ch, ch
    mov cl, cmd_len
    .cmd_to_hex:
        mov bl, [si]
        call byte_to_hex
        mov [di], bh
        mov [di + 1], bl
        add di, 2
        inc si
    loop .cmd_to_hex
    ret
format_cmd_hex endp

format_output proc
    push cx
    push si
    ; Format hex code for command
    mov di, offset output_line
    call format_cmd_hex
    ; Print padding
    mov cx, 13
    sub cl, cmd_len
    sub cl, cmd_len
    .format_padding:
        mov byte ptr [di], ' '
        inc di
    loop .format_padding

    ; Print first operand
    cmp op_1_len, 0
    je .end_format
    mov si, offset op_1
    xor ch, ch
    mov cl, op_1_len
    .format_op1:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op1
   
    ; Print second operand
    cmp op_2_len, 0
    je .end_format
    mov byte ptr [di], ','
    mov byte ptr [di + 1], ' '
    add di, 2
    mov si, offset op_2
    xor ch, ch
    mov cl, op_2_len 
    .format_op2:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op2
    .end_format:
        ; add new line
        ; mov byte ptr [di], 0dh
        ; inc di
        mov byte ptr [di], 0ah
        inc di

        sub di, offset output_line
        mov bx, di
        mov output_line_len, bl

        pop si
        pop cx
    ret
format_output endp

write_line_to_file proc
    push cx
    xor ah, ah
    mov al, output_line_len
    mov cx, ax
    mov ax, 4000h
    mov bx, fd_out
    mov dx, offset output_line
    int 21h
    pop cx
    ret
write_line_to_file endp

;
; HELPERS
;
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

reset_cmd_stats proc
    mov cmd_len, 1
    mov d, 0
    mov w, 0
    mov cmd_off_b, 0
    mov cmd_offs, 0
    mov md, 0
    mov reg, 0
    mov rm, 0
    mov op_1_len, 0
    mov op_2_len, 0
    mov output_line_len, 0
    ret
reset_cmd_stats endp

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

end start

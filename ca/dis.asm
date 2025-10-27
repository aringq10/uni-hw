
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

    cmd_len db 0
    first_byte db 0
    sec_byte db 0
    d db 0
    w db 0
    md db 0
    reg db 0
    rm db 0
    sreg db 0
    
    cmd_off_b db 0
    cmd_offs db 0, 0
    cmd_offs_hex db 5 dup(0)
    cmd_offs_hex_len db 0

    tmp_address dw 0
    main_loop_si dw 0
    ;
    ; STUFF FOR FORMATING CMD OUTPUT
    ;
    cmd_address dw 100h
    cmd_name dw 0

    cmd_address_padding dw 8
    cmd_hex_padding dw 22
    cmd_name_padding dw 8
    
    seg_override db 3 dup (?)
    seg_override_len db 0
    op_1 db 32 dup (?)
    op_1_len db 0
    op_2 db 32 dup (?)
    op_2_len db 0
    op_len db 0

    reg_string_w0l db "ACDBACDB"
    reg_string_w0r db "LLLLHHHH"
    reg_string_w1l db "ACDBSBSD"
    reg_string_w1r db "XXXXPPII"

    registers db "AL"
              db "CL"
              db "DL"
              db "BL"
              db "AH"
              db "CH"
              db "DH"
              db "BH"
              db "AX"
              db "CX"
              db "DX"
              db "BX"
              db "SP"
              db "BP"
              db "SI"
              db "DI"
              db "ES"
              db "CS"
              db "SS"
              db "DS"

    sreg_string db "ES"
                db "CS"
                db "SS"
                db "DS"

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

    ;
    ; Command names
    ;

    op_struct struc
        cn_ptr dw 0
        cmd_type   db 0
        op_1_   db 0
        op_2_   db 0
    ends

    include opcodes.inc

    cmd_name_len dw 0

    header db "Address|Command   Hex   Value|Command|Operands", 0ah
           db "----------------------------------------------", 0ah
    header_len dw 94

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

set_cmd macro name
    mov cmd_name, offset name
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
    
    ; create output file
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
    ; Loop through input file in blocks of 512 bytes

    ; Print header
    mov cx, header_len
    mov ax, 4000h
    mov bx, fd_out
    mov dx, offset header
    int 21h
    jc file_err

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
            mov main_loop_si, si

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
            call decode_opc

            ;
            ; UPDATE CX AND
            ; FORMAT OUTPUT
            ;
            call format_output
            xor ah, ah
            mov al, cmd_len
            sub cx, ax
            add si, ax
            add cmd_address, ax
           
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
decode_opc proc
    mov ax, size op_struct
    mov bl, first_byte
    mul bl

    lea bx, opcodes
    add bx, ax

    mov al, [bx].cmd_type
    cmp al, t_ext_opc
    jne .skip_ext_opc
        mov ax, 2
        mov dl, sec_byte
        shr dl, 3
        and dl, 7
        mul dl

        mov dx, [bx].cn_ptr
        add dx, ax
        mov bx, dx
        mov ax, [bx]
        mov cmd_name, ax
        jmp .end_decode
    .skip_ext_opc:
    mov ax, [bx].cn_ptr
    mov cmd_name, ax

    .end_decode:
    ret
decode_opc endp

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
    and al, 7
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
        jmp .md2 
        .skip_rm6:
            mov cmd_off_b, 0 
            mov cmd_offs_hex_len, 0
            mov [cmd_offs], 0
            mov [cmd_offs + 1], 0
        jmp .offset_to_hex
    .md1:
        mov cmd_offs_hex_len, 3
        inc cmd_len
        mov cmd_off_b, 1
        mov al, [si + 2]
        mov [cmd_offs], al
        mov [cmd_offs + 1], 0
        jmp .offset_to_hex
    .md2:
        mov cmd_offs_hex_len, 5
        add cmd_len, 2
        mov cmd_off_b, 2
        mov al, [si + 2]
        mov [cmd_offs], al
        mov al, [si + 3]
        mov [cmd_offs + 1], al
        jmp .offset_to_hex
    .md3:
        mov cmd_off_b, 0 
        mov [cmd_offs], 0
        mov [cmd_offs + 1], 0

    .offset_to_hex:
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
        mov byte ptr [cmd_offs_hex + 2], 'h'
        ret

    .two_byte_offset:
        mov bl, [cmd_offs + 1]
        call byte_to_hex
        mov [cmd_offs_hex], bh
        mov [cmd_offs_hex + 1], bl       
        mov bl, [cmd_offs]
        call byte_to_hex
        mov [cmd_offs_hex + 2], bh
        mov [cmd_offs_hex + 3], bl
        mov byte ptr [cmd_offs_hex + 4], 'h'
        ret
parse_2nd_byte endp

;
; FORMATTING OPERANDS
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

op_1_sreg proc
    mov di, offset op_1
    call op_from_sreg
    mov1len
    ret
op_1_sreg endp

op_2_sreg proc
    mov di, offset op_2
    call op_from_sreg
    mov2len
    ret
op_2_sreg endp

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
op_from_reg endp

op_from_sreg proc
    mov si, offset sreg_string
    xor ah, ah
    mov al, sreg
    mov bx, 2
    mul bx
    add si, ax

    mov al, [si]
    mov [di], al
    mov al, [si + 1]
    mov [di + 1], al

    mov op_len, 2
    ret
op_from_sreg endp

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
op_from_daddress endp

op_from_raddress proc
    ; requires cmd_offs_hex to hold the offset value in hex.
    mov tmp_address, di

    ; Register, not relative address
    cmp md, 3
    jne .real_raddress
    mov al, rm
    mov reg, al
    call op_from_reg
    mov op_len, 2
    ret

    .real_raddress:

    mov byte ptr [di], '[' 
    inc di


    ; Calculate string offset for mem_struct03 or mem_struct47
    ; mem_struct03: si = rm * 7
    ; mem_struct47: si = rm * 2
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
        je .daddress_md0_rm6
        .skip_rm6_md0:
        mov si, offset mem_struct47
        mov bx, 2
        sub ax, 4
        mul bx
        add si, ax
        mov cx, 2
    
    .write_pre_offset:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .write_pre_offset

    .daddress_md0_rm6:
    xor ch, ch
    mov cl, cmd_offs_hex_len
    mov si, offset cmd_offs_hex
    jcxz .skip_offset

    cmp md, 0
    je .no_plus
    mov byte ptr [di], ' '
    mov byte ptr [di + 1], '+'
    mov byte ptr [di + 2], ' '
    add di, 3

    .no_plus:
    .write_offset:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .write_offset

    .skip_offset:
    mov byte ptr [di], ']'
    inc di

    sub di, tmp_address
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

;
; FORMATTING FINAL OUTPUT
;
format_cmd_address proc
    ; Format command address
    mov bx, cmd_address
    mov bl, bh
    call byte_to_hex
    mov [di], bh
    mov [di + 1], bl
    mov bx, cmd_address
    call byte_to_hex
    mov [di + 2], bh
    mov [di + 3], bl
    mov byte ptr [di + 4], 'h'
    add di, 5
    ; Format padding
    mov cx, cmd_address_padding
    sub cx, 5
    .format_cmd_address_padding:
        mov byte ptr [di], ' '
        inc di
    loop .format_cmd_address_padding
    ret
format_cmd_address endp

format_cmd_hex proc
    ; Format current command's hex value
    xor ch, ch
    mov cl, cmd_len
    .cmd_to_hex:
        mov bl, [si]
        call byte_to_hex
        mov [di], bh
        mov [di + 1], bl
        mov byte ptr [di + 2], ' '
        add di, 3
        inc si
    loop .cmd_to_hex
    mov cx, cmd_hex_padding
    sub cl, cmd_len
    sub cl, cmd_len
    sub cl, cmd_len
    jcxz .skip_cmd_hex_padding
    ; Format padding
    .format_cmd_hex_padding:
        mov byte ptr [di], ' '
        inc di
    loop .format_cmd_hex_padding
    .skip_cmd_hex_padding:
    ret
format_cmd_hex endp

format_cmd_name proc
    ; Format command name
    mov si, cmd_name
    .format_cmd_name:
        mov al, [si]
        mov [di], al
        inc di
        inc si
        cmp byte ptr [si], '$'
        jne .format_cmd_name
    ; Skip everything if op_1 isn't set
    ; cmp op_1_len, 0
    ; je .skip_cmd_name_padding
    ; ; Format padding
    ; mov cx, cmd_name_padding
    ; sub cx, cmd_name_len
    ; .format_cmd_name_padding:
    ;     mov byte ptr [di], ' '
    ;     inc di
    ; loop .format_cmd_name_padding

    ; .skip_cmd_name_padding:
        ret
format_cmd_name endp

format_op_1 proc
    ; Format first operand
    mov si, offset op_1
    cmp byte ptr [si], '['
    jne .skip_op1_seg_override
    cmp seg_override_len, 0
    je .skip_op1_seg_override

    mov si, offset seg_override
    mov cx, 3
    .format_op1_seg_override:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op1_seg_override
    mov si, offset op_1

    .skip_op1_seg_override:
    xor ch, ch
    mov cl, op_1_len
    .format_op1:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op1
    ret 
format_op_1 endp

format_op_2 proc
    mov byte ptr [di], ','
    mov byte ptr [di + 1], ' '
    add di, 2
    ; Format second operand
    mov si, offset op_2
    cmp byte ptr [si], '['
    jne .skip_op2_seg_override
    cmp seg_override_len, 0
    je .skip_op2_seg_override

    mov si, offset seg_override
    mov cx, 3
    .format_op2_seg_override:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op2_seg_override
    mov si, offset op_2

    .skip_op2_seg_override:
    xor ch, ch
    mov cl, op_2_len 
    .format_op2:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .format_op2
    ret
format_op_2 endp

format_output proc
    push cx
    push si
    mov di, offset output_line

    call format_cmd_address
    call format_cmd_hex
    call format_cmd_name
    ; cmp op_1_len, 0
    ; je .end_format
    ; call format_op_1
    ; cmp op_2_len, 0
    ; je .end_format
    ; call format_op_2

    .end_format:
    ; *Uncomment if viewing output.asm on a non-Unix machine
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
    mov md, 0
    mov reg, 0
    mov rm, 0
    mov sreg, 0

    mov cmd_off_b, 0
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

format_seg_override proc
    mov di, offset seg_override
    mov dh, first_byte
    cmp dh, 26h
    je .seg_ov26h
    cmp dh, 2Eh
    je .seg_ov2Eh
    cmp dh, 36h
    je .seg_ov36h
    cmp dh, 3Eh
    je .seg_ov3Eh
    mov seg_override_len, 0
    ret

    .seg_ov26h:
        mov seg_override_len, 3
        mov byte ptr [di], 'E'
        mov byte ptr [di + 1], 'S'
        mov byte ptr [di + 2], ':'
        jmp .end_seg_ov
    .seg_ov2Eh:
        mov seg_override_len, 3
        mov byte ptr [di], 'C'
        mov byte ptr [di + 1], 'S'
        mov byte ptr [di + 2], ':'
        jmp .end_seg_ov
    .seg_ov36h:
        mov seg_override_len, 3
        mov byte ptr [di], 'S'
        mov byte ptr [di + 1], 'S'
        mov byte ptr [di + 2], ':'
        jmp .end_seg_ov
    .seg_ov3Eh:
        mov seg_override_len, 3
        mov byte ptr [di], 'D'
        mov byte ptr [di + 1], 'S'
        mov byte ptr [di + 2], ':'

    .end_seg_ov:
        inc cmd_len
        mov al, sec_byte
        mov first_byte, al
        mov al, [si + 2]
        mov sec_byte, al
        inc si
        inc main_loop_si
        ret
format_seg_override endp

end start

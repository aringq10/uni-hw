
    ;
    ; A disassembler for 8086 assembly .COM and .EXE files:
    ; used dosbox, tasm and tlink.
    ;
    ; Usage:
    ; dis.exe input.com output.asm
    ;
    ; Written by Aringas Civilka, Vilnius university MIF 2025
    ; https://github.com/aringq10
    ;

.model small
.stack 100h
.data
    ; Files
    fn_in db 255 dup(0)
    fn_out db 255 dup(0)
    fd_in dw ?
    fd_out dw ?
    buff db 512 dup(?)
    buff_size dw 512
    bytes_rem dw ?
    eof dw 0

    ; Command characteristics
    cmd_len db ?
    opc_byte db ?
    addr_byte db ?
    md db ?
    reg db ?
    tmp_reg db ?
    rm db ?
    cmd_disp_len db ?
    cmd_disp_b db ?, ?
    cmd_disp_hex db 6 dup(?)

    ; Current command
    cc_addr dw ?
    cc_type db ?
    cc_op_1 db ?
    cc_op_2 db ?
    tmp_op  db ?
    ; Address byte already parsed flag
    addr_b_parsed_f db ?

    main_loop_si dw ? ; Address of current command in buff_in
    imm_offset dw ?   ; Offset from main_loop_si for
                      ; non_addr_byte operands to read from

    op_struct struc
        cn_ptr      dw ?
        cmd_type    db ?
        op_1_code   db ?
        op_2_code   db ?
    ends

    include opcodes.inc

    ; Stuff for formatting output
    disp_address dw ?
    cmd_address dw 100h
    cmd_name dw ?
    cmd_name_len dw ?

    cmd_address_padding dw 8
    cmd_hex_padding dw 22
    cmd_name_padding dw 10
    
    seg_ov dw ?
    seg_ov_string db "ES:$"
                  db "CS:$"
                  db "SS:$"
                  db "DS:$"
                  db "$"

    op_1 db 32 dup (?)
    op_2 db 32 dup (?)

    registers db "AL$"
              db "CL$"
              db "DL$"
              db "BL$"
              db "AH$"
              db "CH$"
              db "DH$"
              db "BH$"
              db "AX$"
              db "CX$"
              db "DX$"
              db "BX$"
              db "SP$"
              db "BP$"
              db "SI$"
              db "DI$"
              db "ES$"
              db "CS$"
              db "SS$"
              db "DS$"
              db "1 $"
              db "3$"

    mem_struct03 db "BX + SI$"
                 db "BX + DI$"
                 db "BP + SI$"
                 db "BP + DI$"
    mem_struct47 db "SI$"
                 db "DI$"
                 db "BP$"
                 db "BX$"

    output_line db 255 dup (?)
    output_line_len dw ?
    
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

; CODE ENTRY
.code
start:
    mov ax, @data
    mov ds, ax
    
    call get_2_args

    call open_files

    call main_loop

    call close_files

    terminate

; MAIN LOOP
main_loop proc
    call check_for_exe
    call initial_read

    mov si, offset buff
    mov cx, ax
    cmp cx, 0
    jne .command_parse_loop
    ret

    .command_parse_loop:
        call reset_cmd_stats
        mov main_loop_si, si

        mov al, [si]
        mov opc_byte, al
        mov al, [si + 1]
        mov addr_byte, al

        call decode_opc
        call form_operand_strings
        call format_output_line

        ; Update cx, si and cmd_address using cmd_len
        xor ah, ah
        mov al, cmd_len
        sub cx, ax
        add si, ax
        add cmd_address, ax
       
        call write_line_to_file 

        ; Read more bytes to fill buffer in case
        ; there are less than 8 bytes left.
        ; (max command length is 7 bytes)
        cmp eof, 1
        je .continue_parsing
        cmp cx, 7
        jg .continue_parsing
        cmp cx, 1
        jb .continue_parsing
        
        call update_buffer

        .continue_parsing:
        cmp cx, 0
        jg .command_parse_loop 

    ret
main_loop endp

; COMMAND PARSING
decode_opc proc
    push si
    push cx

    call check_seg_ov

    mov ax, size op_struct
    mov bl, opc_byte
    mul bl

    lea bx, opcodes
    add bx, ax
    mov cc_addr, bx
    mov al, [bx].cmd_type

    cmp al, t_ext_opc
    jne .skip_ext_opc
    ; In case of extended opcode
    mov ax, 2
    mov dl, addr_byte
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

    mov bx, cc_addr
    mov al, [bx].cmd_type
    mov cc_type, al
    mov al, [bx].op_1_code
    mov cc_op_1, al
    mov al, [bx].op_2_code
    mov cc_op_2, al
    
    ; implement f6_f7 TEST command, since it has
    ; different layout than other f6_f7 commands
    cmp opc_byte, 0F6h
    jne .skip_f6_impl
    call implement_test_f6_f7
    jmp .skip_f7_impl

    .skip_f6_impl:
    cmp opc_byte, 0F7h
    jne .skip_f7_impl
    call implement_test_f6_f7

    .skip_f7_impl:
    pop cx
    pop si
    ret
decode_opc endp

parse_addr_byte proc
    push di

    mov addr_b_parsed_f, 1
    mov si, main_loop_si

    inc cmd_len
    ; store mod (md)
    mov al, addr_byte
    shr al, 6
    mov md, al

    ; store reg
    mov al, addr_byte
    shr al, 3
    and al, 7
    mov reg, al

    ; store r/m (rm)
    mov al, addr_byte
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
    
    ; Store displacement bytes in cmd_disp_b,
    ; their count in cmd_disp_len and
    ; their hex value in cmd_disp_hex.
    ; Update cmd_len accordingly
    .md0:
        cmp rm, 6
        jne .skip_rm6
        jmp .md2 
        .skip_rm6:
            mov cmd_disp_len, 0
        jmp .disp_to_hex
    .md1:
        inc cmd_len
        mov cmd_disp_len, 1
        mov al, [si + 2]
        mov [cmd_disp_b], al
        jmp .disp_to_hex
    .md2:
        add cmd_len, 2
        mov cmd_disp_len, 2
        mov al, [si + 2]
        mov [cmd_disp_b], al
        mov al, [si + 3]
        mov [cmd_disp_b + 1], al
        jmp .disp_to_hex
    .md3:
        mov cmd_disp_len, 0 

    .disp_to_hex:
        mov si, offset cmd_disp_b
        mov di, offset cmd_disp_hex

        cmp cmd_disp_len, 1
        je .one_byte_disp
        cmp cmd_disp_len, 2
        je .two_byte_disp
        jmp .end_pars_addr_b

    .one_byte_disp:
        call op_from_imm8
        jmp .end_pars_addr_b

    .two_byte_disp:
        call op_from_imm16

    .end_pars_addr_b:
        mov byte ptr [di], '$'
        pop di
        ret
parse_addr_byte endp

; FORMATTING OPERANDS
form_operand_strings proc
    push si
    push cx

    cmp cc_type, t_2byte
    jne .skip_t_2byte
    inc cmd_len
    .skip_t_2byte:

    mov di, offset op_1
    mov al, cc_op_1
    mov tmp_op, al
    call form_op

    mov di, offset op_2
    mov al, cc_op_2
    mov tmp_op, al
    call form_op

    .end_forming_ops:
        pop cx
        pop si
        ret
form_operand_strings endp

form_op proc
    ; op_none
    cmp tmp_op, op_none
    jne .skip_op_none
    jmp .end_forming_op
    .skip_op_none:
    ; predefined op
    cmp tmp_op, 30
    jnb .skip_op_predef
    call op_predef
    jmp .end_forming_op
    .skip_op_predef:
    ; op requires address byte
    cmp tmp_op, 40
    jnb .skip_op_addr_byte
    call op_addr_byte
    jmp .end_forming_op
    .skip_op_addr_byte:
    ; no address byte required
    cmp tmp_op, 50
    jnb .end_forming_op
    call op_no_addr_byte
    .end_forming_op:
        mov byte ptr [di], '$'
        ret
form_op endp

op_predef proc
    mov ax, 3
    mov bl, tmp_op
    mul bl

    mov si, offset registers
    add si, ax

    call copy_str_until_ds

    ret
op_predef endp

op_addr_byte proc
    cmp addr_b_parsed_f, 1
    je .skip_addr_b_parse
    call parse_addr_byte
    .skip_addr_b_parse:

    ; Update imm_offset in case op_2 is of type imm
    inc imm_offset
    xor ah, ah
    mov al, cmd_disp_len
    add imm_offset, ax

    cmp tmp_op, op_reg8
    je .reg8
    cmp tmp_op, op_reg16
    je .reg16
    cmp tmp_op, op_regmem8
    je .regmem8
    cmp tmp_op, op_regmem16
    je .regmem16
    cmp tmp_op, op_sreg
    je .sreg
    ret

    .reg8:
        call op_from_reg8
        ret
    .reg16:
        call op_from_reg16
        ret
    .regmem8:
        call op_from_regmem8
        ret
    .regmem16:
        call op_from_regmem16
        ret
    .sreg:
        call op_from_sreg
        ret
op_addr_byte endp

op_no_addr_byte proc
    mov si, main_loop_si
    add si, imm_offset

    cmp tmp_op, op_imm8
    je .imm8
    cmp tmp_op, op_imm16
    je .imm16
    cmp tmp_op, op_daddr
    je .daddr
    cmp tmp_op, op_faddr
    je .faddr
    cmp tmp_op, op_disp8
    je .disp8
    cmp tmp_op, op_disp16
    je .disp16
    ret

    .imm8:
        call op_from_imm8
        inc cmd_len
        ret
    .imm16:
        call op_from_imm16
        add cmd_len, 2
        ret
    .daddr:
        call op_from_daddress
        add cmd_len, 2
        ret
    .faddr:
        call op_from_faddress
        add cmd_len, 4
        ret
    .disp8:
        inc cmd_len
        call op_from_disp8
        ret
    .disp16:
        add cmd_len, 2
        call op_from_disp16
        ret
op_no_addr_byte endp

op_from_reg8 proc
    ; Uses 'registers' string to form op_reg8
    mov bl, reg
    mov tmp_op, bl
    call op_predef
    ret
op_from_reg8 endp

op_from_reg16 proc
    ; Uses 'registers' string to form op_reg16
    mov bl, reg
    add bl, 8
    mov tmp_op, bl
    call op_predef
    ret
op_from_reg16 endp

op_from_sreg proc
    ; Uses 'registers' string to form op_sreg
    xor ah, ah
    mov al, reg
    add ax, 16
    mov tmp_op, al
    call op_predef
    ret
op_from_sreg endp

op_from_regmem8 proc
    ; In case of register, refer to op_from_reg8
    cmp md, 11b
    jne .real_raddr8
    mov al, reg
    mov tmp_reg, al
    mov al, rm
    mov reg, al
    call op_from_reg8
    mov al, tmp_reg
    mov reg, al
    ret

    .real_raddr8:
    call op_from_mem

    ret
op_from_regmem8 endp

op_from_regmem16 proc
    ; In case of register, refer to op_from_reg16
    cmp md, 11b
    jne .real_raddr16
    mov al, reg
    mov tmp_reg, al
    mov al, rm
    mov reg, al
    call op_from_reg16
    mov al, tmp_reg
    mov reg, al
    ret

    .real_raddr16:
    call op_from_mem

    ret
op_from_regmem16 endp

op_from_mem proc
    mov byte ptr [di], '[' 
    inc di

    ; Calculate string offset for mem_struct03 or mem_struct47
    ; mem_struct03: si = rm * 8
    ; mem_struct47: si = rm * 3
    xor ah, ah
    mov al, rm
    cmp rm, 3
    jg .rm47
    .rm03:
        mov si, offset mem_struct03
        mov bx, 8
        mul bx
        add si, ax
        jmp .write_pre_disp
    .rm47:
        cmp rm, 6   ; direct address when r/m = 110 and mod = 00
        jne .skip_rm6_md0
        cmp md, 0
        je .daddress_md0_rm6
        .skip_rm6_md0:
        mov si, offset mem_struct47
        mov bx, 3
        sub ax, 4
        mul bx
        add si, ax
    
    .write_pre_disp:
        call copy_str_until_ds

    .daddress_md0_rm6:
    mov si, offset cmd_disp_hex
    cmp byte ptr [si], '$'
    je .skip_disp

    cmp md, 0
    je .no_plus
    mov byte ptr [di], ' '
    mov byte ptr [di + 1], '+'
    mov byte ptr [di + 2], ' '
    add di, 3

    .no_plus:
    .write_disp:
        call copy_str_until_ds

    .skip_disp:
    mov byte ptr [di], ']'
    inc di

    ret
op_from_mem endp

op_from_imm8 proc
    mov bl, [si]
    call byte_to_hex
    mov [di], bh
    mov [di + 1], bl
    mov byte ptr [di + 2], 'h'
    add di, 3
    ret
op_from_imm8 endp

op_from_imm16 proc
    mov bl, [si + 1]
    call byte_to_hex
    mov [di], bh
    mov [di + 1], bl

    mov bl, [si]
    call byte_to_hex
    mov [di + 2], bh
    mov [di + 3], bl
    mov byte ptr [di + 4], 'h'
    add di, 5
    ret
op_from_imm16 endp

op_from_daddress proc
    mov byte ptr [di], '['
    inc di

    call op_from_imm16

    mov byte ptr [di], ']'
    inc di
    ret
op_from_daddress endp

op_from_faddress proc
    mov byte ptr [di], '['
    inc di

    add si, 2
    call op_from_imm16
    mov byte ptr [di], ':'
    inc di

    sub si, 2
    call op_from_imm16
    mov byte ptr [di], ']'
    inc di
    ret
op_from_faddress endp

op_from_disp8 proc
    mov dx, cmd_address
    mov disp_address, dx

    xor dh, dh
    mov dl, [si]
    mov al, [si]
    and al, 80h
    cmp al, 80h
    jne .add_disp8

    .sub_disp8:
    xor dl, 0FFh
    dec dl
    sub disp_address, dx
    jmp .end_disp8

    .add_disp8:
    add disp_address, dx
    mov dl, cmd_len
    add disp_address, dx

    .end_disp8:
    mov si, offset disp_address
    mov byte ptr [di], '['
    inc di
    call op_from_imm16
    mov byte ptr [di], ']'
    inc di

    ret
op_from_disp8 endp

op_from_disp16 proc
    mov dx, cmd_address
    mov disp_address, dx

    mov dx, [si]
    mov ax, [si]
    and ax, 8000h
    cmp ax, 8000h
    jne .add_disp16

    .sub_disp16:
    xor dx, 0FFFFh
    dec dx
    sub disp_address, dx
    jmp .end_disp16

    .add_disp16:
    add disp_address, dx
    xor dh, dh
    mov dl, cmd_len
    add disp_address, dx

    .end_disp16:
    mov si, offset disp_address
    mov byte ptr [di], '['
    inc di
    call op_from_imm16
    mov byte ptr [di], ']'
    inc di

    ret
op_from_disp16 endp

; FORMATTING FINAL OUTPUT
format_cmd_address proc
    ; Format command address
    mov si, offset cmd_address
    call op_from_imm16
    ; Format padding
    mov cx, cmd_address_padding
    sub cx, 5
    call print_padding
    ret
format_cmd_address endp

format_cmd_hex proc
    mov si, main_loop_si
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
    call print_padding
    .skip_cmd_hex_padding:
    ret
format_cmd_hex endp

format_cmd_name proc
    ; Format command name
    mov si, cmd_name
    mov bx, di
    call copy_str_until_ds
    mov cmd_name_len, di
    sub cmd_name_len, bx

    cmp byte ptr [op_1], '$'
    je .skip_cmd_name_padding
    ; Format padding
    mov cx, cmd_name_padding
    sub cx, cmd_name_len
    call print_padding

    .skip_cmd_name_padding:
    ret
format_cmd_name endp

format_op_1 proc
    ; Format first operand
    cmp byte ptr [op_1], '['
    jne .skip_op1_seg_override

    mov si, seg_ov
    call copy_str_until_ds

    .skip_op1_seg_override:
    mov si, offset op_1
    call copy_str_until_ds

    ret 
format_op_1 endp

format_op_2 proc
    mov byte ptr [di], ','
    mov byte ptr [di + 1], ' '
    add di, 2
    ; Format second operand
    cmp byte ptr [op_2], '['
    jne .skip_op2_seg_override

    mov si, seg_ov
    call copy_str_until_ds

    .skip_op2_seg_override:

    mov si, offset op_2
    call copy_str_until_ds

    ret
format_op_2 endp

format_output_line proc
    push cx
    push si
    mov di, offset output_line

    call format_cmd_address
    call format_cmd_hex
    call format_cmd_name
    cmp byte ptr [op_1], '$'
    je .end_format
    call format_op_1
    cmp byte ptr [op_2], '$'
    je .end_format
    call format_op_2

    .end_format:
    mov byte ptr [di], 0ah
    inc di

    sub di, offset output_line
    mov bx, di
    mov output_line_len, bx

    pop si
    pop cx
    ret
format_output_line endp

; HELPERS
write_line_to_file proc
    push cx
    mov cx, output_line_len
    mov ax, 4000h
    mov bx, fd_out
    mov dx, offset output_line
    int 21h
    jc .file_err_write

    pop cx
    ret

    .file_err_write:
        call print_file_err
write_line_to_file endp

update_buffer proc
    ; Copy last bytes to start
    ; Set new SI = offset buff
    ;         CX = bytes read + bytes_rem
    mov bytes_rem, cx
    mov di, offset buff
    .rewrite_to_start:
        mov al, [si]
        mov [di], al
        inc di
        inc si
    loop .rewrite_to_start

    mov si, offset buff

    mov cx, buff_size
    sub cx, bytes_rem
    mov dx, offset buff
    add dx, bytes_rem
    mov ax, 3f00h
    mov bx, fd_in
    int 21h
    jc .file_err_updating_buffer

    mov cx, ax
    cmp cx, 0
    jne .not_eof
    mov eof, 1
    .not_eof:
    add cx, bytes_rem

    ret

    .file_err_updating_buffer:
        call print_file_err
update_buffer endp

get_2_args proc
    xor cx, cx
    mov cl, es:[80h]

    ; read first arg
    xor si, si
    mov di, offset fn_in
    call get_arg
    cmp bx, 0
    je .print_invalid_input
    cmp bx, 12
    jg .print_invalid_input
    ; check for /?
    cmp bx, 2
    jne .continue_to_sec_arg
    cmp [fn_in], '/'
    jne .continue_to_sec_arg
    cmp [fn_in + 1], '?'
    je .print_usage

    .continue_to_sec_arg:
    ; read second arg
    mov di, offset fn_out
    call get_arg
    cmp bx, 0
    je .print_invalid_input
    cmp bx, 12
    jg .print_invalid_input

    ret

    .print_invalid_input:
        print_string err_msg
        print_string newl
        print_string usage_msg
        terminate

    .print_usage:
        print_string usage_msg
        terminate
get_2_args endp

get_arg proc
    ; Input:
    ; si - offset from start of cli arguments string
    ; cx - total length or arguments string at 80h
    ; di - destination address for argument string
    ; Output:
    ; bx - argument's length
    xor bx, bx

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
        cmp al, 0Dh
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
    mov md, 0
    mov reg, 0
    mov rm, 0

    mov cc_addr, 0
    mov cc_type, 0
    mov cc_op_1, 0
    mov cc_op_2, 0

    mov addr_b_parsed_f, 0
    mov imm_offset, 1

    mov cmd_disp_len, 0
    mov byte ptr [op_1], '$'
    mov byte ptr [op_2], '$'
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
    and bl, 1111b
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

check_seg_ov proc
    ; Store offset in seg_ov for seg_ov_string
    mov seg_ov, offset seg_ov_string

    mov dh, opc_byte
    cmp dh, 26h
    je .seg_ov26h
    cmp dh, 2Eh
    je .seg_ov2Eh
    cmp dh, 36h
    je .seg_ov36h
    cmp dh, 3Eh
    je .seg_ov3Eh
    add seg_ov, 16
    ret

    .seg_ov26h:
        add seg_ov, 0
        jmp .end_seg_ov
    .seg_ov2Eh:
        add seg_ov, 4
        jmp .end_seg_ov
    .seg_ov36h:
        add seg_ov, 8
        jmp .end_seg_ov
    .seg_ov3Eh:
        add seg_ov, 12
        
    .end_seg_ov:
        inc cmd_len
        mov al, addr_byte
        mov opc_byte, al
        mov al, [si + 2]
        mov addr_byte, al
        inc main_loop_si
        ret
check_seg_ov endp

copy_str_until_ds proc
    cmp byte ptr [si], '$'
    je .end_copy
    .string_copy:
        mov al, [si]
        mov [di], al
        inc di
        inc si
        cmp byte ptr [si], '$'
        jne .string_copy

    .end_copy:
    ret
copy_str_until_ds endp

print_padding proc
    .loop_pp:
        mov byte ptr [di], ' '
        inc di
    loop .loop_pp
    ret
print_padding endp

implement_test_f6_f7 proc
    mov dh, addr_byte
    shr dh, 3
    and dh, 7
    cmp dh, 0
    jne .skip_impl
    cmp opc_byte, 0F6h
    jne .f7h
    mov cc_op_2, op_imm8
    ret

    .f7h:
        mov cc_op_2, op_imm16
    .skip_impl:
        ret
implement_test_f6_f7 endp

open_files proc
    ; open input file
    mov ax, 3d00h
    mov dx, offset fn_in
    int 21h
    mov fd_in, ax
    jc .file_err_open
    
    ; create output file
    mov ax, 3c00h
    xor cx, cx
    mov dx, offset fn_out
    int 21h
    mov fd_out, ax
    jc .file_err_open

    ret

    .file_err_open:
        call print_file_err
open_files endp

close_files proc
    ; close both files
    mov ax, 3e00h
    mov bx, fd_out
    int 21h
    jc .file_err_close
    mov ax, 3e00h
    mov bx, fd_in
    int 21h
    jc .file_err_close

    ret

    .file_err_close:
        call print_file_err
close_files endp

initial_read proc
    ; read first chunk of bytes of size 'buff_size'
    mov cx, buff_size
    mov dx, offset buff
    mov ax, 3f00h
    mov bx, fd_in
    int 21h
    jc .file_err_initial_read
    
    ret

    .file_err_initial_read:
        call print_file_err
initial_read endp

print_file_err proc
    print_string file_err_msg
    mov bl, al
    call byte_to_hex
    print_char bh
    print_char bl
    print_char 'h'

    terminate
print_file_err endp

check_for_exe proc
    ; Checks for .exe signature bytes 'MZ' and skips
    ; the amount of bytes that the header takes up
    ; by updating the file read pointer of fd_in
    call initial_read

    mov si, offset buff
    cmp word ptr [si], 5A4Dh ; MZ header signature
    jne .end_check_for_exe

    mov dx, [si + 08h]
    mov cx, dx
    shl dx, 4
    rcl cx, 4
    and cx, 1111b

    mov ax, 4200h
    mov bx, fd_in
    int 21h
    jc .file_err_check_exe
    mov cmd_address, 0
    ret

    .end_check_for_exe:
        mov ax, 4200h
        mov bx, fd_in
        xor cx, cx
        xor dx, dx
        int 21h
        jc .file_err_check_exe
        ret

    .file_err_check_exe:
        call print_file_err
check_for_exe endp

end start

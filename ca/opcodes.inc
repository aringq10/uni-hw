; argument types:
    
    op_none equ 0
; byte register
    op_al   equ 1
    op_cl   equ 2
    op_dl   equ 3
    op_bl   equ 4
    op_ah   equ 5
    op_ch   equ 6
    op_dh   equ 7
    op_bh   equ 8
; word register
    op_ax   equ 9
    op_cx   equ 10
    op_dx   equ 11
    op_bx   equ 12
    op_sp   equ 13
    op_bp   equ 14
    op_si   equ 15
    op_di   equ 16
; segment register
    op_es   equ 17
    op_cs   equ 18
    op_ss   equ 19
    op_ds   equ 20

; operands to be parsed
    op_reg8
    op_reg16
    op_regmem8
    op_regmem16
    op_imm8
    op_imm16
    op_imm8_ext
    op_daddr
    op_sreg
    op_port
    op_1


; command types
t_unk
t_normal - opc, mod reg r/m, disp
t_normal_sreg - opc, mod sreg r/m, disp
t_ext - opc, mod reg r/m, disp, imm
t_ext_opc - opc, mod opc r/m, disp, imm
t_imm - opc, imm
t_daddr - opc, direct address
t_byte - opc
t_port

op_struct<cn_unk,t_unk,op_none,op_none>

op_struct<cn_add,t_normal,op_regmem8,op_reg8>                 ; 00 ADD1
op_struct<cn_add,t_normal,op_regmem16,op_reg16>               ; 01
op_struct<cn_add,t_normal,op_reg8,op_regmem8>                 ; 02
op_struct<cn_add,t_normal,op_reg16,op_regmem16>               ; 03
op_struct<cn_add,t_imm,op_al,op_imm8>                         ; 04 ADD3
op_struct<cn_add,t_imm,op_ax,op_imm16>                        ; 05
op_struct<cn_push,t_byte,op_es,op_none>                       ; 06 PUSH3
op_struct<cn_pop,t_byte,op_es,op_none>                        ; 07 POP3
op_struct<cn_or,t_normal,op_regmem8,op_reg8>                  ; 08 OR1
op_struct<cn_or,t_normal,op_regmem16,op_reg16>                ; 09
op_struct<cn_or,t_normal,op_reg8,op_regmem8>                  ; 0A
op_struct<cn_or,t_normal,op_reg16,op_regmem16>                ; 0B
op_struct<cn_or,t_imm,op_al,op_imm8>                          ; 0C OR3
op_struct<cn_or,t_imm,op_ax,op_imm16>                         ; 0D
op_struct<cn_push,t_byte,op_cs,op_none>                       ; 0E PUSH3
op_struct<cn_pop,t_byte,op_cs,op_none>                        ; 0F POP3 

op_struct<cn_push,t_byte,op_ss,op_none>                       ; 16 PUSH3
op_struct<cn_pop,t_byte,op_ss,op_none>                        ; 17 POP3

op_struct<cn_push,t_byte,op_ds,op_none>                       ; 1E PUSH3
op_struct<cn_pop,t_byte,op_ds,op_none>                        ; 1F POP3

op_struct<cn_and,t_normal,op_regmem8,op_reg8>                 ; 20 AND1
op_struct<cn_and,t_normal,op_regmem16,op_reg16>               ; 21
op_struct<cn_and,t_normal,op_reg8,op_regmem8>                 ; 22
op_struct<cn_and,t_normal,op_reg16,op_regmem16>               ; 23
op_struct<cn_and,t_imm,op_al,op_imm8>                         ; 24 AND3
op_struct<cn_and,t_imm,op_ax,op_imm16>                        ; 25

op_struct<cn_sub,t_normal,op_regmem8,op_reg8>                 ; 28 SUB1
op_struct<cn_sub,t_normal,op_regmem16,op_reg16>               ; 29
op_struct<cn_sub,t_normal,op_reg8,op_regmem8>                 ; 2A
op_struct<cn_sub,t_normal,op_reg16,op_regmem16>               ; 2B
op_struct<cn_sub,t_imm,op_al,op_imm8>                         ; 2C SUB3
op_struct<cn_sub,t_imm,op_ax,op_imm16>                        ; 2D

op_struct<cn_xor,t_normal,op_regmem8,op_reg8>                 ; 30 XOR1
op_struct<cn_xor,t_normal,op_regmem16,op_reg16>               ; 31
op_struct<cn_xor,t_normal,op_reg8,op_regmem8>                 ; 32
op_struct<cn_xor,t_normal,op_reg16,op_regmem16>               ; 33
op_struct<cn_xor,t_imm,op_al,op_imm8>                         ; 34 XOR3
op_struct<cn_xor,t_imm,op_ax,op_imm16>                        ; 35

op_struct<cn_cmp,t_normal,op_regmem8,op_reg8>                 ; 38 CMP1
op_struct<cn_cmp,t_normal,op_regmem16,op_reg16>               ; 39
op_struct<cn_cmp,t_normal,op_reg8,op_regmem8>                 ; 3A
op_struct<cn_cmp,t_normal,op_reg16,op_regmem16>               ; 3B
op_struct<cn_cmp,t_imm,op_al,op_imm8>                         ; 3C CMP3
op_struct<cn_cmp,t_imm,op_ax,op_imm16>                        ; 3D

op_struct<cn_inc,t_byte,op_ax,op_none>                        ; 40 INC2
op_struct<cn_inc,t_byte,op_cx,op_none>                        ; 41
op_struct<cn_inc,t_byte,op_dx,op_none>                        ; 42
op_struct<cn_inc,t_byte,op_bx,op_none>                        ; 43
op_struct<cn_inc,t_byte,op_sp,op_none>                        ; 44
op_struct<cn_inc,t_byte,op_bp,op_none>                        ; 45
op_struct<cn_inc,t_byte,op_si,op_none>                        ; 46
op_struct<cn_inc,t_byte,op_di,op_none>                        ; 47
op_struct<cn_dec,t_byte,op_ax,op_none>                        ; 48 DEC2
op_struct<cn_dec,t_byte,op_cx,op_none>                        ; 49
op_struct<cn_dec,t_byte,op_dx,op_none>                        ; 4A
op_struct<cn_dec,t_byte,op_bx,op_none>                        ; 4B
op_struct<cn_dec,t_byte,op_sp,op_none>                        ; 4C
op_struct<cn_dec,t_byte,op_bp,op_none>                        ; 4D
op_struct<cn_dec,t_byte,op_si,op_none>                        ; 4E
op_struct<cn_dec,t_byte,op_di,op_none>                        ; 4F

op_struct<cn_push,t_byte,op_ax,op_none>                       ; 50 PUSH2
op_struct<cn_push,t_byte,op_cx,op_none>                       ; 51
op_struct<cn_push,t_byte,op_dx,op_none>                       ; 52
op_struct<cn_push,t_byte,op_bx,op_none>                       ; 53
op_struct<cn_push,t_byte,op_sp,op_none>                       ; 54
op_struct<cn_push,t_byte,op_bp,op_none>                       ; 55
op_struct<cn_push,t_byte,op_si,op_none>                       ; 56
op_struct<cn_push,t_byte,op_di,op_none>                       ; 57
op_struct<cn_pop,t_byte,op_ax,op_none>                        ; 58 POP2
op_struct<cn_pop,t_byte,op_cx,op_none>                        ; 59
op_struct<cn_pop,t_byte,op_dx,op_none>                        ; 5A
op_struct<cn_pop,t_byte,op_bx,op_none>                        ; 5B
op_struct<cn_pop,t_byte,op_sp,op_none>                        ; 5C
op_struct<cn_pop,t_byte,op_bp,op_none>                        ; 5D
op_struct<cn_pop,t_byte,op_si,op_none>                        ; 5E
op_struct<cn_pop,t_byte,op_di,op_none>                        ; 5F

op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>         ; 80 ext_opc_80_83
op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm16>       ; 81
op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>         ; 82
op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm8_ext>    ; 83

op_struct<cn_xchg,t_normal,op_regmem8,op_reg8>                ; 86 XCHG1
op_struct<cn_xchg,t_normal,op_regmem16,op_reg16>              ; 87
op_struct<cn_mov,t_normal,op_regmem8,op_reg8>                 ; 88 MOV1
op_struct<cn_mov,t_normal,op_regmem16,op_reg16>               ; 89
op_struct<cn_mov,t_normal,op_reg8,op_regmem8>                 ; 8A
op_struct<cn_mov,t_normal,op_reg16,op_regmem16>               ; 8B
op_struct<cn_mov,t_normal_sreg,op_regmem16,op_sreg>           ; 8C MOV6
op_struct<cn_lea,t_normal,op_reg16,op_regmem16>               ; 8D LEA
op_struct<cn_mov,t_normal_sreg,op_sreg,op_regmem16>           ; 8E MOV6
op_struct<cn_pop,t_normal,op_regmem16,op_none>                ; 8F POP1

op_struct<cn_xchg,t_byte,op_ax,op_ax>                         ; 90 XCHG2
op_struct<cn_xchg,t_byte,op_cx,op_ax>                         ; 91
op_struct<cn_xchg,t_byte,op_dx,op_ax>                         ; 92
op_struct<cn_xchg,t_byte,op_bx,op_ax>                         ; 93
op_struct<cn_xchg,t_byte,op_sp,op_ax>                         ; 94
op_struct<cn_xchg,t_byte,op_bp,op_ax>                         ; 95
op_struct<cn_xchg,t_byte,op_si,op_ax>                         ; 96
op_struct<cn_xchg,t_byte,op_di,op_ax>                         ; 97

op_struct<cn_mov,t_addr,op_al,op_daddr>                       ; A0 MOV4
op_struct<cn_mov,t_addr,op_ax,op_daddr>                       ; A1
op_struct<cn_mov,t_addr,op_daddr,op_al>                       ; A2 MOV5
op_struct<cn_mov,t_addr,op_daddr,op_ax>                       ; A3

op_struct<cn_mov,t_imm,op_al,op_imm8>                         ; B0 MOV3
op_struct<cn_mov,t_imm,op_cl,op_imm8>                         ; B1
op_struct<cn_mov,t_imm,op_dl,op_imm8>                         ; B2
op_struct<cn_mov,t_imm,op_bl,op_imm8>                         ; B3
op_struct<cn_mov,t_imm,op_ah,op_imm8>                         ; B4
op_struct<cn_mov,t_imm,op_ch,op_imm8>                         ; B5
op_struct<cn_mov,t_imm,op_dh,op_imm8>                         ; B6
op_struct<cn_mov,t_imm,op_bh,op_imm8>                         ; B7
op_struct<cn_mov,t_imm,op_ax,op_imm16>                        ; B8
op_struct<cn_mov,t_imm,op_cx,op_imm16>                        ; B9
op_struct<cn_mov,t_imm,op_dx,op_imm16>                        ; BA
op_struct<cn_mov,t_imm,op_bx,op_imm16>                        ; BB
op_struct<cn_mov,t_imm,op_sp,op_imm16>                        ; BC
op_struct<cn_mov,t_imm,op_bp,op_imm16>                        ; BD
op_struct<cn_mov,t_imm,op_si,op_imm16>                        ; BE
op_struct<cn_mov,t_imm,op_di,op_imm16>                        ; BF

op_struct<cn_mov,t_ext,op_imm8,op_regmem8>                    ; C6 MOV2
op_struct<cn_mov,t_ext,op_imm16,op_regmem8>                   ; C7

op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_1>            ; D0 ext_opc_d0_d3
op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_1>           ; D1
op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_cl>           ; D2
op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_cl>          ; D3

op_struct<cn_xlat,t_byte,op_none,op_none>                     ; D7 XLAT

op_struct<cn_in,t_port,op_al,op_port>                         ; E4 IN1
op_struct<cn_in,t_port,op_ax,op_port>                         ; E5
op_struct<cn_out,t_port,op_port,op_al>                        ; E6 OUT1
op_struct<cn_out,t_port,op_port,op_ax>                        ; E7

op_struct<cn_in,t_byte,op_al,op_dx>                           ; EC IN2
op_struct<cn_in,t_byte,op_ax,op_dx>                           ; ED
op_struct<cn_out,t_byte,op_dx,op_al>                          ; EE OUT2
op_struct<cn_out,t_byte,op_dx,op_ax>                          ; EF

op_struct<cn_not,t_normal,op_regmem8,op_none>                 ; F6 NOT
op_struct<cn_not,t_normal,op_regmem16,op_none>                ; F7

op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem8,op_none>         ; FE ext_opc_fe_ff
op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem16,op_none>        ; FF

op_struct<cn_lds,t_normal,op_reg16


; Extended opcode segments
80_83
D0_D3
F6_F7
FE_FF

ext_opc_80_83
    ADD2 000
    OR2  001
    -
    -
    AND2 100
    SUB2 101
    XOR2 110
    CMP2 111

ext_opc_d0_d3
    -
    -
    -
    -
    SHL/SAL 100
    SHR     101
    -
    SAR     111

ext_opc_fe_ff
    INC1 000
    DEC1 001
    -
    -
    -
    -
    PUSH1 110
    -

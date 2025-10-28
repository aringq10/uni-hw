; TODO commands:
;   REP
;   REPNZ/REPNE
;   MOVS
;   CMPS
;   SCAS
;   LODS
;   STOS
;   CALL3
;   JMP4
;   ESC

; OPERAND TYPES
; byte register
    op_al       equ 0
    op_cl       equ 1
    op_dl       equ 2
    op_bl       equ 3
    op_ah       equ 4
    op_ch       equ 5
    op_dh       equ 6
    op_bh       equ 7
; word register
    op_ax       equ 8
    op_cx       equ 9
    op_dx       equ 10
    op_bx       equ 11
    op_sp       equ 12
    op_bp       equ 13
    op_si       equ 14
    op_di       equ 15
; segment register
    op_es       equ 16
    op_cs       equ 17
    op_ss       equ 18
    op_ds       equ 19

    op_1_const  equ 20
    op_none     equ 21

; other
    ; 22-26 need address byte
    op_reg8     equ 22
    op_reg16    equ 23
    op_regmem8  equ 24
    op_regmem16 equ 25
    op_sreg     equ 26
    ; 27-30 no address byte needed
    op_imm8     equ 27
    op_imm16    equ 28
    op_daddr    equ 29
    op_disp8    equ 30
    op_disp16   equ 31

; COMMAND TYPES
    t_unk       equ 32
    t_unsupp    equ 33
    t_normal    equ 34
    t_ext_opc   equ 35
    t_2byte     equ 36

; COMMAND NAMES
    cn_unk    db "UNK$"
    cn_unsupp db "UNSUPPORTED$"
    cn_aaa    db "AAA$"
    cn_aad    db "AAD$"
    cn_aam    db "AAM$"
    cn_aas    db "AAS$"
    cn_adc    db "ADC$"
    cn_add    db "ADD$"
    cn_and    db "AND$"
    cn_call   db "CALL$"
    cn_callf  db "CALLF$"
    cn_cbw    db "CBW$"
    cn_clc    db "CLC$"
    cn_cld    db "CLD$"
    cn_cli    db "CLI$"
    cn_cmc    db "CMC$"
    cn_cmp    db "CMP$"
    cn_cmpsb  db "CMPSB$"
    cn_cmpsw  db "CMPSW$"
    cn_cwd    db "CWD$"
    cn_daa    db "DAA$"
    cn_das    db "DAS$"
    cn_dec    db "DEC$"
    cn_div    db "DIV$"
    cn_hlt    db "HLT$"
    cn_idiv   db "IDIV$"
    cn_imul   db "IMUL$"
    cn_in     db "IN$"
    cn_inc    db "INC$"
    cn_int    db "INT$"
    cn_into   db "INTO$"
    cn_iret   db "IRET$"
    cn_ja     db "JA$"
    cn_jae    db "JAE$"
    cn_jb     db "JB$"
    cn_jbe    db "JBE$"
    cn_jcxz   db "JCXZ$"
    cn_jg     db "JG$"
    cn_jge    db "JGE$"
    cn_jl     db "JL$"
    cn_jle    db "JLE$"
    cn_jmp    db "JMP$"
    cn_jmpf   db "JMPF$"
    cn_jno    db "JNO$"
    cn_jns    db "JNS$"
    cn_jnz    db "JNZ$"
    cn_jo     db "JO$"
    cn_jpe    db "JPE$"
    cn_jpo    db "JPO$"
    cn_js     db "JS$"
    cn_jz     db "JZ$"
    cn_lahf   db "LAHF$"
    cn_lds    db "LDS$"
    cn_lea    db "LEA$"
    cn_les    db "LES$"
    cn_lock   db "LOCK$"
    cn_lodsb  db "LODSB$"
    cn_lodsw  db "LODSW$"
    cn_loop   db "LOOP$"
    cn_loopnz db "LOOPNZ$"
    cn_loopz  db "LOOPZ$"
    cn_mov    db "MOV$"
    cn_movsb  db "MOVSB$"
    cn_movsw  db "MOVSW$"
    cn_mul    db "MUL$"
    cn_neg    db "NEG$"
    cn_nop    db "NOP$"
    cn_not    db "NOT$"
    cn_or     db "OR$"
    cn_out    db "OUT$"
    cn_pop    db "POP$"
    cn_popf   db "POPF$"
    cn_push   db "PUSH$"
    cn_pushf  db "PUSHF$"
    cn_rcl    db "RCL$"
    cn_rcr    db "RCR$"
    cn_rep    db "REP$"
    cn_repne  db "REPNE$"
    cn_ret    db "RET$"
    cn_retf   db "RETF$"
    cn_retn   db "RETN$"
    cn_repz   db "REPZ$"
    cn_rol    db "ROL$"
    cn_ror    db "ROR$"
    cn_sahf   db "SAHF$"
    cn_sal    db "SAL$"
    cn_sar    db "SAR$"
    cn_sbb    db "SBB$"
    cn_scasb  db "SCASB$"
    cn_scasw  db "SCASW$"
    cn_segcs  db "SEGCS$"
    cn_segds  db "SEGDS$"
    cn_seges  db "SEGES$"
    cn_segss  db "SEGSS$"
    cn_shl    db "SHL$"
    cn_shr    db "SHR$"
    cn_stc    db "STC$"
    cn_std    db "STD$"
    cn_sti    db "STI$"
    cn_stos   db "STOS$" 
    cn_stosb  db "STOSB$"
    cn_stosw  db "STOSW$"
    cn_sub    db "SUB$"
    cn_test   db "TEST$"
    cn_wait   db "WAIT$"
    cn_xchg   db "XCHG$"
    cn_xlat   db "XLAT$"
    cn_xor    db "XOR$"



; OPCODES 00-FF
label opcodes
    op_struct<cn_add,t_normal,op_regmem8,op_reg8>                   ; 00
    op_struct<cn_add,t_normal,op_regmem16,op_reg16>                 ; 01
    op_struct<cn_add,t_normal,op_reg8,op_regmem8>                   ; 02
    op_struct<cn_add,t_normal,op_reg16,op_regmem16>                 ; 03
    op_struct<cn_add,t_normal,op_al,op_imm8>                        ; 04
    op_struct<cn_add,t_normal,op_ax,op_imm16>                       ; 05
    op_struct<cn_push,t_normal,op_es,op_none>                       ; 06
    op_struct<cn_pop,t_normal,op_es,op_none>                        ; 07
    op_struct<cn_or,t_normal,op_regmem8,op_reg8>                    ; 08
    op_struct<cn_or,t_normal,op_regmem16,op_reg16>                  ; 09
    op_struct<cn_or,t_normal,op_reg8,op_regmem8>                    ; 0A
    op_struct<cn_or,t_normal,op_reg16,op_regmem16>                  ; 0B
    op_struct<cn_or,t_normal,op_al,op_imm8>                         ; 0C
    op_struct<cn_or,t_normal,op_ax,op_imm16>                        ; 0D
    op_struct<cn_push,t_normal,op_cs,op_none>                       ; 0E
    op_struct<cn_pop,t_normal,op_cs,op_none>                        ; 0F

    op_struct<cn_adc,t_normal,op_regmem8,op_reg8>                   ; 10
    op_struct<cn_adc,t_normal,op_regmem16,op_reg16>                 ; 11
    op_struct<cn_adc,t_normal,op_reg8,op_regmem8>                   ; 12
    op_struct<cn_adc,t_normal,op_reg16,op_regmem16>                 ; 13
    op_struct<cn_adc,t_normal,op_al,op_imm8>                        ; 14
    op_struct<cn_adc,t_normal,op_ax,op_imm16>                       ; 15
    op_struct<cn_push,t_normal,op_ss,op_none>                       ; 16
    op_struct<cn_pop,t_normal,op_ss,op_none>                        ; 17
    op_struct<cn_sbb,t_normal,op_regmem8,op_reg8>                   ; 18
    op_struct<cn_sbb,t_normal,op_regmem16,op_reg16>                 ; 19
    op_struct<cn_sbb,t_normal,op_reg8,op_regmem8>                   ; 1A
    op_struct<cn_sbb,t_normal,op_reg16,op_regmem16>                 ; 1B
    op_struct<cn_sbb,t_normal,op_al,op_imm8>                        ; 1C
    op_struct<cn_sbb,t_normal,op_ax,op_imm16>                       ; 1D
    op_struct<cn_push,t_normal,op_ds,op_none>                       ; 1E
    op_struct<cn_pop,t_normal,op_ds,op_none>                        ; 1F

    op_struct<cn_and,t_normal,op_regmem8,op_reg8>                   ; 20
    op_struct<cn_and,t_normal,op_regmem16,op_reg16>                 ; 21
    op_struct<cn_and,t_normal,op_reg8,op_regmem8>                   ; 22
    op_struct<cn_and,t_normal,op_reg16,op_regmem16>                 ; 23
    op_struct<cn_and,t_normal,op_al,op_imm8>                        ; 24
    op_struct<cn_and,t_normal,op_ax,op_imm16>                       ; 25
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_daa,t_normal,op_none,op_none>                      ; 27
    op_struct<cn_sub,t_normal,op_regmem8,op_reg8>                   ; 28
    op_struct<cn_sub,t_normal,op_regmem16,op_reg16>                 ; 29
    op_struct<cn_sub,t_normal,op_reg8,op_regmem8>                   ; 2A
    op_struct<cn_sub,t_normal,op_reg16,op_regmem16>                 ; 2B
    op_struct<cn_sub,t_normal,op_al,op_imm8>                        ; 2C
    op_struct<cn_sub,t_normal,op_ax,op_imm16>                       ; 2D
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_das,t_normal,op_none,op_none>                      ; 2F

    op_struct<cn_xor,t_normal,op_regmem8,op_reg8>                   ; 30
    op_struct<cn_xor,t_normal,op_regmem16,op_reg16>                 ; 31
    op_struct<cn_xor,t_normal,op_reg8,op_regmem8>                   ; 32
    op_struct<cn_xor,t_normal,op_reg16,op_regmem16>                 ; 33
    op_struct<cn_xor,t_normal,op_al,op_imm8>                        ; 34
    op_struct<cn_xor,t_normal,op_ax,op_imm16>                       ; 35
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_aaa,t_normal,op_none,op_none>                      ; 37
    op_struct<cn_cmp,t_normal,op_regmem8,op_reg8>                   ; 38
    op_struct<cn_cmp,t_normal,op_regmem16,op_reg16>                 ; 39
    op_struct<cn_cmp,t_normal,op_reg8,op_regmem8>                   ; 3A
    op_struct<cn_cmp,t_normal,op_reg16,op_regmem16>                 ; 3B
    op_struct<cn_cmp,t_normal,op_al,op_imm8>                        ; 3C
    op_struct<cn_cmp,t_normal,op_ax,op_imm16>                       ; 3D
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_aas,t_normal,op_none,op_none>                      ; 3F

    op_struct<cn_inc,t_normal,op_ax,op_none>                        ; 40
    op_struct<cn_inc,t_normal,op_cx,op_none>                        ; 41
    op_struct<cn_inc,t_normal,op_dx,op_none>                        ; 42
    op_struct<cn_inc,t_normal,op_bx,op_none>                        ; 43
    op_struct<cn_inc,t_normal,op_sp,op_none>                        ; 44
    op_struct<cn_inc,t_normal,op_bp,op_none>                        ; 45
    op_struct<cn_inc,t_normal,op_si,op_none>                        ; 46
    op_struct<cn_inc,t_normal,op_di,op_none>                        ; 47
    op_struct<cn_dec,t_normal,op_ax,op_none>                        ; 48
    op_struct<cn_dec,t_normal,op_cx,op_none>                        ; 49
    op_struct<cn_dec,t_normal,op_dx,op_none>                        ; 4A
    op_struct<cn_dec,t_normal,op_bx,op_none>                        ; 4B
    op_struct<cn_dec,t_normal,op_sp,op_none>                        ; 4C
    op_struct<cn_dec,t_normal,op_bp,op_none>                        ; 4D
    op_struct<cn_dec,t_normal,op_si,op_none>                        ; 4E
    op_struct<cn_dec,t_normal,op_di,op_none>                        ; 4F

    op_struct<cn_push,t_normal,op_ax,op_none>                       ; 50
    op_struct<cn_push,t_normal,op_cx,op_none>                       ; 51
    op_struct<cn_push,t_normal,op_dx,op_none>                       ; 52
    op_struct<cn_push,t_normal,op_bx,op_none>                       ; 53
    op_struct<cn_push,t_normal,op_sp,op_none>                       ; 54
    op_struct<cn_push,t_normal,op_bp,op_none>                       ; 55
    op_struct<cn_push,t_normal,op_si,op_none>                       ; 56
    op_struct<cn_push,t_normal,op_di,op_none>                       ; 57
    op_struct<cn_pop,t_normal,op_ax,op_none>                        ; 58
    op_struct<cn_pop,t_normal,op_cx,op_none>                        ; 59
    op_struct<cn_pop,t_normal,op_dx,op_none>                        ; 5A
    op_struct<cn_pop,t_normal,op_bx,op_none>                        ; 5B
    op_struct<cn_pop,t_normal,op_sp,op_none>                        ; 5C
    op_struct<cn_pop,t_normal,op_bp,op_none>                        ; 5D
    op_struct<cn_pop,t_normal,op_si,op_none>                        ; 5E
    op_struct<cn_pop,t_normal,op_di,op_none>                        ; 5F

    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 60
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 61
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 62
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 63
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 64
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 65
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 66
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 67
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 68
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 69
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6A
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6B
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6C
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6D
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6E
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 6F

    op_struct<cn_jo,t_normal,op_disp8,op_none>                      ; 70
    op_struct<cn_jno,t_normal,op_disp8,op_none>                     ; 71
    op_struct<cn_jb,t_normal,op_disp8,op_none>                      ; 72
    op_struct<cn_jae,t_normal,op_disp8,op_none>                     ; 73
    op_struct<cn_jz,t_normal,op_disp8,op_none>                      ; 74
    op_struct<cn_jnz,t_normal,op_disp8,op_none>                     ; 75
    op_struct<cn_jbe,t_normal,op_disp8,op_none>                     ; 76
    op_struct<cn_ja,t_normal,op_disp8,op_none>                      ; 77
    op_struct<cn_js,t_normal,op_disp8,op_none>                      ; 78
    op_struct<cn_jns,t_normal,op_disp8,op_none>                     ; 79
    op_struct<cn_jpe,t_normal,op_disp8,op_none>                     ; 7A
    op_struct<cn_jpo,t_normal,op_disp8,op_none>                     ; 7B
    op_struct<cn_jl,t_normal,op_disp8,op_none>                      ; 7C
    op_struct<cn_jge,t_normal,op_disp8,op_none>                     ; 7D
    op_struct<cn_jle,t_normal,op_disp8,op_none>                     ; 7E
    op_struct<cn_jg,t_normal,op_disp8,op_none>                      ; 7F

    op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>           ; 80 ext_opc_80_83
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm16>         ; 81
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>           ; 82
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm8>          ; 83
    op_struct<cn_test,t_normal,op_regmem8,op_reg8>                  ; 84
    op_struct<cn_test,t_normal,op_regmem16,op_reg16>                ; 85
    op_struct<cn_xchg,t_normal,op_regmem8,op_reg8>                  ; 86
    op_struct<cn_xchg,t_normal,op_regmem16,op_reg16>                ; 87
    op_struct<cn_mov,t_normal,op_regmem8,op_reg8>                   ; 88
    op_struct<cn_mov,t_normal,op_regmem16,op_reg16>                 ; 89
    op_struct<cn_mov,t_normal,op_reg8,op_regmem8>                   ; 8A
    op_struct<cn_mov,t_normal,op_reg16,op_regmem16>                 ; 8B
    op_struct<cn_mov,t_normal,op_regmem16,op_sreg>                  ; 8C
    op_struct<cn_lea,t_normal,op_reg16,op_regmem16>                 ; 8D
    op_struct<cn_mov,t_normal,op_sreg,op_regmem16>                  ; 8E
    op_struct<cn_pop,t_normal,op_regmem16,op_none>                  ; 8F

    op_struct<cn_xchg,t_normal,op_ax,op_ax>                         ; 90
    op_struct<cn_xchg,t_normal,op_cx,op_ax>                         ; 91
    op_struct<cn_xchg,t_normal,op_dx,op_ax>                         ; 92
    op_struct<cn_xchg,t_normal,op_bx,op_ax>                         ; 93
    op_struct<cn_xchg,t_normal,op_sp,op_ax>                         ; 94
    op_struct<cn_xchg,t_normal,op_bp,op_ax>                         ; 95
    op_struct<cn_xchg,t_normal,op_si,op_ax>                         ; 96
    op_struct<cn_xchg,t_normal,op_di,op_ax>                         ; 97
    op_struct<cn_cbw,t_normal,op_none,op_none>                      ; 98
    op_struct<cn_cwd,t_normal,op_none,op_none>                      ; 99
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_wait,t_normal,op_none,op_none>                     ; 9B
    op_struct<cn_pushf,t_normal,op_none,op_none>                    ; 9C
    op_struct<cn_popf,t_normal,op_none,op_none>                     ; 9D
    op_struct<cn_sahf,t_normal,op_none,op_none>                     ; 9E
    op_struct<cn_lahf,t_normal,op_none,op_none>                     ; 9F

    op_struct<cn_mov,t_normal,op_al,op_daddr>                       ; A0
    op_struct<cn_mov,t_normal,op_ax,op_daddr>                       ; A1
    op_struct<cn_mov,t_normal,op_daddr,op_al>                       ; A2
    op_struct<cn_mov,t_normal,op_daddr,op_ax>                       ; A3
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_test,t_normal,op_al,op_imm8>                       ; A8
    op_struct<cn_test,t_normal,op_ax,op_imm16>                      ; A9
    op_struct<cn_stos,t_unk,op_none,op_none>
    op_struct<cn_stos,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<cn_mov,t_normal,op_al,op_imm8>                        ; B0
    op_struct<cn_mov,t_normal,op_cl,op_imm8>                        ; B1
    op_struct<cn_mov,t_normal,op_dl,op_imm8>                        ; B2
    op_struct<cn_mov,t_normal,op_bl,op_imm8>                        ; B3
    op_struct<cn_mov,t_normal,op_ah,op_imm8>                        ; B4
    op_struct<cn_mov,t_normal,op_ch,op_imm8>                        ; B5
    op_struct<cn_mov,t_normal,op_dh,op_imm8>                        ; B6
    op_struct<cn_mov,t_normal,op_bh,op_imm8>                        ; B7
    op_struct<cn_mov,t_normal,op_ax,op_imm16>                       ; B8
    op_struct<cn_mov,t_normal,op_cx,op_imm16>                       ; B9
    op_struct<cn_mov,t_normal,op_dx,op_imm16>                       ; BA
    op_struct<cn_mov,t_normal,op_bx,op_imm16>                       ; BB
    op_struct<cn_mov,t_normal,op_sp,op_imm16>                       ; BC
    op_struct<cn_mov,t_normal,op_bp,op_imm16>                       ; BD
    op_struct<cn_mov,t_normal,op_si,op_imm16>                       ; BE
    op_struct<cn_mov,t_normal,op_di,op_imm16>                       ; BF

    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_ret,t_normal,op_imm16,op_none>                     ; C2
    op_struct<cn_ret,t_normal,op_none,op_none>                      ; C3
    op_struct<cn_les,t_normal,op_reg16,op_regmem16>                 ; C4
    op_struct<cn_lds,t_normal,op_reg16,op_regmem16>                 ; C5
    op_struct<cn_mov,t_normal,op_regmem8,op_imm8>                   ; C6
    op_struct<cn_mov,t_normal,op_regmem16,op_imm16>                 ; C7
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_ret,t_normal,op_imm16,op_none>                     ; CA
    op_struct<cn_ret,t_normal,op_none,op_none>                      ; CB
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_int,t_normal,op_imm8,op_none>                      ; CD
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_iret,t_normal,op_none,op_none>                     ; CF

    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_1_const>        ; D0 ext_opc_d0_d3
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_1_const>       ; D1
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_cl>             ; D2
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_cl>            ; D3
    op_struct<cn_aam,t_2byte,op_none,op_none>                       ; D4
    op_struct<cn_aad,t_2byte,op_none,op_none>                       ; D5
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_xlat,t_normal,op_none,op_none>                     ; D7
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<cn_loopnz,t_normal,op_disp8,op_none>                  ; E0
    op_struct<cn_loopz,t_normal,op_disp8,op_none>                   ; E1
    op_struct<cn_loop,t_normal,op_disp8,op_none>                    ; E2
    op_struct<cn_jcxz,t_normal,op_disp8,op_none>                    ; E3
    op_struct<cn_in,t_normal,op_al,op_imm8>                         ; E4
    op_struct<cn_in,t_normal,op_ax,op_imm8>                         ; E5
    op_struct<cn_out,t_normal,op_imm8,op_al>                        ; E6
    op_struct<cn_out,t_normal,op_imm8,op_ax>                        ; E7
    op_struct<cn_call,t_normal,op_daddr,op_none>                    ; E8
    op_struct<cn_jmp,t_normal,op_disp16,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_jmp,t_normal,op_disp8,op_none>                     ; EB
    op_struct<cn_in,t_normal,op_al,op_dx>                           ; EC
    op_struct<cn_in,t_normal,op_ax,op_dx>                           ; ED
    op_struct<cn_out,t_normal,op_dx,op_al>                          ; EE
    op_struct<cn_out,t_normal,op_dx,op_ax>                          ; EF

    op_struct<cn_lock,t_normal,op_none,op_none>                     ; F0
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_hlt,t_normal,op_none,op_none>                      ; F4
    op_struct<cn_cmc,t_normal,op_none,op_none>                      ; F5
    op_struct<ext_opc_f6_f7,t_ext_opc,op_regmem8,op_none>           ; F6 ext_opc_f6_f7
    op_struct<ext_opc_f6_f7,t_ext_opc,op_regmem16,op_none>          ; F7
    op_struct<cn_clc,t_normal,op_none,op_none>                      ; F8
    op_struct<cn_stc,t_normal,op_none,op_none>                      ; F9
    op_struct<cn_cli,t_normal,op_none,op_none>                      ; FA
    op_struct<cn_sti,t_normal,op_none,op_none>                      ; FB
    op_struct<cn_cld,t_normal,op_none,op_none>                      ; FC
    op_struct<cn_std,t_normal,op_none,op_none>                      ; FD
    op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem8,op_none>           ; FE ext_opc_fe_ff
    op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem16,op_none>          ; FF

; Extended opcode segments
label ext_opc_80_83
    dw cn_add     ; ADD2 000
    dw cn_or      ; OR2  001
    dw cn_adc     ; ADC2 010
    dw cn_sbb     ; SBB2 011
    dw cn_and     ; AND2 100
    dw cn_sub     ; SUB2 101
    dw cn_xor     ; XOR2 110
    dw cn_cmp     ; CMP2 111

label ext_opc_d0_d3
    dw cn_rol     ; ROL 000
    dw cn_ror     ; ROR 001
    dw cn_rcl     ; RCL 010
    dw cn_rcr     ; RCR 011
    dw cn_shl     ; SHL 100
    dw cn_shr     ; SHR 101
    dw cn_unsupp  ; - not used
    dw cn_sar     ; SAR 111


label ext_opc_f6_f7
    dw cn_test    ; TEST2 000 - implement separately (op_regmem, op_imm)
    dw cn_unsupp  ; - not used
    dw cn_not     ; NOT   010
    dw cn_neg     ; NEG   011
    dw cn_mul     ; MUL   100
    dw cn_imul    ; IMUL  101
    dw cn_div     ; DIV   110
    dw cn_idiv    ; IDIV  111

label ext_opc_fe_ff
    dw cn_inc     ; INC1  000
    dw cn_dec     ; DEC1  001
    dw cn_call    ; CALL2 010
    dw cn_call    ; CALL4 011
    dw cn_jmp     ; JMP3  100
    dw cn_jmp     ; JMP5  101
    dw cn_push    ; PUSH1 110
    dw cn_unsupp  ; - not used

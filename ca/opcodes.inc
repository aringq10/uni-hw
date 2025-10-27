; operand types:
    op_none     equ 0
; byte register
    op_al       equ 1
    op_cl       equ 2
    op_dl       equ 3
    op_bl       equ 4
    op_ah       equ 5
    op_ch       equ 6
    op_dh       equ 7
    op_bh       equ 8
; word register
    op_ax       equ 9
    op_cx       equ 10
    op_dx       equ 11
    op_bx       equ 12
    op_sp       equ 13
    op_bp       equ 14
    op_si       equ 15
    op_di       equ 16
; segment register
    op_es       equ 17
    op_cs       equ 18
    op_ss       equ 19
    op_ds       equ 20

; other
    op_reg8     equ 21
    op_reg16    equ 22
    op_regmem8  equ 23
    op_regmem16 equ 24
    op_imm8     equ 25
    op_imm16    equ 26
    op_imm8_ext equ 27
    op_daddr    equ 28
    op_sreg     equ 29
    op_port     equ 30
    op_1_const  equ 31


; command types
    t_unk       equ 32
    t_normal    equ 33          ; opc, mod reg r/m, disp
    t_normal_sreg equ 34        ; opc, mod sreg r/m, disp
    t_ext       equ 35          ; opc, mod reg r/m, disp, imm
    t_ext_opc   equ 36          ; opc, mod opc r/m, disp, imm
    t_imm       equ 37          ; opc, imm
    t_daddr     equ 38          ; opc, direct address
    t_2byte     equ 39
    t_unsupp    equ 40

; op_struct<cn_unk,t_unk,op_none,op_none>

; command names
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
    cn_shl_sal db "SHL/SAL$"
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
    op_struct<cn_add,t_normal,op_regmem8,op_reg8>                   ; 00 ADD1
    op_struct<cn_add,t_normal,op_regmem16,op_reg16>                 ; 01
    op_struct<cn_add,t_normal,op_reg8,op_regmem8>                   ; 02
    op_struct<cn_add,t_normal,op_reg16,op_regmem16>                 ; 03
    op_struct<cn_add,t_imm,op_al,op_imm8>                           ; 04 ADD3
    op_struct<cn_add,t_imm,op_ax,op_imm16>                          ; 05
    op_struct<cn_push,t_normal,op_es,op_none>                       ; 06 PUSH3
    op_struct<cn_pop,t_normal,op_es,op_none>                        ; 07 POP3
    op_struct<cn_or,t_normal,op_regmem8,op_reg8>                    ; 08 OR1
    op_struct<cn_or,t_normal,op_regmem16,op_reg16>                  ; 09
    op_struct<cn_or,t_normal,op_reg8,op_regmem8>                    ; 0A
    op_struct<cn_or,t_normal,op_reg16,op_regmem16>                  ; 0B
    op_struct<cn_or,t_imm,op_al,op_imm8>                            ; 0C OR3
    op_struct<cn_or,t_imm,op_ax,op_imm16>                           ; 0D
    op_struct<cn_push,t_normal,op_cs,op_none>                       ; 0E PUSH3
    op_struct<cn_pop,t_normal,op_cs,op_none>                        ; 0F POP3 

    op_struct<cn_adc,t_normal,op_regmem8,op_reg8>                   ; 10 ADC1
    op_struct<cn_adc,t_normal,op_regmem16,op_reg16>                 ; 11
    op_struct<cn_adc,t_normal,op_reg8,op_regmem8>                   ; 12
    op_struct<cn_adc,t_normal,op_reg16,op_regmem16>                 ; 13
    op_struct<cn_adc,t_imm,op_al,op_imm8>                           ; 14 ADC3
    op_struct<cn_adc,t_imm,op_ax,op_imm16>                          ; 15
    op_struct<cn_push,t_normal,op_ss,op_none>                       ; 16 PUSH3
    op_struct<cn_pop,t_normal,op_ss,op_none>                        ; 17 POP3
    op_struct<cn_sbb,t_normal,op_regmem8,op_reg8>                   ; 18 SBB1
    op_struct<cn_sbb,t_normal,op_regmem16,op_reg16>                 ; 19
    op_struct<cn_sbb,t_normal,op_reg8,op_regmem8>                   ; 1A
    op_struct<cn_sbb,t_normal,op_reg16,op_regmem16>                 ; 1B
    op_struct<cn_sbb,t_imm,op_al,op_imm8>                           ; 1C SBB3
    op_struct<cn_sbb,t_imm,op_ax,op_imm16>                          ; 1D
    op_struct<cn_push,t_normal,op_ds,op_none>                       ; 1E PUSH3
    op_struct<cn_pop,t_normal,op_ds,op_none>                        ; 1F POP3

    op_struct<cn_and,t_normal,op_regmem8,op_reg8>                   ; 20 AND1
    op_struct<cn_and,t_normal,op_regmem16,op_reg16>                 ; 21
    op_struct<cn_and,t_normal,op_reg8,op_regmem8>                   ; 22
    op_struct<cn_and,t_normal,op_reg16,op_regmem16>                 ; 23
    op_struct<cn_and,t_imm,op_al,op_imm8>                           ; 24 AND3
    op_struct<cn_and,t_imm,op_ax,op_imm16>                          ; 25
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_daa,t_normal,op_none,op_none>                      ; 27 DAA
    op_struct<cn_sub,t_normal,op_regmem8,op_reg8>                   ; 28 SUB1
    op_struct<cn_sub,t_normal,op_regmem16,op_reg16>                 ; 29
    op_struct<cn_sub,t_normal,op_reg8,op_regmem8>                   ; 2A
    op_struct<cn_sub,t_normal,op_reg16,op_regmem16>                 ; 2B
    op_struct<cn_sub,t_imm,op_al,op_imm8>                           ; 2C SUB3
    op_struct<cn_sub,t_imm,op_ax,op_imm16>                          ; 2D
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_das,t_normal,op_none,op_none>                      ; 2F DAS

    op_struct<cn_xor,t_normal,op_regmem8,op_reg8>                   ; 30 XOR1
    op_struct<cn_xor,t_normal,op_regmem16,op_reg16>                 ; 31
    op_struct<cn_xor,t_normal,op_reg8,op_regmem8>                   ; 32
    op_struct<cn_xor,t_normal,op_reg16,op_regmem16>                 ; 33
    op_struct<cn_xor,t_imm,op_al,op_imm8>                           ; 34 XOR3
    op_struct<cn_xor,t_imm,op_ax,op_imm16>                          ; 35
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_aaa,t_normal,op_none,op_none>                      ; 37 AAA
    op_struct<cn_cmp,t_normal,op_regmem8,op_reg8>                   ; 38 CMP1
    op_struct<cn_cmp,t_normal,op_regmem16,op_reg16>                 ; 39
    op_struct<cn_cmp,t_normal,op_reg8,op_regmem8>                   ; 3A
    op_struct<cn_cmp,t_normal,op_reg16,op_regmem16>                 ; 3B
    op_struct<cn_cmp,t_imm,op_al,op_imm8>                           ; 3C CMP3
    op_struct<cn_cmp,t_imm,op_ax,op_imm16>                          ; 3D
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_aas,t_normal,op_none,op_none>                      ; 3F AAS

    op_struct<cn_inc,t_normal,op_ax,op_none>                        ; 40 INC2
    op_struct<cn_inc,t_normal,op_cx,op_none>                        ; 41
    op_struct<cn_inc,t_normal,op_dx,op_none>                        ; 42
    op_struct<cn_inc,t_normal,op_bx,op_none>                        ; 43
    op_struct<cn_inc,t_normal,op_sp,op_none>                        ; 44
    op_struct<cn_inc,t_normal,op_bp,op_none>                        ; 45
    op_struct<cn_inc,t_normal,op_si,op_none>                        ; 46
    op_struct<cn_inc,t_normal,op_di,op_none>                        ; 47
    op_struct<cn_dec,t_normal,op_ax,op_none>                        ; 48 DEC2
    op_struct<cn_dec,t_normal,op_cx,op_none>                        ; 49
    op_struct<cn_dec,t_normal,op_dx,op_none>                        ; 4A
    op_struct<cn_dec,t_normal,op_bx,op_none>                        ; 4B
    op_struct<cn_dec,t_normal,op_sp,op_none>                        ; 4C
    op_struct<cn_dec,t_normal,op_bp,op_none>                        ; 4D
    op_struct<cn_dec,t_normal,op_si,op_none>                        ; 4E
    op_struct<cn_dec,t_normal,op_di,op_none>                        ; 4F

    op_struct<cn_push,t_normal,op_ax,op_none>                       ; 50 PUSH2
    op_struct<cn_push,t_normal,op_cx,op_none>                       ; 51
    op_struct<cn_push,t_normal,op_dx,op_none>                       ; 52
    op_struct<cn_push,t_normal,op_bx,op_none>                       ; 53
    op_struct<cn_push,t_normal,op_sp,op_none>                       ; 54
    op_struct<cn_push,t_normal,op_bp,op_none>                       ; 55
    op_struct<cn_push,t_normal,op_si,op_none>                       ; 56
    op_struct<cn_push,t_normal,op_di,op_none>                       ; 57
    op_struct<cn_pop,t_normal,op_ax,op_none>                        ; 58 POP2
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

    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>           ; 80 ext_opc_80_83
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm16>         ; 81
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem8,op_imm8>           ; 82
    op_struct<ext_opc_80_83,t_ext_opc,op_regmem16,op_imm8_ext>      ; 83
    op_struct<cn_test,t_normal,op_regmem8,op_reg8>                  ; 84 TEST1
    op_struct<cn_test,t_normal,op_regmem16,op_reg16>                ; 85
    op_struct<cn_xchg,t_normal,op_regmem8,op_reg8>                  ; 86 XCHG1
    op_struct<cn_xchg,t_normal,op_regmem16,op_reg16>                ; 87
    op_struct<cn_mov,t_normal,op_regmem8,op_reg8>                   ; 88 MOV1
    op_struct<cn_mov,t_normal,op_regmem16,op_reg16>                 ; 89
    op_struct<cn_mov,t_normal,op_reg8,op_regmem8>                   ; 8A
    op_struct<cn_mov,t_normal,op_reg16,op_regmem16>                 ; 8B
    op_struct<cn_mov,t_normal_sreg,op_regmem16,op_sreg>             ; 8C MOV6
    op_struct<cn_lea,t_normal,op_reg16,op_regmem16>                 ; 8D LEA
    op_struct<cn_mov,t_normal_sreg,op_sreg,op_regmem16>             ; 8E MOV6
    op_struct<cn_pop,t_normal,op_regmem16,op_none>                  ; 8F POP1

    op_struct<cn_xchg,t_normal,op_ax,op_ax>                         ; 90 XCHG2
    op_struct<cn_xchg,t_normal,op_cx,op_ax>                         ; 91
    op_struct<cn_xchg,t_normal,op_dx,op_ax>                         ; 92
    op_struct<cn_xchg,t_normal,op_bx,op_ax>                         ; 93
    op_struct<cn_xchg,t_normal,op_sp,op_ax>                         ; 94
    op_struct<cn_xchg,t_normal,op_bp,op_ax>                         ; 95
    op_struct<cn_xchg,t_normal,op_si,op_ax>                         ; 96
    op_struct<cn_xchg,t_normal,op_di,op_ax>                         ; 97
    op_struct<cn_cbw,t_normal,op_none,op_none>                      ; 98 CBW
    op_struct<cn_cwd,t_normal,op_none,op_none>                      ; 99 CWD
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_pushf,t_normal,op_none,op_none>                    ; 9C
    op_struct<cn_popf,t_normal,op_none,op_none>                     ; 9D
    op_struct<cn_sahf,t_normal,op_none,op_none>                     ; 9E
    op_struct<cn_lahf,t_normal,op_none,op_none>                     ; 9F

    op_struct<cn_mov,t_daddr,op_al,op_daddr>                         ; A0 MOV4
    op_struct<cn_mov,t_daddr,op_ax,op_daddr>                         ; A1
    op_struct<cn_mov,t_daddr,op_daddr,op_al>                         ; A2 MOV5
    op_struct<cn_mov,t_daddr,op_daddr,op_ax>                         ; A3
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_test,t_normal,op_al,op_imm8>                       ; A8 TEST3
    op_struct<cn_test,t_normal,op_ax,op_imm16>                      ; A9
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<cn_mov,t_imm,op_al,op_imm8>                           ; B0 MOV3
    op_struct<cn_mov,t_imm,op_cl,op_imm8>                           ; B1
    op_struct<cn_mov,t_imm,op_dl,op_imm8>                           ; B2
    op_struct<cn_mov,t_imm,op_bl,op_imm8>                           ; B3
    op_struct<cn_mov,t_imm,op_ah,op_imm8>                           ; B4
    op_struct<cn_mov,t_imm,op_ch,op_imm8>                           ; B5
    op_struct<cn_mov,t_imm,op_dh,op_imm8>                           ; B6
    op_struct<cn_mov,t_imm,op_bh,op_imm8>                           ; B7
    op_struct<cn_mov,t_imm,op_ax,op_imm16>                          ; B8
    op_struct<cn_mov,t_imm,op_cx,op_imm16>                          ; B9
    op_struct<cn_mov,t_imm,op_dx,op_imm16>                          ; BA
    op_struct<cn_mov,t_imm,op_bx,op_imm16>                          ; BB
    op_struct<cn_mov,t_imm,op_sp,op_imm16>                          ; BC
    op_struct<cn_mov,t_imm,op_bp,op_imm16>                          ; BD
    op_struct<cn_mov,t_imm,op_si,op_imm16>                          ; BE
    op_struct<cn_mov,t_imm,op_di,op_imm16>                          ; BF

    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_les,t_normal,op_reg16,op_regmem16>                 ; C4 LES
    op_struct<cn_lds,t_normal,op_reg16,op_regmem16>                 ; C5 LDS
    op_struct<cn_mov,t_ext,op_imm8,op_regmem8>                      ; C6 MOV2
    op_struct<cn_mov,t_ext,op_imm16,op_regmem8>                     ; C7
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_1_const>        ; D0 ext_opc_d0_d3
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_1_const>       ; D1
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_cl>             ; D2
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_cl>            ; D3
    op_struct<cn_aam,t_2byte,op_none,op_none>                       ; D4 AAM
    op_struct<cn_aad,t_2byte,op_none,op_none>                       ; D5 AAD
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_xlat,t_normal,op_none,op_none>                     ; D7 XLAT
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>

    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_in,t_normal,op_al,op_port>                         ; E4 IN1
    op_struct<cn_in,t_normal,op_ax,op_port>                         ; E5
    op_struct<cn_out,t_normal,op_port,op_al>                        ; E6 OUT1
    op_struct<cn_out,t_normal,op_port,op_ax>                        ; E7
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_in,t_normal,op_al,op_dx>                           ; EC IN2
    op_struct<cn_in,t_normal,op_ax,op_dx>                           ; ED
    op_struct<cn_out,t_normal,op_dx,op_al>                          ; EE OUT2
    op_struct<cn_out,t_normal,op_dx,op_ax>                          ; EF

    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<ext_opc_f6_f7,t_ext_opc,op_regmem8,op_none>           ; F6 ext_opc_f6_f7
    op_struct<ext_opc_f6_f7,t_ext_opc,op_regmem16,op_none>          ; F7
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<cn_unk,t_unk,op_none,op_none>
    op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem8,op_none>           ; FE ext_opc_fe_ff
    op_struct<ext_opc_fe_ff,t_ext_opc,op_regmem16,op_none>          ; FF

; Extended opcode segments
label ext_opc_80_83
    dw cn_add ; ADD2 000
    dw cn_or  ; OR2  001
    dw cn_adc ; ADC2 010
    dw cn_sbb ; SBB2 011
    dw cn_and ; AND2 100
    dw cn_sub ; SUB2 101
    dw cn_xor ; XOR2 110
    dw cn_cmp ; CMP2 111

label ext_opc_d0_d3
    dw cn_unk     ; -
    dw cn_unk     ; -
    dw cn_unk     ; -
    dw cn_unk     ; -
    dw cn_shl_sal ; SHL/SAL 100
    dw cn_shr     ; SHR     101
    dw cn_unk     ; -
    dw cn_sar     ; SAR     111

label ext_opc_f6_f7
    dw cn_test ; TEST2 000 - implement separately
    dw cn_unk  ; -
    dw cn_not  ; NOT   010
    dw cn_neg  ; NEG   011
    dw cn_mul  ; MUL   100
    dw cn_imul ; IMUL  101
    dw cn_div  ; DIV   110
    dw cn_idiv ; IDIV  111

label ext_opc_fe_ff
    dw cn_inc  ; INC1 000
    dw cn_dec  ; DEC1 001
    dw cn_unk  ; -
    dw cn_unk  ; -
    dw cn_unk  ; -
    dw cn_unk  ; -
    dw cn_push ; PUSH1 110
    dw cn_unk  ; -

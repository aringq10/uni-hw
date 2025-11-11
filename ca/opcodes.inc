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
    op_3_const  equ 21
    op_none     equ 22

; other
    ; 30-34 need address byte
    op_reg8     equ 30
    op_reg16    equ 31
    op_regmem8  equ 32
    op_regmem16 equ 33
    op_sreg     equ 34
    ; 40-45 no address byte needed
    op_imm8     equ 40
    op_imm16    equ 41
    op_daddr    equ 42
    op_disp8    equ 43
    op_disp16   equ 44
    op_faddr    equ 45

; COMMAND TYPES
    t_unk       equ 50
    t_unsupp    equ 51
    t_normal    equ 52
    t_ext_opc   equ 53
    t_seg_ov    equ 54
    t_2byte     equ 55

; COMMAND NAMES
    cn_es_ov     db "ES:$"
    cn_cs_ov     db "CS:$"
    cn_ss_ov     db "SS:$"
    cn_ds_ov     db "DS:$"
    cn_unk       db "UNK$"
    cn_unsupp    db "UNSUPPORTED$"
    cn_aaa       db "AAA$"
    cn_aad       db "AAD$"
    cn_aam       db "AAM$"
    cn_aas       db "AAS$"
    cn_adc       db "ADC$"
    cn_add       db "ADD$"
    cn_and       db "AND$"
    cn_call      db "CALL$"
    cn_cbw       db "CBW$"
    cn_clc       db "CLC$"
    cn_cld       db "CLD$"
    cn_cli       db "CLI$"
    cn_cmc       db "CMC$"
    cn_cmp       db "CMP$"
    cn_cmpsb     db "CMPSB$"
    cn_cmpsw     db "CMPSW$"
    cn_cwd       db "CWD$"
    cn_daa       db "DAA$"
    cn_das       db "DAS$"
    cn_dec       db "DEC$"
    cn_div       db "DIV$"
    cn_esc0      db "ESC0$"
    cn_esc1      db "ESC1$"
    cn_esc2      db "ESC2$"
    cn_esc3      db "ESC3$"
    cn_esc4      db "ESC4$"
    cn_esc5      db "ESC5$"
    cn_esc6      db "ESC6$"
    cn_esc7      db "ESC7$"
    cn_hlt       db "HLT$"
    cn_idiv      db "IDIV$"
    cn_imul      db "IMUL$"
    cn_in        db "IN$"
    cn_inc       db "INC$"
    cn_int       db "INT$"
    cn_into      db "INTO$"
    cn_iret      db "IRET$"
    cn_lahf      db "LAHF$"
    cn_jmp       db "JMP$"
    cn_jcxz      db "JCXZ$"
    cn_ja        db "JA$"
    cn_jae_jnc   db "JAE/JNC$"
    cn_jb_jc     db "JB/JC$"
    cn_jbe       db "JBE$"
    cn_je_jz     db "JE/JZ$"
    cn_jg        db "JG$"
    cn_jge       db "JGE$"
    cn_jl        db "JL$"
    cn_jle       db "JLE$"
    cn_jne_jnz   db "JNE/JNZ$"
    cn_jno       db "JNO$"
    cn_jnp_jpo   db "JNP/JPO$"
    cn_jp_jpe    db "JP/JPE$"
    cn_jo        db "JO$"
    cn_jns       db "JNS$"
    cn_js        db "JS$"
    cn_lds       db "LDS$"
    cn_lea       db "LEA$"
    cn_les       db "LES$"
    cn_lock      db "LOCK$"
    cn_lodsb     db "LODSB$"
    cn_lodsw     db "LODSW$"
    cn_loop      db "LOOP$"
    cn_loopne_nz db "LOOPNE/NZ$"
    cn_loope_z   db "LOOPE/Z$"
    cn_mov       db "MOV$"
    cn_movsb     db "MOVSB$"
    cn_movsw     db "MOVSW$"
    cn_mul       db "MUL$"
    cn_neg       db "NEG$"
    cn_not       db "NOT$"
    cn_or        db "OR$"
    cn_out       db "OUT$"
    cn_pop       db "POP$"
    cn_popf      db "POPF$"
    cn_push      db "PUSH$"
    cn_pushf     db "PUSHF$"
    cn_rcl       db "RCL$"
    cn_rcr       db "RCR$"
    cn_rep_z_e   db "REP/Z/E$"
    cn_repnz_ne  db "REPNZ/NE$"
    cn_ret       db "RET$"
    cn_retf      db "RETF$"
    cn_retn      db "RETN$"
    cn_rol       db "ROL$"
    cn_ror       db "ROR$"
    cn_sahf      db "SAHF$"
    cn_sar       db "SAR$"
    cn_sbb       db "SBB$"
    cn_scasb     db "SCASB$"
    cn_scasw     db "SCASW$"
    cn_shl_sal   db "SHL$"
    cn_shr       db "SHR$"
    cn_stc       db "STC$"
    cn_std       db "STD$"
    cn_sti       db "STI$"
    cn_stos      db "STOS$" 
    cn_stosb     db "STOSB$"
    cn_stosw     db "STOSW$"
    cn_sub       db "SUB$"
    cn_test      db "TEST$"
    cn_wait      db "WAIT$"
    cn_xchg      db "XCHG$"
    cn_xlat      db "XLAT$"
    cn_xor       db "XOR$"



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
    op_struct<cn_es_ov,t_seg_ov,op_none,op_none>                    ; 26 seg_ov ES
    op_struct<cn_daa,t_normal,op_none,op_none>                      ; 27
    op_struct<cn_sub,t_normal,op_regmem8,op_reg8>                   ; 28
    op_struct<cn_sub,t_normal,op_regmem16,op_reg16>                 ; 29
    op_struct<cn_sub,t_normal,op_reg8,op_regmem8>                   ; 2A
    op_struct<cn_sub,t_normal,op_reg16,op_regmem16>                 ; 2B
    op_struct<cn_sub,t_normal,op_al,op_imm8>                        ; 2C
    op_struct<cn_sub,t_normal,op_ax,op_imm16>                       ; 2D
    op_struct<cn_cs_ov,t_seg_ov,op_none,op_none>                    ; 2E seg_ov CS
    op_struct<cn_das,t_normal,op_none,op_none>                      ; 2F

    op_struct<cn_xor,t_normal,op_regmem8,op_reg8>                   ; 30
    op_struct<cn_xor,t_normal,op_regmem16,op_reg16>                 ; 31
    op_struct<cn_xor,t_normal,op_reg8,op_regmem8>                   ; 32
    op_struct<cn_xor,t_normal,op_reg16,op_regmem16>                 ; 33
    op_struct<cn_xor,t_normal,op_al,op_imm8>                        ; 34
    op_struct<cn_xor,t_normal,op_ax,op_imm16>                       ; 35
    op_struct<cn_ss_ov,t_seg_ov,op_none,op_none>                    ; 36 seg_ov SS
    op_struct<cn_aaa,t_normal,op_none,op_none>                      ; 37
    op_struct<cn_cmp,t_normal,op_regmem8,op_reg8>                   ; 38
    op_struct<cn_cmp,t_normal,op_regmem16,op_reg16>                 ; 39
    op_struct<cn_cmp,t_normal,op_reg8,op_regmem8>                   ; 3A
    op_struct<cn_cmp,t_normal,op_reg16,op_regmem16>                 ; 3B
    op_struct<cn_cmp,t_normal,op_al,op_imm8>                        ; 3C
    op_struct<cn_cmp,t_normal,op_ax,op_imm16>                       ; 3D
    op_struct<cn_ds_ov,t_seg_ov,op_none,op_none>                    ; 3E seg_ov DS
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

    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; 60 60-6F not used
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
    op_struct<cn_jb_jc,t_normal,op_disp8,op_none>                      ; 72
    op_struct<cn_jae_jnc,t_normal,op_disp8,op_none>                 ; 73
    op_struct<cn_je_jz,t_normal,op_disp8,op_none>                   ; 74
    op_struct<cn_jne_jnz,t_normal,op_disp8,op_none>                 ; 75
    op_struct<cn_jbe,t_normal,op_disp8,op_none>                     ; 76
    op_struct<cn_ja,t_normal,op_disp8,op_none>                      ; 77
    op_struct<cn_js,t_normal,op_disp8,op_none>                      ; 78
    op_struct<cn_jns,t_normal,op_disp8,op_none>                     ; 79
    op_struct<cn_jp_jpe,t_normal,op_disp8,op_none>                  ; 7A
    op_struct<cn_jnp_jpo,t_normal,op_disp8,op_none>                 ; 7B
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
    op_struct<cn_call,t_normal,op_faddr,op_none>                    ; 9A
    op_struct<cn_wait,t_normal,op_none,op_none>                     ; 9B
    op_struct<cn_pushf,t_normal,op_none,op_none>                    ; 9C
    op_struct<cn_popf,t_normal,op_none,op_none>                     ; 9D
    op_struct<cn_sahf,t_normal,op_none,op_none>                     ; 9E
    op_struct<cn_lahf,t_normal,op_none,op_none>                     ; 9F

    op_struct<cn_mov,t_normal,op_al,op_daddr>                       ; A0
    op_struct<cn_mov,t_normal,op_ax,op_daddr>                       ; A1
    op_struct<cn_mov,t_normal,op_daddr,op_al>                       ; A2
    op_struct<cn_mov,t_normal,op_daddr,op_ax>                       ; A3
    op_struct<cn_movsb,t_normal,op_none,op_none>                    ; A4
    op_struct<cn_movsw,t_normal,op_none,op_none>                    ; A5
    op_struct<cn_cmpsb,t_normal,op_none,op_none>                    ; A6
    op_struct<cn_cmpsw,t_normal,op_none,op_none>                    ; A7
    op_struct<cn_test,t_normal,op_al,op_imm8>                       ; A8
    op_struct<cn_test,t_normal,op_ax,op_imm16>                      ; A9
    op_struct<cn_stosb,t_normal,op_none,op_none>                    ; AA
    op_struct<cn_stosw,t_normal,op_none,op_none>                    ; AB
    op_struct<cn_lodsb,t_normal,op_none,op_none>                    ; AC
    op_struct<cn_lodsw,t_normal,op_none,op_none>                    ; AD
    op_struct<cn_scasb,t_normal,op_none,op_none>                    ; AE
    op_struct<cn_scasw,t_normal,op_none,op_none>                    ; AF

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

    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; C0 - not used
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; C1 - not used
    op_struct<cn_ret,t_normal,op_imm16,op_none>                     ; C2
    op_struct<cn_ret,t_normal,op_none,op_none>                      ; C3
    op_struct<cn_les,t_normal,op_reg16,op_regmem16>                 ; C4
    op_struct<cn_lds,t_normal,op_reg16,op_regmem16>                 ; C5
    op_struct<cn_mov,t_normal,op_regmem8,op_imm8>                   ; C6
    op_struct<cn_mov,t_normal,op_regmem16,op_imm16>                 ; C7
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; C8 - not used
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; C9 - not used
    op_struct<cn_ret,t_normal,op_imm16,op_none>                     ; CA
    op_struct<cn_ret,t_normal,op_none,op_none>                      ; CB
    op_struct<cn_int,t_normal,op_3_const,op_none>                   ; CC
    op_struct<cn_int,t_normal,op_imm8,op_none>                      ; CD
    op_struct<cn_into,t_normal,op_none,op_none>                     ; CE
    op_struct<cn_iret,t_normal,op_none,op_none>                     ; CF

    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_1_const>        ; D0 ext_opc_d0_d3
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_1_const>       ; D1
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem8,op_cl>             ; D2
    op_struct<ext_opc_d0_d3,t_ext_opc,op_regmem16,op_cl>            ; D3
    op_struct<cn_aam,t_2byte,op_none,op_none>                       ; D4
    op_struct<cn_aad,t_2byte,op_none,op_none>                       ; D5
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; D6
    op_struct<cn_xlat,t_normal,op_none,op_none>                     ; D7
    op_struct<cn_esc0,t_normal,op_regmem8,op_none>                  ; D8
    op_struct<cn_esc1,t_normal,op_regmem8,op_none>                  ; D9
    op_struct<cn_esc2,t_normal,op_regmem8,op_none>                  ; DA
    op_struct<cn_esc3,t_normal,op_regmem8,op_none>                  ; DB
    op_struct<cn_esc4,t_normal,op_regmem8,op_none>                  ; DC
    op_struct<cn_esc5,t_normal,op_regmem8,op_none>                  ; DD
    op_struct<cn_esc6,t_normal,op_regmem8,op_none>                  ; DE
    op_struct<cn_esc7,t_normal,op_regmem8,op_none>                  ; DF

    op_struct<cn_loopne_nz,t_normal,op_disp8,op_none>                  ; E0
    op_struct<cn_loope_z,t_normal,op_disp8,op_none>                   ; E1
    op_struct<cn_loop,t_normal,op_disp8,op_none>                    ; E2
    op_struct<cn_jcxz,t_normal,op_disp8,op_none>                    ; E3
    op_struct<cn_in,t_normal,op_al,op_imm8>                         ; E4
    op_struct<cn_in,t_normal,op_ax,op_imm8>                         ; E5
    op_struct<cn_out,t_normal,op_imm8,op_al>                        ; E6
    op_struct<cn_out,t_normal,op_imm8,op_ax>                        ; E7
    op_struct<cn_call,t_normal,op_disp16,op_none>                   ; E8
    op_struct<cn_jmp,t_normal,op_disp16,op_none>                    ; E9
    op_struct<cn_jmp,t_normal,op_faddr,op_none>                     ; EA
    op_struct<cn_jmp,t_normal,op_disp8,op_none>                     ; EB
    op_struct<cn_in,t_normal,op_al,op_dx>                           ; EC
    op_struct<cn_in,t_normal,op_ax,op_dx>                           ; ED
    op_struct<cn_out,t_normal,op_dx,op_al>                          ; EE
    op_struct<cn_out,t_normal,op_dx,op_ax>                          ; EF

    op_struct<cn_lock,t_normal,op_none,op_none>                     ; F0
    op_struct<cn_unsupp,t_unsupp,op_none,op_none>                   ; F1 - not used
    op_struct<cn_repnz_ne,t_normal,op_none,op_none>                 ; F2
    op_struct<cn_rep_z_e,t_normal,op_none,op_none>                  ; F3
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
    dw cn_add
    dw cn_or
    dw cn_adc
    dw cn_sbb
    dw cn_and
    dw cn_sub
    dw cn_xor
    dw cn_cmp

label ext_opc_d0_d3
    dw cn_rol
    dw cn_ror
    dw cn_rcl
    dw cn_rcr
    dw cn_shl_sal
    dw cn_shr
    dw cn_unsupp
    dw cn_sar


label ext_opc_f6_f7
    dw cn_test    ; TEST - implement separately (op_regmem, op_imm)
    dw cn_unsupp
    dw cn_not
    dw cn_neg
    dw cn_mul
    dw cn_imul
    dw cn_div
    dw cn_idiv

label ext_opc_fe_ff
    dw cn_inc
    dw cn_dec
    dw cn_call
    dw cn_call
    dw cn_jmp
    dw cn_jmp
    dw cn_push
    dw cn_unsupp

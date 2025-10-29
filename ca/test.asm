.model tiny

buferioDydis	EQU	121
.code
org 100h

start:
    rep
    repnz
    movsb
    cmpsb
    scasb
    lodsb
    stosb

    movsw
    cmpsw
    scasw
    lodsw
    stosw

    int 3
    into

    sal al, 3
    sar bx, 2
    shl ax, 10
    shr si, 4

    db  9Ah         ; opcode CALL FAR
    dw  5678h        ; offset
    dw  1234h        ; segment

    db  0EAh         ; opcode CALL FAR
    dw  8765h        ; offset
    dw  4321h        ; segment

    db 0FFh
    db 0E8h

    jmp .end
    mov dl, var8
    mov bx, var16

    and	[bx+si], ah
    and	[bx+si], ah
    or ax, 240Ah
    test bl, 08h
    test bx, 1234h

    in al, 20h
    in ax, 20h
    in al, dx
    in ax, dx
    out 21h, al
    out 21h, ax
    out dx, al
    out dx, ax
    mov cx, 2
.loop:
    dec cx
    mov bl, var8
    mov si, var16
    cmp cx, 0
    je .loop
    jb .loop
    
    mov ax, es:[80h]
    mov bx, ds:[bp]

	MOV	ah, 9
	MOV	dx, offset ivesk
	INT	21h

	MOV	ah, 0Ah
	MOV	dx, offset bufDydis
	INT	21h

    MOV	ah, 9
	MOV	dx, offset enteris
	INT	21h

;****algoritmas****
	XOR	ch, ch
	SUB	ax, ax
	MOV	cl, nuskaite
	MOV	bx, offset buferis
	MOV	dl, 'A'
	MOV	dh, 'Z'

	XOR	ch, ch
	SUB	ax, ax
	MOV	cl, nuskaite
	MOV	bx, offset buferis
	MOV	dl, 'A'
	MOV	dh, 'Z'

	XOR	ch, ch
	SUB	ax, ax
	MOV	cl, nuskaite
	MOV	bx, offset buferis
	MOV	dl, 'A'
	MOV	dh, 'Z'

ciklas1:
	CMP	dl, [ds:bx]
	JG	nelygu
	CMP	dh, [ds:bx]
	JL	nelygu
	INC	ax

nelygu:
	INC	bx
	DEC	cx
	CMP	cx, 0
	JG	ciklas1

;****Spausdinimas****
	MOV	dl, 10
	DIV	dl
	MOV	[rezult2 + 2], ah
	ADD	[rezult2 + 2], 030h
	XOR	ah, ah
	DIV	dl
	MOV	[rezult2 + 1], ah
	ADD	[rezult2 + 1], 030h
	MOV	[rezult2], al
	ADD	[rezult2], 030h

	MOV	dl, 10
	DIV	dl
	MOV	[rezult2 + 2], ah
	ADD	[rezult2 + 2], 030h
	XOR	ah, ah
	DIV	dl
	MOV	[rezult2 + 1], ah
	ADD	[rezult2 + 1], 030h
	MOV	[rezult2], al
	ADD	[rezult2], 030h

	MOV	dl, 10
	DIV	dl
	MOV	[rezult2 + 2], ah
	ADD	[rezult2 + 2], 030h
	XOR	ah, ah
	DIV	dl
	MOV	[rezult2 + 1], ah
	ADD	[rezult2 + 1], 030h
	MOV	[rezult2], al
	ADD	[rezult2], 030h

	MOV	ah, 9
	MOV	dx, offset rezult
	INT	21h



.end:
    mov ax, 4C00h
    int 21h



; === Data segment ===
var8    db 69h
var16   dw 1234h
	bufDydis DB  buferioDydis
	nuskaite DB  ?
	buferis	 DB  buferioDydis dup ('$')
	ivesk	 DB  'Iveskite eilute:', 13, 10, '$'
	rezult	 DB  'Radau tiek didziuju raidziu: '
	rezult2	 DB  3 dup (' ')
	enteris	 DB  13, 10, '$'
end start

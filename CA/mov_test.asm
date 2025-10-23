.model tiny
.code
org 100h

start:
    mov ax, bx
    mov al, cl
    mov bx, dx
    mov ch, bl
    mov si, di
    mov bp, ax

    ; === Register <-> Memory ===
    mov byte ptr [var8], al
    mov word ptr [var16], ax
    mov al, [var8]
    mov ax, [var16]
    mov [bx], al
    mov [si], ax
    mov dl, [bx]
    mov di, [var16]

    ; === Immediate to Register ===
    mov al, 12h
    mov bl, 0FFh
    mov ax, 1234h
    mov cx, 0ABCDh

    ; === Immediate to Memory ===
    mov byte ptr [var8], 55h
    mov word ptr [var16], 6789h

    ; === Direct Memory Addressing ===
    mov al, var8
    mov ax, var16
    mov var8, al
    mov var16, ax

    ; === Segment Register Moves ===
    mov ax, ds
    mov ds, ax
    mov bx, es
    mov es, bx
    mov cx, ss
    mov ss, cx
    mov ax, cs          ; valid (only AX works)

    mov al, [var8] 
    mov ax, 4C00h
    int 21h

; === Data segment ===
var8    db 0
var16   dw 0

end start

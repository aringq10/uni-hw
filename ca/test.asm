.model tiny
.code
org 100h

start:
    test bl, 08h
    test bx, 1234h

    in al, 20
    in ax, 20
    in al, dx
    in ax, dx
    out 21, al
    out 21, ax
    out dx, al
    out dx, ax
    mov cx, 2
.loop:
    dec cx
    cmp cx, 0
    je .loop
    jb .loop
    
    mov ax, es:[80h]
    mov bx, ds:[bp]

    mov ax, 4C00h
    int 21h

; === Data segment ===
var8    db 69h
var16   dw 1234h

end start

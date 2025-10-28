.model tiny
.code
org 100h

start:
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

    mov ax, 4C00h
    int 21h

; === Data segment ===
var8    db 69h
var16   dw 1234h

end start

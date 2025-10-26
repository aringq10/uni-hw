.model tiny
.code
org 100h

start:
;
; MOV 1: register <-> register/memory
;
    ; no offset
    mov ax, [bx+si]
    mov bx, [bx+di]
    mov cx, [bp+si]
    mov dx, [bp+di]
    mov ah, [si]
    mov bl, [di]
    mov ch, [bp]
    mov dl, [bx]
    mov [bx+si], ax
    mov [bx+di], bx
    mov [bp+si], cx
    mov [bp+di], dx
    mov [si], al
    mov [di], bh
    mov [bp], cl
    mov [bx], dh
    ; 1 byte offset
    mov ax, [bx+si+01h]
    mov bx, [bx+di+02h]
    mov cx, [bp+si+03h]
    mov dx, [bp+di+04h]
    mov ah, [si+05h]
    mov bl, [di+06h]
    mov ch, [bp+07h]
    mov dl, [bx+08h]
    mov [bx+si+01h], ax 
    mov [bx+di+02h], bx 
    mov [bp+si+03h], cx 
    mov [bp+di+04h], dx 
    mov [si+05h], al 
    mov [di+06h], bh 
    mov [bp+07h], cl 
    mov [bx+08h], dh 
    ; 2 byte offset
    mov ax, [bx+si+0102h]
    mov bx, [bx+di+0203h]
    mov cx, [bp+si+0304h]
    mov dx, [bp+di+0405h]
    mov ah, [si+0506h]
    mov bl, [di+0607h]
    mov ch, [bp+0708h]
    mov dl, [bx+0809h]
    mov [bx+si+0102h], ax
    mov [bx+di+0203h], bx
    mov [bp+si+0304h], cx
    mov [bp+di+0405h], dx
    mov [si+0506h], al
    mov [di+0607h], bh
    mov [bp+0708h], cl
    mov [bx+0809h], dh
    ; register <-> register
    mov ax, bx
    mov dx, si
    mov cx, bp
    mov di, sp
    mov ah, al

;
; MOV 2: imm -> memory/register
;
    mov word ptr [bx+si], 1234h
    mov byte ptr [si], 12h
    mov word ptr var16, 0ABCDh
    mov byte ptr var8, 0ABh
;
; MOV 3: imm -> register
;
    mov ax, 0102h
    mov al, 01h
    mov bx, 0203h
    mov bh, 02h
    mov cx, 0304h
    mov ch, 03h
    mov dx, 0405h
    mov dl, 04h
;    
; MOV 4, 5: AX/AL <-> memory
;
    mov al, var8
    mov ax, var16
    mov var8, al
    mov var16, ax

;
; MOV 6: Segment registers
;
    mov ax, ds
    mov es, ax
    mov bx, ss
    mov ds, bx
    mov es, cx
    mov cx, es
    mov es, var16
    mov ds, [bx+si+0304h]

;
; Segment overrides
;

    mov ax, es:[si]
    mov bx, es:[80h]
    mov ax, cs:[bx]
    mov ax, ss:[di]
    mov ax, ds:[bp]
    mov es:[80h], 1234h

    mov ax, 4C00h
    int 21h

; === Data segment ===
var8    db 69h
var16   dw 1234h

end start

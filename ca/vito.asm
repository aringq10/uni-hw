;1. Parašykite programą, kuri viename faile ieško nurodyto fragmento ir visus rastus 
;pakeičia į kitą nurodytą fragmentą, o rezultatą įrašo į kitą failą. Pvz.: Jei „duom.txt“ 
;failas yra toks, tai komanda antra duom.txt abcd EFG rez.txt sukurtų tokį „rez.txt“ failą.
;Labas.exe in.txt out.txt
;int 21h ah 62
;Funkcijos?


.MODEL SMALL
.STACK 100h
.DATA
    msg1 db 'Enter a string to search for', '$'
    msg2 db 'Enter a string to replace with', '$'
    file_err_msg db "Something went wrong with one of the files", '$'
    arg_err_msg db "Forgot to enter arguments!", '$'
    newline db 0dh,0ah,'$'
    bytesread dw ?
    Intxt db 255 dup(?)
    Outtxt db 255 dup(?)
    InHandle dw ?
    OutHandle dw ?
    search db 255,0,255 dup(0)
    replace db 255,0,255 dup(0)
    buffer db 1024 dup(0) ; 1024,0,1024 dup(0)
    linebuffer db 255 dup(0) ;linebuffer db 255,0,255 dup(0)
.CODE

wrt PROC
    push cx
    push si
    ;write linebuffer to output file
    mov bx, [OutHandle] ; file handle in DI
    ;calculate length of linebuffer
    mov si, offset linebuffer ; skip length byte
    xor cx, cx
    len_loop:
        mov al, [si]               ; load byte at SI
        cmp al, 0
        je len_done
        inc si
        inc cx
        jmp len_loop
    len_done:
        mov dx, offset linebuffer ; data starts after length byte
        mov ah, 40h        ; DOS: write file
        mov cx, 5
        mov bx, [OutHandle]
        mov bx, 1
        int 21h

    pop si
    pop cx
    ret
wrt ENDP

file_err:
    mov dx, offset file_err_msg
    mov ah, 09h
    int 21h

    mov ax, 4C00h
    int 21h

arg_err:
    mov dx, offset arg_err_msg
    mov ah, 09h
    int 21h

    mov ax, 4C00h
    int 21h

strt:
    ;define data
    mov ax, @data
    mov ds, ax

    ;Read file names from command line File.exe in.txt out.txt
    xor cx, cx
    mov cl, es:[80h]
    mov si, 0

    mov di, offset Intxt
    call get_arg
    cmp bx, 0
    je arg_err
    mov byte ptr [di], 0

    mov di, offset Outtxt
    call get_arg
    cmp bx, 0
    je arg_err
    mov byte ptr [di], 0

    ; open input file
    mov ah, 3Dh
    mov al, 0
    mov dx, offset Intxt
    int 21h
    mov InHandle, ax
    jc file_err

    ;open output file
    mov ah, 3Ch
    mov cx, 0
    mov dx, offset Outtxt
    int 21h
    mov OutHandle, ax
    jc file_err

    ; get search string
    mov dx, offset msg1
    mov ah, 09h
    int 21h
    mov dx, offset newline
    int 21h

    mov dx, offset search
    mov ah, 0Ah
    int 21h
    mov ah, 09h
    mov dx, offset newline
    int 21h

    ; get replace string
    mov dx, offset msg2
    mov ah, 09h
    int 21h
    mov dx, offset newline
    int 21h

    mov dx, offset replace
    mov ah, 0Ah
    int 21h
    mov ah, 09h
    mov dx, offset newline
    int 21h




    mov ax, ds
    mov es, ax


    ;read from input file to buffer
    mov bx, [InHandle]
    mov ah, 3Fh        ; DOS: read file
    mov cx, 1024       ; number of bytes to read
    mov dx, offset buffer ; buffer to store data
    int 21h
    mov cx, ax
    mov bytesread, ax

    ;buffer -> linebuffer
    mov si, offset buffer  ; SI points to start of buffer
    lines:
    mov di, offset linebuffer ; start of linebuffer

    char:
        cmp cx, 0
        je done
        lodsb                     ; load byte from buffer into AL, SI++
        dec cx
        cmp al, 0Dh               ; carriage return = end of line
        je ready
        cmp al, 'd'
        je ready
        stosb                      ; store byte into linebuffer, DI++
        jmp char

    ready:
        mov byte ptr [di], 0
        ;call sear
        call wrt
        jmp lines

    done:
    









    ; close input
    mov ah, 3Eh
    mov bx, InHandle
    int 21h

    ; close output
    mov ah, 3Eh
    mov bx, OutHandle
    int 21h

    mov ax, 4C00h
    int 21h

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
        cmp al, 0Dh
        je .end_arg
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

END strt

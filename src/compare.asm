; ---------------------------------------------------
; Compare ( _compare ) two integers.
; Returns 1 if the first number is bigger.
; Returns 0 if numbers are equal.
; Returns -1 if the first number is lesser than the second nuber.
; ---------------------------------------------------

    global  _compare
    section .text


_compare:
    mov     eax,    [esp+4]
    mov     ecx,    [esp+8]
    cmp     eax,    ecx
    jg      _greater
    cmp     eax,    ecx
    jl      _lesser
    mov     eax,    0
    ret


_greater:
    mov     eax,    1
    ret


_lesser:
    mov     eax,    -1
    ret

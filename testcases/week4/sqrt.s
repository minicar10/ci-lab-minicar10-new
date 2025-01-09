mov     x0, 72
mov     x3, 144
mov     x2, 1
mov     x1, 5184
b       .L2

.L11: add     x2, x0, 1
    add     x0, x3, x2
    cmp     x3, x2
    b.lt    .L3

.L6:



    lsr     x4, x0, 63
    add     x0, x0, x4
    asr     x0, x0, 1

    add     x1, x0, 0
    add x5, x0, 0
    mov     x6, 0
.L_square:
    add     x6, x6, 1
    cmp     x6, x5
    b.eq    .L_done
    add     x1, x1, x5
    b       .L_square

.L_done:
    cmp     x1, 144
    b.eq    .L3

.L2:
    cmp     x1, 143
    b.le    .L11
    sub     x3, x0, 1
    add     x0, x3, x2
    cmp     x3, x2
    b.ge    .L6

.L3:
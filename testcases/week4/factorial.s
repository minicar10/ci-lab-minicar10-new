start:
    mov x7, 0
    mov x5, 0
    mov x4, 1
    mov x2, 9
    mov x0, 1
.L2:
    orr x1, x2, x7
    orr x3, x0, x7
    mov x0, 0
.L4:
    and x6, x1, x4
    cmp x6, x4
    b.ne .L3
    add    x0, x0, x3
.L3:
    lsl x3, x3, 1
    lsr x1, x1, 1
    cmp x1, x5 
    b.ne .L4
    sub    x2, x2, x4
    cmp x2, 0
    b.ne .L2

    print    x0, d
    ret

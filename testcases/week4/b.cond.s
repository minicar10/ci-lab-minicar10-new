start:
    mov x0, 1
    mov x1, 2
    sub x3, x0, x1
    cmp x3, 0
    b.ne .notequal

.goback:
    // Print x0
    //correct value is 26
    //incorrect value is 1
    print x0, d
    ret

.notequal:
    add x0, x0, 13
    add x0, x0, 12
    b .goback


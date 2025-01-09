start:
    mov x0, 1
    mov x1, 2
    sub x3, x0, x1
    cmp x0, 0
    b.eq .helper

.goback:
    // Print x0
    // correct: 1
    print x0, d
    ret

.helper:
    mov x0, 65535
    mov x1, 42069
    b .goback

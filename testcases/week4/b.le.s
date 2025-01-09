mov x0, 42
mov x1, 43
cmp x1, x0
b.le .goback

helper:
    sub x1, x1, 1
    cmp x0, x1    // Keep original comparison order
    b.le helper2
    add x1, x1, x1

.goback:
    mov x0, 0

helper2:
    print x1, d

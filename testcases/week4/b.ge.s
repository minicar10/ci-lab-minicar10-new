mov x0, 42
mov x1, 43
cmp x1, x0
b.ge helper

.goback:
    mov x0 0

helper:
    sub x1 x1 1
    cmp x0, x1
    b.ge helper2
    add x1 x1 x1

helper2:
    // Correct: 42
    print x1 d
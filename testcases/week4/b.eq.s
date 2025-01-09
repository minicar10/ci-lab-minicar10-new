mov x0, 42
mov x1, 42
cmp x0, x1
b.eq helper

.goback:
    add x0, x0, x1

helper:
    // Correct: 42
    print x0 d
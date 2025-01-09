main:
    mov x21, 0
    mov     x19, 0
    mov     x20, 30

test_loop:
    add     x0, x19, x21
    call      fib_iterative
    print x0, d
    add     x19, x19, 1
    cmp     x19, x20
    b.le     test_loop

    ret

fib_iterative:
    cmp     x0, 1
    b.le    .Lexit

    mov     x1, 0
    mov     x2, 1
    mov     x3, 2

.Lloop:
    add     x4, x1, x2
    add     x1, x2, x15
    add      x2, x4, x15
    add     x3, x3, 1
    cmp     x3, x0
    b.le    .Lloop

    add      x0, x2, x15
    b       .Lexit

.Lexit:
    ret
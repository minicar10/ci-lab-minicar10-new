main:
    mov x15, 0
    mov     x21, 0
    mov     x19, 0
    mov     x20, 30

test_loop:
    add     x0, x19, x21
    call      fib_recursive
    print   x0, d
    add     x19, x19, 1
    cmp     x19, x20
    b.le    test_loop
    ret

fib_recursive:
    cmp     x0, 1
    b.le    .Lexit

    sub     x1, x0, 1
    sub     x2, x0, 2

    orr     x0, x1, x15
    call      fib_recursive
    orr     x3, x0, x15

    orr     x0, x2, x15
    call      fib_recursive

    add     x0, x0, x3

.Lexit:
    ret

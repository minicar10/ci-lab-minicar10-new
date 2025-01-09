main:
    mov x19, 0
    mov x20, 1
    mov x21, 10000

sum_loop:
    add x0, x20, 0
    call collatz_steps
    add x19, x19, x0

    add x20, x20, 1
    cmp x20, x21
    b.le sum_loop

    add x0, x19, 0
    print x0, d
    ret

collatz_steps:
    mov x18, 1
    mov x19, 0

loop:
    cmp x0, 1
    b.eq done

    add x1, x0, 0
    and x1, x1, x18
    cmp x1, 0
    b.ne odd

    lsr x0, x0, 1
    b next

odd:
    add x1, x0, 0
    lsl x0, x0, 1
    add x0, x0, x1
    add x0, x0, 1

next:
    add x19, x19, 1
    b loop

done:
    add x0, x19, 0
    ret

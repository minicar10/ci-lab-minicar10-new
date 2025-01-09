start:
    // x1 is stack pointer
    // x2 is limit
    // x3 is counter
    // x4 is junk register (no xzr)
    eor x1, x1, x1
    mov x1, 1023
    mov x5, 16
    sub x1, x1, x5
    eor x2, x2, x2
    mov x7, 26
    add x2, x2, x7
    eor x3, x3, x3 
    add x3, x3, 1

.input_loop:
    sub x4, x3, x2
    cmp x4, 0
    b.eq .start_sum
    store x3, x1, 8
    add x3, x3, 1
    sub x1, x1, x5
    b .input_loop

.start_sum:
    eor x0, x0, x0
    eor x2, x2, x2
    eor x3, x3, x3
    mov x7, 26
    add x3, x3, x7

    eor x6, x6, x6
    mov x5, 16
    mov x7, 1
    sub x7, x6, x7

.sum_loop:
    sub x4, x3, 1
    cmp x4, 0
    b.eq .exit
    add x1, x1, x5
    load x2, 8, x1
    add x0, x0, x2
    eor x2, x2, x2
    add x3, x3, x7
    b .sum_loop

.exit:
    print x0, d
    ret

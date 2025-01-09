start:
    // x1 is stack pointer
    // x2 is limit
    // x3 is counter
    // x4 is junk register (no xzr)
    // x7 has temporary values

    eor x1, x1, x1
    mov x1, 1023
    mov x7, 16
    sub x1, x1, x7
    eor x2, x2, x2
    mov x7, 27
    add x2, x2, x7
    eor x3, x3, x3 
    add x3, x3, 1
    mov x7, 16

.input_loop:
    cmp x3, x2
    b.eq .start_sum
    store x3, x1, 8
    add x3, x3, 1
    sub x1, x1, x7
    b .input_loop

.start_sum:
    // x0 is running sum (and eventual result)
    // x1 is stack pointer
    // x2 is load destination
    // x3 is counter
    // x4 is junk register (no xzr)
    // x5 is stack pointer for x30 saves
    // x6 is x30 save load destination

    eor x0, x0, x0
    eor x2, x2, x2
    eor x3, x3, x3
    mov x7, 26
    add x3, x3, x7
    eor x5, x5, x5
    add x5, x5, x1
    eor x6, x6, x6

    mov x7, 16
    sub x5, x5, x7
    store x30, x5, 8

    // start recursing
    call recursive
    load x30, 8, x5
    add x5, x5, 16

.exit:
    // Print x0
    // correct: 351
    print x0, d
    ret

recursive:
    // x0 is running sum (and eventual result)
    // x1 is stack pointer
    // x2 is load destination
    // x3 is counter
    // x4 is junk register (no xzr)
    // x5 is stack pointer for x30 saves
    // x6 is x30 save load destination

    // prologue, set up stack stuff
    mov x7, 16
    sub x5, x5, x7
    store x30, x5, 8

    eor x7, x7, x7
    cmp x3, x7
    b.eq .base_case

    add x1, x1, 16
    load x2, 8, x1
    add x0, x0, x2
    eor x2, x2, x2
    mov x9, 1
    sub x3, x3, x9
    call recursive

.base_case:
    load x30, 8, x5
    add x5, x5, 16
    ret

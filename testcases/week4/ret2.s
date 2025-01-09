start:
    mov x0, 10
    mov x1, 15
    mov x2, 0
    mov x30, 0xabcdef
    add x2, x30, x2
    call helper
    mov x30, 0
    add x30, x2, x30

    add x0, x0, x1

    // Print x0
    // correct: 43
    print x0, d
    ret

helper:
    mov x0, 28
    mov x1, 85
    ret

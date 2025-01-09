// Written by: Prithvi Jamadagni
// input value is passed in x0
// return is via x0
// register allocations
// x1 has upper half
// x2 has lower half
// x3 has mask
// after a stage, all returns to x0, and we go again



main:
    mov x0 0x137F
    print x0 b
    call popcount
    print x0 d

    mov x0 0xDEADBEEF
    print x0 b
    call popcount
    print x0 d

    mov x0 69420
    print x0 b
    call popcount
    print x0 d

    mov x0 0x12345678
    print x0 b
    call popcount
    print x0 d

    mov x0 0x99999999
    print x0 b
    call popcount
    print x0 d

    mov x0 0x11111111
    print x0 b
    call popcount
    print x0 d

    ret



popcount:
    //val = (val & 0x55555555) + ((val >> 1) & 0x55555555);
    mov x3 0x55555555
    and x1 x0 x3
    lsr x2 x0 1
    and x2 x2 x3
    add x0 x1 x2

    //val = (val & 0x33333333) + ((val >> 2) & 0x33333333);
    mov x3 0x33333333
    and x1 x0 x3
    lsr x2 x0 2
    and x2 x2 x3
    add x0 x1 x2

    //val = (val & 0x0F0F0F0F) + ((val >> 4) & 0x0F0F0F0F);
    mov x3 0x0F0F0F0F
    and x1 x0 x3
    lsr x2 x0 4
    and x2 x2 x3
    add x0 x1 x2

    //val = (val & 0x00FF00FF) + ((val >> 8) & 0x00FF00FF);
    mov x3 0x00FF00FF
    and x1 x0 x3
    lsr x2 x0 8
    and x2 x2 x3
    add x0 x1 x2

    //val = (val & 0x0000FFFF) + ((val >> 16) & 0x0000FFFF);
    mov x3 0x0000FFFF
    and x1 x0 x3
    lsr x2 x0 16
    and x2 x2 x3
    add x0 x1 x2

    ret
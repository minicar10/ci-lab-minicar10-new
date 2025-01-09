main:
    mov x0, 0xabcd
    call rev
    print x0 x
    call rev
    print x0 x

    mov x0, 0xdeadbeef
    call rev
    print x0 x
    call rev
    print x0 x

rev:
    mov x15, 0
    // x0 = input (and later result)
    // x1 = r (result being built)
    // x3 = loop counter (i)
    // x4 = temporary for lookup
    // x6 = AND mask (15)

    mov     x6, 15
    eor     x3, x3, x3    // i = 0
    eor     x1, x1, x1    // r = 0

.loop_body:
    lsl     x1, x1, 4
    and     x4, x0, x6    // Get current nibble

    cmp     x4, 0
    b.eq    .do_0
    cmp     x4, 1
    b.eq    .do_1
    cmp     x4, 2
    b.eq    .do_2
    cmp     x4, 3
    b.eq    .do_3
    cmp     x4, 4
    b.eq    .do_4
    cmp     x4, 5
    b.eq    .do_5
    cmp     x4, 6
    b.eq    .do_6
    cmp     x4, 7
    b.eq    .do_7
    cmp     x4, 8
    b.eq    .do_8
    cmp     x4, 9
    b.eq    .do_9
    cmp     x4, 10
    b.eq    .do_10
    cmp     x4, 11
    b.eq    .do_11
    cmp     x4, 12
    b.eq    .do_12
    cmp     x4, 13
    b.eq    .do_13
    cmp     x4, 14
    b.eq    .do_14
    b       .do_15        // If none of above, must be 15

.do_0:
    mov     x4, 0
    b       .post_lookup
.do_1:
    mov     x4, 8
    b       .post_lookup
.do_2:
    mov     x4, 4
    b       .post_lookup
.do_3:
    mov     x4, 12
    b       .post_lookup
.do_4:
    mov     x4, 2
    b       .post_lookup
.do_5:
    mov     x4, 10
    b       .post_lookup
.do_6:
    mov     x4, 6
    b       .post_lookup
.do_7:
    mov     x4, 14
    b       .post_lookup
.do_8:
    mov     x4, 1
    b       .post_lookup
.do_9:
    mov     x4, 9
    b       .post_lookup
.do_10:
    mov     x4, 5
    b       .post_lookup
.do_11:
    mov     x4, 13
    b       .post_lookup
.do_12:
    mov     x4, 3
    b       .post_lookup
.do_13:
    mov     x4, 11
    b       .post_lookup
.do_14:
    mov     x4, 7
    b       .post_lookup
.do_15:
    mov     x4, 15

.post_lookup:
    orr     x1, x1, x4
    lsr     x0, x0, 4
    add     x3, x3, 1
    cmp     x3, x6
    b.ne    .loop_body

    orr     x0, x1, x15
    ret
_start:
    mov x0 3
    mov x1 11
    call modinv
    print x0 d    // Should print 4

    mov x0 7
    mov x1 20
    call modinv
    print x0 d    // Should print 3

    mov x0 4
    mov x1 8
    call modinv
    print x0 d    // Should print -1

    mov x0 1
    mov x1 17
    call modinv
    print x0 d    // Should print 1

    mov x0 157
    mov x1 561
    call modinv
    print x0 d    // Should print 268

    mov x0 16
    mov x1 17
    call modinv
    print x0 d    // Should print 16

    mov x0 2
    mov x1 5
    call modinv
    print x0 d    // Should print 3
    ret

modinv:
    // Save initial values
    mov x15 0    // Zero register
    add x2 x0 x15  // x2 = a
    add x3 x1 x15  // x3 = m
    add x20 x0 x15 // x20 = old_r
    add x21 x1 x15 // x21 = r

    mov x4 1   // old_s
    mov x5 0   // s
    mov x6 0   // old_t
    mov x7 1   // t

euclid_loop:
    cmp x21 0
    b.eq done

    // Compute quotient: x9 = x20 / x21
    add x8 x20 x15  // dividend
    mov x9 0       // quotient

div_loop:
    cmp x8 x21
    b.lt div_done
    sub x8 x8 x21
    add x9 x9 1
    b div_loop

div_done:
    // x8 now contains remainder
    // Save old values before updates
    add x10 x21 x15  // temp = r
    add x11 x5 x15   // temp_s = s
    add x12 x7 x15   // temp_t = t

    // Update r: r = old_r - q * r
    add x20 x21 x15    // old_r = r
    add x21 x8 x15     // r = remainder

    // Update s: s = old_s - q * s
    mov x16 0        // Initialize product
    add x17 x9 x15   // Copy quotient
s_mul_loop:
    cmp x17 0
    b.eq s_mul_done
    add x16 x16 x5
    sub x17 x17 1
    b s_mul_loop
s_mul_done:
    add x5 x4 x15     // s = old_s
    sub x5 x5 x16     // s = s - q*old_s
    add x4 x11 x15    // old_s = temp_s

    // Update t: t = old_t - q * t
    mov x16 0        // Initialize product
    add x17 x9 x15   // Copy quotient
t_mul_loop:
    cmp x17 0
    b.eq t_mul_done
    add x16 x16 x7
    sub x17 x17 1
    b t_mul_loop
t_mul_done:
    add x7 x6 x15     // t = old_t
    sub x7 x7 x16     // t = t - q*old_t
    add x6 x12 x15    // old_t = temp_t

    b euclid_loop

done:
    cmp x20 1
    b.ne no_inverse

    // Normalize s to be positive
normalize_loop:
    cmp x4 0
    b.ge normalize_done
    add x4 x4 x3
    b normalize_loop

normalize_done:
    add x0 x4 x15
    ret

no_inverse:
    mov x0 0
    mov x1 1
    sub x0 x0 x1
    ret
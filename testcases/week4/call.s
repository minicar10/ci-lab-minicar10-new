start:
    mov x1, 1000
    mov x0, 100
    call add_two_nums
    print x0, d
    ret

add_two_nums:
    add x0, x0, x1
    ret

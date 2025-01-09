mov x0, 41
mov x1, 43
cmp x0, x1
b.gt done

sub x1, x1, 1
cmp x1, x0
b.gt done
add x1, x1, x1

done:
    print x1, d
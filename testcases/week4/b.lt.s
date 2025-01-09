mov x0, 41
mov x1, 43
cmp x1, x0
b.lt done

sub x1, x1, 1
cmp x0, x1      
b.lt done
add x1, x1, x1     // Double if we get here

done:
    print x1, d
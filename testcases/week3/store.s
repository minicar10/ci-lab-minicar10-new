mov x0 0x0123456789abcdef
mov x1 0xfedcba9876543210
store x1 0 8

// test storing the correct number of bytes
store x0 0 1
load x2 0 8
print x2 x

store x0 0 2
load x2 0 8
print x2 x

store x0 0 4
load x2 0 8
print x2 x

store x0 0 8
load x2 0 8
print x2 x
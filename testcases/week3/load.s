mov x0 0x0123456789abcdef
store x0 0 8

// test loading the correct number of bytes
load x1 0 1
print x1 x

load x1 0 2
print x1 x

load x1 0 4
print x1 x

load x1 0 8
print x1 x
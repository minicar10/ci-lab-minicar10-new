// Simple tests for the and instruction.
add x0, x0, 0b101010
add x1, x1, 0b111100
add x2, x2, 0b010101
add x3, x3, 0b101010
add x4, x4, 0xabcd

and x2, x0, x2
and x3, x0, x3
and x4, x1, x4

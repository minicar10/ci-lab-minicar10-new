mov x1 42
b helper
mov x1 0xdeadbeef

helper:
    // Correct: 42
    print x1, d
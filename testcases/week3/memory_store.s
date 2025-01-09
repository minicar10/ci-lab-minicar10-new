put "Hello" 100
put ", World!" 200
store x9 300 4
store x10 400 4
mov x0 100
mov x1 200
mov x2 300
mov x3 400
print x0 s
print x1 s
print x2 d
print x3 d
put "Concatenation Test" 500
put " - Success!" 600
mov x4 500
mov x5 600
print x4 s
print x5 s
put "Overwrite Test" 100
store x11 300 4
mov x0 100
mov x2 300
print x0 s
print x2 d
print x6 d
put "EdgeCaseBoundary" 1020
store x1 1023 1
mov x7 1020
mov x8 1023
print x7 s
print x8 d

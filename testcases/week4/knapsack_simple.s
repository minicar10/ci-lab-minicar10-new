// Written By: Prithvi Jamadagni

// first we prep the knapsack weights/vals.
// this would normally be in the .data section.
// we will have 16 objects, so 16 weights and 16 values
// we will set a knapsack capacity (max weight) of 37

// variable allocations:
// x0 is a temporary register
// x1 has a pointer to the beginning of weights
// x2 has a pointer to the beginning of values
// x3 is a temp register
// x4 is our inner loop counter [37 -> 0]
// x5 is a pointer to the dp sliding window array
// 
// x8 is a "zero" register, used wherever we need the value 0.
// x9 is a temporary register, used wherever needed


//initialize data

    mov x1 100 // index into weights
    mov x2 200 // index into values

// weights: 
// 8,9,6,1,9,5,1,3,5,4,6,6,6,2,2,6
    mov x0 0x0301050901060908
    store x0 x1 8
    add x1 x1 8
    mov x0 0x0602020606060405
    store x0 x1 8

// values:
// 7,2,6,0,5,4,5,2,3,2,8,2,6,5,8,5
    mov x0 0x0205040500060207
    store x0 x2 8
    add x2 x2 8
    mov x0 0x0508050602080203
    store x0 x2 8



// we will have our sliding window (38 shorts) starting at address 500
// we will assume it is 0'd out already

// reset x5 to address of dp array
    mov x5 500

// prep outer loop, j
    mov x1 100 // index into weights
    mov x2 200 // index into values
	b .j_loop_chk
.j_loop_body:
	// set up i loop
	mov x4, 38
	b .i_loop_chk

.i_loop_body:	
	sub	x4 x4 1                 // decrement i to actual value
    lsl x5 x4 1 
    add x5 x5 500               // x5 is address of dp[i]

	// load weights[j] into x0
	load x0 1 x1
	// if i < weights[j], skip this iteration
    cmp x4 x0
	b.lt .i_loop_chk		// if i < weights[j] continue
    

	// else, dp[i] = max(dp[i], dp[i - weights[j]] + values[j]);


	// first get dp[i - weights[j]] + values[j]
    lsl x0 x0 1         // x0 is an integer, but we need to multiply by 2 due to pointer arith
    sub x0 x5 x0
	load x0 2 x0        // x0 has dp[i - weights[j]], now we need to add values[j]
	load x3 1 x2
    add x0 x0 x3
	// now x0 has dp[i - weights[j]] + values[j]

	// now we need dp[i]
	// let's get that into x6
	load x6 2 x5

	// now we compare, then conditionally select
	cmp x0 x6
	b.gt .keep_x0
	add x0 x6 0
.keep_x0:
	store x0 x5 2

.i_loop_chk:
	cmp x4 0
	b.ne	.i_loop_body

	// increment index into weights and values
	add	x1 x1 1
    add x2 x2 1

.j_loop_chk:
	cmp x1 116
	b.lt .j_loop_body

	// return the last element of the dp array
    load x0 2 574
	print x0 d
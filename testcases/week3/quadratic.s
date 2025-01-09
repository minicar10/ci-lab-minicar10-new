    // Load constants
    mov    x0, 5       
    mov    x1, 3  
    mov    x2, 4   

    // Compute A^2 using shifts and adds
    lsl     x3, x0, 1    
    add     x3, x3, x0   
    add     x3, x3, x0    
    add     x3, x3, x0
    add     x3, x3, x0

    // Compute B^2 using shifts and adds
    lsl     x4, x1, 1    
    add     x4, x4, x1   
    add     x4, x4, x1   
    add     x4, x4, x1
    add     x4, x4, x1

    // Compute A^2 + B^2
    add     x5, x3, x4   

    // Compute (A^2 + B^2) - C
    sub     x6, x5, x2    

    print x6, d
.data 
    fin: .asciiz "input_matrix.txt"
    fout: .asciiz "output_matrix.txt"
    buffer_read: .space 1024
    buffer_write: .space 1024
    num_buf: .space 10
    .align 2 # 4B
    # --- predefined variables
    image: .word 0
    kernel: .word 0
    out: .word 0
    # --- predefined variables
    N: .word 0
    M: .word 0
    p: .word 0
    s: .word 0
.text 
    # cap phat bo nho dong
    li $v0, 9
    li $a0, 196
    syscall 
    la $t0, image
    sw $v0, 0($t0)
    li $v0, 9
    li $a0, 64
    syscall 
    la $t0, kernel
    sw $v0, 0($t0)
    li $v0, 9
    li $a0, 196
    syscall 
    la $t0, out
    sw $v0, 0($t0)
    # Open file to read
    li $v0, 13
    la $a0, fin
    li $a1, 0
    li $a2, 0
    syscall 
    move $s6, $v0
    # Read file content into buffer_read
    li $v0, 14
    move $a0, $s6
    la $a1, buffer_read
    li $a2, 2048
    syscall 
    # read N
    lb $t0, 0($a1)
    subi $t0, $t0, 48
    la $t1, N
    sw $t0, 0($t1)
    # read M
    lb $t0, 2($a1)
    subi $t0, $t0, 48
    la $t1, M
    sw $t0, 0($t1)
    # read p
    lb $t0, 4($a1)
    subi $t0, $t0, 48
    la $t1, p
    sw $t0, 0($t1)
    # read s
    lb $t0, 6($a1)
    subi $t0, $t0, 48
    la $t1, s
    sw $t0, 0($t1)
    
    # parsing
    la $t0, buffer_read
    addi $t0, $t0, 9

    la $t3, image
    lw $t2, 0($t3) # address of 'image'
    j parse
    
load_kernel:
    la $t3, kernel
    lw $t2, 0($t3) # address of 'kernel'
    j skip

parse:
    lb $t1, 0($t0)
    beq $t1, 0, parse_space
    beq $t1, 10, load_kernel
    beq $t1, 13, parse_space
    beq $t1, ' ', parse_space
    beq $t1, '-', parse_minus
    beq $t1, '.', parse_dot
    mul $s0, $s0, 10
    subi $t1, $t1, 48
    add $s0, $s0, $t1
    j skip
parse_minus:
    # dat boi so la -1
    li $t9, -1
    mtc1 $t9, $f30
    cvt.s.w $f30, $f30
    j skip
parse_dot:
    # cong vao $f2
    ori $s7, $s7, 1 # so co dau phay
    mtc1 $s0, $f2
    cvt.s.w $f2, $f2
    li $s0, 0
    j skip
parse_space:
    andi $t8, $s7, 1
    bne $t8, $zero, parse_space_float
parse_space_integer:  
    # xu li so nguyen
    mtc1 $s0, $f2
    cvt.s.w $f2, $f2
    j add_points
parse_space_float:
    # xu li phan thap phan
    mtc1 $s0, $f4
    cvt.s.w $f4, $f4
divide:
    li $t8, 1
    mtc1 $t8, $f28
    cvt.s.w $f28, $f28
    c.lt.s $f4, $f28
    bc1t add_points
    li $t8, 10
    mtc1 $t8, $f26
    cvt.s.w $f26, $f26
    div.s $f4, $f4, $f26
    j divide
add_points:
    add.s $f2, $f2, $f4 # $f4 la phan thap phan
    mul.s $f2, $f2, $f30 # $f30 la -1 hoac 1
    swc1 $f2, 0($t2)
    addi $t2, $t2, 4
    # reset boi so ve 1
    li $t9, 1
    mtc1 $t9, $f30
    cvt.s.w $f30, $f30
    # reset $f2
    li $t9, 0
    mtc1 $t9, $f2
    cvt.s.w $f2, $f2
    # reset $f4
    mov.s $f4, $f2
    # reset $s0
    li $s0, 0
    # clear flag dau phay
    li $t8, 1
    not $t8, $t8
    and $s7, $s7, $t8
skip:
    lb $t3, 0($t0)
    addi $t0, $t0, 1
    bne $t3, 0, parse
    
solve:
    la $t0, p
    lw $t1, 0($t0) # padding
    sub $a0, $zero, $t1 # y (y-coordinate of kernel on image) - padding
    move $a1, $a0 # x (x-coordinate of kernel on image) - padding
    la $t9, out
    lw $t8, 0($t9)
    li $s5, 0 # ans count
next_iteration:
    li $t2, 0 # i (kernel[i][...])
    li $t3, 0 # j (kernel[...][j])
    li $t4, 0 # partial sum
    mtc1 $t4, $f30
    cvt.s.w $f30, $f30
    jal calculate_element
    
    la $t6, s
    lw $t7, 0($t6)
    add $a1, $a1, $t7 # add stride
    la $t4, N
    lw $t5, 0($t4)
    add $t5, $t5, $t1 # N + padding
    blt $a1, $t5, next_iteration
    # x = N - padding
    add $a0, $a0, $t7 # add stride
    sub $a1, $zero, $t1
    blt $a0, $t5, next_iteration
    # x = N + padding && y = N + padding (end)
    j print
    
calculate_element:
    # kernel[i][j] = kernel[i * M + j]
    la $t4, M # t4 = &M
    lw $t5, 0($t4) # t5 = M
    bge $t3, $t5, skip_adding # bao ve bien
    bge $t2, $t5, skip_adding # bao ve bien
    mul $t4, $t2, $t5 # t4 = i * M
    add $t4, $t4, $t3 # t4 = i * M + j
    li $t5, 4
    mul $t4, $t4, $t5
    # t6 = kernel[i][j]
    la $t5, kernel # t5 = kernel
    lw $t6, 0($t5)
    add $t5, $t6, $t4 # t5 = &kernel[i][j]
    lwc1 $f0, 0($t5)
    # image[y + i][x + j]
    la $t4, N # t4 = &N
    lw $t5, 0($t4) # t5 = N
    add $t4, $a0, $t2 # t4 = y + i
    bge $t4, $t5, skip_adding # bao ve bien => t4 >= t5 -> padding => img[][] = 0
    blt $t4, $zero, skip_adding # bao ve bien => t4 < 0
    mul $t4, $t4, $t5 # t4 = (y + i) * N
    li $t6, 0
    add $t6, $t6, $a1
    add $t4, $t4, $a1 # t4 = (y + i) * N + x
    add $t6, $t6, $t3
    add $t4, $t4, $t3 # t4 = (y + i) * N + x + j (t6 = x + j)
    blt $t6, $zero, skip_adding # bao ve bien => t6 < 0
    bge $t6, $t5, skip_adding # bao ve bien => t6 >= t5 -> padding => img[][] = 0
    li $t5, 4
    mul $t4, $t4, $t5
    la $t5, image # t5 = image
    lw $t6, 0($t5)
    add $t5, $t6, $t4 # t5 = &image[y + i][x + j]
    lwc1 $f2, 0($t5)
    mul.s $f4, $f2, $f0 # f4 = image[y + i][x + j] * kernel[i][j]
    add.s $f30, $f30, $f4 # f30 += f4
skip_adding:
    addi $t3, $t3, 1
    la $t4, N
    lw $t5, 0($t4)
    add $t5, $t5, $t1
    blt $t3, $t5, next_calculate_iteration
    # t3 = t5 (j = N + padding)
    addi $t2, $t2, 1
    li $t3, 0
    blt $t2, $t5, next_calculate_iteration
    # i = N + padding && j = N + padding (end)
    swc1 $f30, 0($t8)
    addi $s5, $s5, 1
    addi $t8, $t8, 4
    jr $ra
next_calculate_iteration:
    j calculate_element
    
print:
    la $t0, out
    lw $t1, 0($t0)
    li $t8, 0 # ans count
    la $t9, buffer_write
print_loop:
    lwc1 $f0, 0($t1)
    beq $t8, $s5, exit
    beq $t8, 0, skip_space
    li $t2, ' '
    sb $t2, 0($t9)
    addi $t9, $t9, 1
skip_space:
    li $t2, 0
    mtc1 $t2, $f2
    cvt.s.w $f2, $f2
    c.lt.s $f0, $f2
    bc1f no_minus_sign
    li $t2, '-'
    sb $t2, 0($t9)
    addi $t9, $t9, 1
    li $t2, -1
    mtc1 $t2, $f2
    cvt.s.w $f2, $f2
    mul.s $f0, $f0, $f2
    
no_minus_sign:   
    cvt.w.s $f2, $f0 # f0 la dap an, f2 la phan nguyen
    mfc1 $a0, $f2
    mfc1 $t7, $f2
    jal count_ready
    li $t2, '.'
    sb $t2, 0($t9)
    addi $t9, $t9, 1
    mtc1 $t7, $f6
    cvt.s.w $f6, $f6
    sub.s $f4, $f0, $f6 # f4 = f0 - f2
    li $t2, 10000
    mtc1 $t2, $f2
    cvt.s.w $f2, $f2
    mul.s $f2, $f2, $f4
    cvt.w.s $f2, $f2
    mfc1 $a0, $f2
    jal count_ready
    
    addi $t1, $t1, 4
    addi $t8, $t8, 1
    j print_loop
    
count_ready:
    la $t2, num_buf
    li $t3, 0
count_loop:
    addi $t3, $t3, 1
    li $t4, 10
    div $a0, $t4
    mflo $a0 # quotient
    mfhi $t4 # remainder
    addi $t4, $t4, 48
    sb $t4, 0($t2)
    addi $t2, $t2, 1
    la $t5, num_buf
    beq $a0, 0, count_write
    j count_loop
count_write:
    subi $t2, $t2, 1
    lb $t6, 0($t2)
    sb $t6, 0($t9)
    addi $t9, $t9, 1
    bne $t5, $t2, count_write
    jr $ra
    
exit:
    li $v0, 13
    la $a0, fout
    li $a1, 1
    li $a2, 0
    syscall
    move $s6, $v0
    
    li $v0, 15
    move $a0, $s6
    la $a1, buffer_write
    sub $a2, $t9, $a1
    syscall
    
    li $v0, 16
    move $a0, $s6
    syscall
    
    li $v0, 10
    syscall
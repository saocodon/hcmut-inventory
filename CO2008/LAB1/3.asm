.data 
    pa: .asciiz "Insert a: "
    pb: .asciiz "Insert b: "
    pc: .asciiz "Insert c: "
    pd: .asciiz "Insert d: "
    pf: .asciiz "F = "
    pr: .asciiz " remainder "
    a: .word 0
    b: .word 0
    c: .word 0
    d: .word 0

.text 
    # User input
    li $v0, 4
    la $a0, pa
    syscall
    li $v0, 5
    syscall 
    sw $v0, a
    
    li $v0, 4
    la $a0, pb
    syscall
    li $v0, 5
    syscall 
    sw $v0, b
    
    li $v0, 4
    la $a0, pc
    syscall
    li $v0, 5
    syscall 
    sw $v0, c
    
    li $v0, 4
    la $a0, pd
    syscall
    li $v0, 5
    syscall 
    sw $v0, d
    
    lw $t0, a
    lw $t1, b
    lw $t2, c
    lw $t3, d
    subi $t9, $t0, 10 # a - 10
    add $t8, $t1, $t3 # b + d
    mul $t7, $t9, $t8 # (a-10)(b+d)
    sub $t9, $t2, $t0 # c - a
    sub $t9, $t9, $t0 # c - 2 * a
    mul $t8, $t9, $t7 # (a-10)(b+d)(c-2*a)
    add $t4, $t0, $t1 # a + b
    add $t4, $t4, $t2 # a + b + c
    div $t8, $t4
    # print result
    mflo $t5
    mfhi $t6
    li $v0, 4
    la $a0, pf
    syscall
    li $v0, 1
    add $a0, $t5, $zero
    syscall
    li $v0, 4
    la $a0, pr
    syscall
    li $v0, 1
    add $a0, $t6, $zero
    syscall

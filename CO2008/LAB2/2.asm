.data 
    a_prm: .asciiz "a = "
    b_prm: .asciiz "b = "
    gcd_prm: .asciiz "GCD = "
    lcm_prm: .asciiz ", LCM = "
.text 
.globl main

main:
    li $v0, 4
    la $a0, a_prm
    syscall 
    li $v0, 5
    syscall 
    move $s0, $v0
    li $v0, 4
    la $a0, b_prm
    syscall 
    li $v0, 5
    syscall 
    move $s1, $v0 # b
    li $v0, 4
    la $a0, gcd_prm
    syscall 
    jal gcd
    move $s7, $v0
    move $a0, $s7
    li $v0, 1
    syscall
    li $v0, 4
    la $a0, lcm_prm
    syscall 
    mul $t0, $s0, $s1
    div $t0, $s7
    mflo $a0
    li $v0, 1
    syscall
    li $v0, 10
    syscall
    
gcd:
    # base case
    beq $s0, 0, gcd_base_b
    beq $s1, 0, gcd_base_a
    beq $s0, $s1, gcd_base_a
    bgt $s0, $s1, recursive_a
    j recursive_b
    
gcd_base_b:
    move $v0, $s1
    jr $ra

gcd_base_a:
    move $v0, $s0
    jr $ra
    
recursive_a:
    # save $ra, $s0, $s1 on stack
    addi $sp, $sp, -12
    sw $ra, 8($sp)
    sw $s0, 4($sp)
    sw $s1, 0($sp)
    # compute gcd(a - b, b)
    sub $s0, $s0, $s1
    jal gcd
    # restore $ra, $s0, $s1 from stack
    lw $s1, 0($sp)
    lw $s0, 4($sp)
    lw $ra, 8($sp)
    addi $sp, $sp, 12
    # return to caller
    jr $ra
    
recursive_b:
    # save $ra, $s0, $s1 on stack
    addi $sp, $sp, -12
    sw $ra, 8($sp)
    sw $s0, 4($sp)
    sw $s1, 0($sp)
    # compute gcd(a, b - a)
    sub $s1, $s1, $s0
    jal gcd
    # restore $ra, $s0, $s1 from stack
    lw $s1, 0($sp)
    lw $s0, 4($sp)
    lw $ra, 8($sp)
    addi $sp, $sp, 12
    # return to caller
    jr $ra
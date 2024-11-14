.data 
    nums: .space 20
    num_prompt: .ascii "Please insert element "
.text 
.globl main    

main:
    li $t0, 0 # index
    la $t9, nums # pointer nums
    beq $t8, 0, input
    beq $t8, 1, solve

input:
    or $t8, $t8, 1 # set bit (inputted)

    li $v0, 4 # print a string
    la $a0, num_prompt
    syscall 
    
    li $v0, 1 # print an integer
    addi $a0, $t0, 1
    syscall 
    
    li $v0, 11 # print a character
    li $a0, 58 # :
    syscall
    li $a0, 32 # space
    syscall
    
    li $v0, 5 # read an integer
    syscall # read into $v0
    
    sw $v0, 0($t9)
    addi $t9, $t9, 4
    addi $t0, $t0, 1
    
    beq $t0, 5, main
    j input

solve:
    or $t8, $t8, 2 # set bit (solved)
    
    lw $t1, 0($t9)
    
    # solve
    li $t3, 4
    div $t1, $t3 # lo = quotient, hi = remainder
    mflo $t3
    mfhi $t4
    beq $t4, 0, du0
    beq $t4, 1, du12
    beq $t4, 2, du12
    beq $t4, 3, du3
    
du12:
    mul $t5, $t3, 4
    sw $t5, 0($t9)
    j print

du3:
    addi $t3, $t3, 1
    mul $t5, $t3, 4
    sw $t5, 0($t9)
    j print
    
du0:
    add $t5, $t1, $zero
    
print:
    li $v0, 1
    add $a0, $t5, $zero
    syscall 
    
    beq $t0, 4, skip_comma
    
    li $v0, 11
    li $a0, 44 # ,
    syscall
    li $a0, 32
    syscall
    
skip_comma:   
    addi $t9, $t9, 4
    addi $t0, $t0, 1
    
    bne $t0, 5, solve
    
exit:
    li $v0, 10
    syscall
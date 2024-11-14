.data 
    nums: .space 60
    nums_prompt: .asciiz "Please insert element "
    second_max_prompt: .asciiz "Second largest value is "
    indices_prompt: .asciiz ", found in index "
.text 
.globl main

main:
    li $t1, 0 # init max
    li $s0, 0 # init max2
    li $t0, 0
    la $t9, nums
    
input:
    li $v0, 4
    la $a0, nums_prompt
    syscall
    li $v0, 1
    addi $a0, $t0, 1
    syscall
    li $v0, 11
    li $a0, ':'
    syscall 
    li $a0, ' '
    syscall 
    li $v0, 5
    syscall 
    sw $v0, 0($t9) 
    bgt $v0, $t1, skip1 # max2 < max < $v0
    blt $v0, $t1, check2 # max2 < $v0 < max
    j next
check2:
    bgt $v0, $s0, skip2 # combine conditions
    j next
skip1:
    move $s0, $t1
    move $t1, $v0
    j next
skip2:
    move $s0, $v0
next:
    addi $t0, $t0, 1
    addi $t9, $t9, 4
    bne $t0, 15, input
    
print_max:
    or $s7, $s7, 2
    li $v0, 4
    la $a0, second_max_prompt
    syscall 
    li $v0, 1
    move $a0, $s0
    syscall
    li $v0, 4
    la $a0, indices_prompt
    syscall
    # reset variables
    li $t0, 0
    la $t9, nums
    
find:    
    lw $t2, 0($t9)
    bne $s0, $t2, skip_label
    # $t2 = $s0
    beq $t3, 0, skip_comma # count = 0 -> skip , (only print the number)
    li $v0, 11
    li $a0, ','
    syscall 
    li $a0, ' '
    syscall

skip_comma:
    addi $t3, $t3, 1            
    li $v0, 1
    move $a0, $t0
    syscall      
    
skip_label:
    addi $t0, $t0, 1
    addi $t9, $t9, 4
    bne $t0, 15, find
    j exit

exit:
    li $v0, 10
    syscall
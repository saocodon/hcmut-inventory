.data 
    nums: .space 40
    nums_prompt: .asciiz "Please insert an element: "
    ans_prompt: .asciiz "The sorted array is: "
.text 
.globl main

main:
    li $t0, 0             
    addi $t1, $t0, 1      
    la $t9, nums          
    beq $t8, 0, input     
    beq $t8, 1, loop_i    
    beq $t8, 2, print     
    j exit                

input:
    li $v0, 4             
    la $a0, nums_prompt
    syscall 
    li $v0, 5             
    syscall 
    sw $v0, 0($t9)        
    addi $t9, $t9, 4      
    addi $t0, $t0, 1      
    bne $t0, 10, input    
    li $t8, 1             
    j main                

loop_i:
    li $t0, 0             
outer_loop:
    addi $t1, $t0, 1      
inner_loop:
    la $t7, nums
    mul $t6, $t0, 4
    add $t7, $t7, $t6
    lw $t5, 0($t7)
    la $t6, nums
    mul $t4, $t1, 4
    add $t6, $t6, $t4
    lw $t4, 0($t6)
    bge $t5, $t4, no_swap
    move $t3, $t5        
    move $t5, $t4        
    move $t4, $t3        
no_swap:
    sw $t5, 0($t7)       
    sw $t4, 0($t6)       
    addi $t1, $t1, 1     
    bne $t1, 10, inner_loop 
    addi $t0, $t0, 1     
    bne $t0, 9, outer_loop 
    li $t8, 2            
    j main               

print:
    li $v0, 4
    la $a0, ans_prompt
    syscall
    li $t0, 0             
    la $t9, nums          
print_loop:
    li $v0, 1
    lw $a0, 0($t9)        
    syscall 
    bge $t0, 9, no_comma 
    li $v0, 11
    li $a0, ','           
    syscall 
    li $a0, ' '           
    syscall 
no_comma:
    addi $t9, $t9, 4      
    addi $t0, $t0, 1      
    bne $t0, 10, print_loop 
    
exit:
    li $v0, 10
    syscall

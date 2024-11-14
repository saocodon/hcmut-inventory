.data 
    str: .space 100
    freq: .byte 100
.text 
    li $v0, 8 # read string
    la $a0, str
    li $a1, 100
    syscall 
    
    la $t0, str
read_string:
    lb $t1, 0($t0)
    beq $t1, 0, freq_sort
    
    la $t2, freq
    add $t2, $t2, $t1
    lb $t3, 0($t2)
    addi $t3, $t3, 1
    sb $t3, 0($t2)
    
    addi $t0, $t0, 3
    j read_string

freq_sort:
    li $t0, 0
outer_loop:
    beq $t0, 100, exit
    addi $t0, $t0, 1
    la $t1, freq
    addi $t1, $t1, 32
    li $t2, 32
inner_loop:
    beq $t2, 128, outer_loop
    lb $t3, 0($t1)
    bne $t3, $t0, not_print
    
    li $v0, 11
    
    beq $t4, 0, skip_semicolon        
    li $a0, ';'
    syscall 
    li $a0, ' '
    syscall    
    
skip_semicolon:
    addi $t4, $t4, 1
    add $a0, $t2, $zero
    syscall
    li $a0, ','
    syscall 
    li $a0, ' '
    syscall 
    li $v0, 1
    add $a0, $t3, $zero
    syscall 
    
not_print:
    addi $t1, $t1, 1
    addi $t2, $t2, 1
    j inner_loop

exit:
    li $v0, 10
    syscall
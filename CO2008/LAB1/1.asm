.data 
    prompt: .asciiz "Hello, "
    name: .space 32

.text 
    # get user input
    li $v0, 8
    la $a0, name
    li $a1, 32
    syscall
run:
    lb $t1, 0($a0)
    beq $t1, $zero, done
    beq $t1, 10, strip
    addi $a0, $a0, 1
    j run
strip:
    li $t2, '.'
    sb $t2, 0($a0)
done:
    # print prompt
    li $v0, 4
    la $a0, prompt
    syscall 
    # print name
    la $a0, name
    syscall  

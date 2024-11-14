.data 
    pa: .asciiz "Insert a:"
    pb: .asciiz "Insert b:"
    sa: .asciiz "a + b = "
    sb: .asciiz "\na - b = "
    a: .word 0
    b: .word 0

.text 
    # print prompt a
    li $v0, 4
    la $a0, pa
    syscall 
    # get a
    li $v0, 5
    syscall
    sw $v0, a
    # print prompt b
    li $v0, 4
    la $a0, pb
    syscall 
    # get b
    li $v0, 5
    syscall
    sw $v0, b
    # print prompt sum a
    li $v0, 4
    la $a0, sa
    syscall
    # load into memory
    lw $t0, a
    lw $t1, b
    # calculate
    add $a0, $t0, $t1
    # print first result
    li $v0, 1
    syscall
    # print prompt sum a
    li $v0, 4
    la $a0, sb
    syscall
    # calculate
    sub $a0, $t0, $t1
    # print first result
    li $v0, 1
    syscall
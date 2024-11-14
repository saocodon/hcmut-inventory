.data
    ps: .asciiz "Please enter a positive integer less than 16: "
    pa: .asciiz "Its binary form is: "
.text 
    li $v0, 4
    la $a0, ps
    syscall 
    li $v0, 5
    syscall 
    add $t0, $v0, $zero
    li $v0, 4
    la $a0, pa
    syscall
    li $v0, 1 
    # get bit 3
    srl $a0, $t0, 3
    syscall 
    # get bit 2
    and $a0, $t0, 4
    srl $a0, $a0, 2
    syscall
    # get bit 1
    and $a0, $t0, 2
    srl $a0, $a0, 1
    syscall 
    # get bit 0
    and $a0, $t0, 1
    syscall 
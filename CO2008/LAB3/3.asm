.data 
    p1: .asciiz "Number of points within the circle: "
    p2: .asciiz "/50000\nCalculated PI number: "
    fout: .asciiz "pi.txt"
    buf: .space 81
    num_buf: .space 10

.text
    la $t7, buf
    
    li $v0, 13
    la $a0, fout
    li $a1, 1
    li $a2, 0
    syscall
    move $s6, $v0

loop:
    beq $t3, 50000, write
    addi $t3, $t3, 1 # i
    li $a1, 200  # max bound, r = 100
    li $v0, 42
    syscall
    move $t0, $a0 # x
    subi $t0, $t0, 100 # x - 100
    syscall
    move $t1, $a0 # y
    subi $t1, $t1, 100 # y - 100
    mul $t0, $t0, $t0 # (x-100)^2
    mul $t1, $t1, $t1 # (y-100)^2
    add $t2, $t0, $t1 # (x-100)^2+(y-100)^2
    blt $t2, 10000, update # (x-100)^2 + (y-100)^2 < 100^2 = 10000
    j loop
    
update:
    addi $t9, $t9, 1 # count
    mul $t8, $t9, 4 # 4 * count
    mtc1 $t3, $f2 # i
    mtc1 $t8, $f4 # 4 * count
    div.s $f12, $f4, $f2
    j loop
    
write:
    beq $t4, 0, m1
    beq $t4, 1, count
    beq $t4, 2, m2
    beq $t4, 3, pi
    
m1:
    la $t5, p1
m1_loop:
    lb $t6, 0($t5)
    beq $t6, 0, count
    sb $t6, 0($t7)
    addi $t5, $t5, 1
    addi $t7, $t7, 1
    j m1_loop
    
count:
    move $a0, $t9
    jal count_ready

m2:
    la $t5, p2
m2_loop:
    lb $t6, 0($t5)
    beq $t6, 0, pi
    sb $t6, 0($t7)
    addi $t5, $t5, 1
    addi $t7, $t7, 1
    j m2_loop
    
pi:
    cvt.w.s $f6, $f12
    mfc1 $a0, $f6
    mfc1 $s5, $f6
    jal count_ready
    li $t5, '.'
    sb $t5, 0($t7)
    addi $t7, $t7, 1
    mtc1 $s5, $f16
    cvt.s.w $f16, $f16
    sub.s $f8, $f12, $f16
    li $t5, 1000000
    mtc1 $t5, $f14
    cvt.s.w $f14, $f14
    mul.s $f8, $f8, $f14
    cvt.w.s $f8, $f8
    mfc1 $a0, $f8
    jal count_ready
    j write_file
    
count_ready:
    la $s0, num_buf
    li $s1, 0
count_loop:
    addi $s1, $s1, 1
    li $t6, 10
    div $a0, $t6
    mflo $a0 # quotient
    mfhi $t6 # remainder
    addi $t6, $t6, 48
    sb $t6, 0($s0)
    addi $s0, $s0, 1
    la $t5, num_buf
    beq $a0, 0, count_write
    beq $s1, 6, count_write
    j count_loop
count_write:
    subi $s0, $s0, 1
    lb $t6, 0($s0)
    sb $t6, 0($t7)
    addi $t7, $t7, 1
    bne $t5, $s0, count_write
    jr $ra
    
write_file:
    li $v0, 15            # Syscall to write
    move $a0, $s6         # File descriptor
    la $a1, buf
    sub $a2, $t7, $a1
    syscall
    
exit:
    li $v0, 16
    move $a0, $s6         # File descriptor
    syscall

    li $v0, 10
    syscall

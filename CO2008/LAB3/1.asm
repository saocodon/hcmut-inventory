.data 
    insert_u: .asciiz "Please insert u: "
    insert_v: .asciiz "Please insert v: "
    insert_a: .asciiz "Please insert a: "
    insert_b: .asciiz "Please insert b: "
    insert_c: .asciiz "Please insert c: "
    insert_d: .asciiz "Please insert d: "
    insert_e: .asciiz "Please insert e: "

.text 
.globl main

main:
    # insert u
    li $v0, 4
    la $a0, insert_u
    syscall
    li $v0, 6
    syscall 
    mov.s $f2, $f0
    # insert v
    li $v0, 4
    la $a0, insert_v
    syscall
    li $v0, 6
    syscall 
    mov.s $f4, $f0
    # insert a
    li $v0, 4
    la $a0, insert_a
    syscall
    li $v0, 6
    syscall 
    mov.s $f6, $f0
    # insert b
    li $v0, 4
    la $a0, insert_b
    syscall
    li $v0, 6
    syscall 
    mov.s $f8, $f0
    # insert c
    li $v0, 4
    la $a0, insert_c
    syscall
    li $v0, 6
    syscall 
    mov.s $f10, $f0
    # insert d
    li $v0, 4
    la $a0, insert_d
    syscall
    li $v0, 6
    syscall 
    mov.s $f12, $f0
    # insert e
    li $v0, 4
    la $a0, insert_e
    syscall
    li $v0, 6
    syscall 
    mov.s $f14, $f0
    
    # calculate a/7
    li $t0, 7
    mtc1 $t0, $f0
    cvt.s.w $f0, $f0
    div.s $f16, $f6, $f0
    # calculate b/6
    li $t0, 6
    mtc1 $t0, $f0
    cvt.s.w $f0, $f0
    div.s $f18, $f8, $f0
    # calculate c/2
    li $t0, 2
    mtc1 $t0, $f0
    cvt.s.w $f0, $f0
    div.s $f20, $f10, $f0
    # calculate u^2-v^2
    mov.s $f22, $f2
    mul.s $f22, $f22, $f2
    mov.s $f24, $f4
    mul.s $f24, $f24, $f4
    sub.s $f22, $f22, $f24
    # calculate c/2 * (u^2-v^2)
    mul.s $f20, $f20, $f22
    # calculate u^6-v^6
    mov.s $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mov.s $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    sub.s $f22, $f22, $f24
    # calculate b/6 * (u^6-v^6)
    mul.s $f18, $f18, $f22
    # calculate u^7-v^7
    mov.s $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mul.s $f22, $f22, $f2
    mov.s $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    mul.s $f24, $f24, $f4
    sub.s $f22, $f22, $f24
    # calculate a/7 * (u^7-v^7)
    mul.s $f16, $f16, $f22
    # calculate sum of above 3s
    add.s $f16, $f16, $f18
    add.s $f16, $f16, $f20
    # calculate d^4 + e^3
    mov.s $f22, $f12
    mul.s $f22, $f22, $f12
    mul.s $f22, $f22, $f12
    mul.s $f22, $f22, $f12
    mov.s $f24, $f14
    mul.s $f24, $f24, $f14
    mul.s $f24, $f24, $f14
    add.s $f22, $f22, $f24
    # divide by d^4 + e^3
    div.s $f12, $f16, $f22
    li $v0, 2
    syscall
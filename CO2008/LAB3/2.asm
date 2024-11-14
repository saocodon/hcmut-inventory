.data 
    fin: .asciiz "raw_input.txt"
    fout: .asciiz "formatted_output.txt"
    title: .asciiz "---Student personal information---"
    name_prompt: .asciiz "\nName: "
    id_prompt: .asciiz "\nID: "
    address_prompt: .asciiz "\nAddress: "
    age_prompt: .asciiz "\nAge: "
    religion_prompt: .asciiz "\nReligion: "
    
    buffer_read: .space 100
    buffer_parse: .space 100
    buffer_write: .space 200

.text 
.globl main

main:
    # Open file to read
    li $v0, 13
    la $a0, fin
    li $a1, 0
    li $a2, 0
    syscall
    move $s6, $v0  # File descriptor

    # Read file content into buffer_read
    li $v0, 14
    move $a0, $s6
    la $a1, buffer_read
    li $a2, 100
    syscall

    # Initialize parsing variables
    la $t0, buffer_read  # Pointer to read buffer
    la $t2, buffer_parse
    li $t3, 0            # Field index counter
    li $t4, 0             # Initialize field size

parse:
    lb $t1, 0($t0)        # Load byte from buffer_read
    beq $t1, ',', comma   # Check if the current character is a comma
    beq $t1, 0, comma     # If end of buffer, go to print

    # If not a comma, store character in buffer_parse
    sb $t1, 0($t2)
    addi $t4, $t4, 1         # Increment field offset
    addi $t2, $t2, 1         # Move to the next position in buffer_parse
    j skip

comma:
    # Null-terminate the current field in buffer_parse
    sb $zero, 0($t2)      
    
    # Allocate memory dynamically based on the number of characters in $t4
    addi $a0, $t4, 1        # Allocate space for characters plus null terminator
    li $v0, 9
    syscall                 # Allocate memory
    move $t5, $v0           # Save allocated address temporarily in $t5
    li $t4, 0             # Initialize field size

    # Store allocated memory in the appropriate register based on field index
    beq $t3, 0, save_name
    beq $t3, 1, save_id
    beq $t3, 2, save_address
    beq $t3, 3, save_age
    beq $t3, 4, save_religion
    j next_field

save_name:
    move $s0, $t5
    j memcpy
save_id:
    move $s1, $t5
    j memcpy
save_address:
    move $s2, $t5
    j memcpy
save_age:
    move $s3, $t5
    j memcpy
save_religion:
    move $s4, $t5

memcpy:
    # Copy characters from buffer_parse into dynamically allocated memory
    la $t8, buffer_parse   # Source address (buffer_parse)
    move $t9, $t5          # Destination address (allocated memory)

copy_loop:
    lb $t7, 0($t8)         # Load byte from buffer_parse
    beq $t7, 0, next_field # If null terminator, end copy
    sb $t7, 0($t9)         # Store byte in destination memory
    addi $t8, $t8, 1       # Move to the next byte in buffer_parse
    addi $t9, $t9, 1       # Move to the next byte in allocated memory
    j copy_loop

next_field:
    beq $t1, 0, print_result
    li $t4, 0               # Reset field size
    la $t2, buffer_parse    # Reset parse buffer pointer
    addi $t3, $t3, 1        # Increment field index
    addi $t0, $t0, 1        # Move to next character in buffer_read
    j parse                 # Continue parsing

skip:
    addi $t0, $t0, 1         # Move to next character in buffer_read
    j parse                  # Continue parsing

print_result:
    li $v0, 4
    la $a0, title
    syscall 
    la $a0, name_prompt
    syscall 
    move $a0, $s0
    syscall
    la $a0, id_prompt
    syscall 
    move $a0, $s1
    syscall 
    la $a0, address_prompt
    syscall 
    move $a0, $s2
    syscall 
    la $a0, age_prompt
    syscall 
    move $a0, $s3
    syscall 
    la $a0, religion_prompt
    syscall 
    move $a0, $s4
    syscall
    
    li $v0, 13
    la $a0, fout
    li $a1, 1
    li $a2, 0
    syscall 
    move $s6, $v0
    
    la $t7, buffer_write
    li $t6, 0 # buffer size
    li $t5, 0
    
ready:
    addi $t5, $t5, 1
    beq $t5, 1, load_title
    beq $t5, 2, load_name_prompt
    beq $t5, 3, load_name
    beq $t5, 4, load_id_prompt
    beq $t5, 5, load_id
    beq $t5, 6, load_address_prompt
    beq $t5, 7, load_address
    beq	$t5, 8, load_age_prompt
    beq $t5, 9, load_age
    beq $t5, 10, load_religion_prompt
    beq $t5, 11, load_religion
    j exit

load_title:
    la $t9, title
    j write
    
load_name_prompt:
    la $t9, name_prompt
    j write
    
load_name:
    move $t9, $s0
    j write
    
load_id_prompt:
    la $t9, id_prompt
    j write
    
load_id:
    move $t9, $s1
    j write
    
load_address_prompt:
    la $t9, address_prompt
    j write
    
load_address:
    move $t9, $s2
    j write
    
load_age_prompt:
    la $t9, age_prompt
    j write
    
load_age:
    move $t9, $s3
    j write
    
load_religion_prompt:
    la $t9, religion_prompt
    j write

load_religion:
    move $t9, $s4
    j write
    
write:
    lb $t8, 0($t9)
    beq $t8, 0, ready
    sb $t8, 0($t7)
    addi $t9, $t9, 1
    addi $t7, $t7, 1
    addi $t6, $t6, 1
    j write

exit:
    li $v0, 15
    move $a0, $s6
    la $a1, buffer_write
    move $a2, $t6
    syscall
      
    # Close the file
    li $v0, 16
    move $a0, $s6
    syscall

    # Exit program
    li $v0, 10
    syscall

.macro push %x
sw %x, 0($fp)
addiu $fp, $fp, 4
.end_macro

.macro pop %x
addiu $fp, $fp, -4
lw %x, 0($fp)
.end_macro

.macro scanf_int
li $v0, 5
syscall
.end_macro

.macro scanf_char
li $v0, 12
syscall
.end_macro

.macro printf_line
la $a0, slabeln
li $v0, 4
syscall
.end_macro
.macro printf_string
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
.end_macro

.macro printf_int
li $v0, 1
syscall
.end_macro

.macro printf_char
li $v0, 11
syscall
.end_macro

.macro flabel_a
addiu $sp, $sp, -1024
li $t8, 97
sw $t8, 4($sp)
lw $v1, 4($sp)
addiu $fp, $fp, -4
addiu $sp, $sp, 1024
.end_macro

.data
fplabel:
.space 10240
gplabel:
.space 40960
slabelb:
.asciiz ""
slabeln:
.asciiz "\n"
__str0:
.asciiz " "
__str1:
.asciiz " "

.text
la $fp, fplabel
la $gp, gplabel
jal flabel_main
flabel_main:
addiu $sp, $sp, -1024
li $t8, 99
sw $t8 0($sp)
lw $t8, 0($sp)
sw $t8, 8($sp)
lw $a0, 8($sp)
printf_char
printf_line
li $t8, 100
sw $t8, 4($sp)
la $a3, __str0
printf_string
lw $t8, 4($sp)
sw $t8, 8($sp)
lw $a0, 8($sp)
printf_char
printf_line
li $t8, 99
sw $t8, 8($sp)
lw $a0, 8($sp)
printf_char
printf_line
la $a3, __str1
printf_string
li $t8, 100
sw $t8, 8($sp)
lw $a0, 8($sp)
printf_char
printf_line
push $0
flabel_a
sw $v1, 12($sp)
lw $t8, 12($sp)
sw $t8, 16($sp)
lw $t8, 16($sp)
sw $t8, 8($sp)
lw $a0, 8($sp)
printf_char
printf_line

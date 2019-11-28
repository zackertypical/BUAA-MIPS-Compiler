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

.data
fplabel:
.space 10240
gplabel:
.space 40960
slabelb:
.asciiz ""
slabeln:
.asciiz "\n"

.text
la $fp, fplabel
la $gp, gplabel
li $t8, 1
sw $t8 0($gp)
jal flabel_main
flabel_main:
addiu $sp, $sp, -1024
li $t8, 2
sw $t8 0($gp)
lw $t8, 0($gp)
sw $t8, 4($sp)
lw $a0, 4($sp)
printf_int
printf_line

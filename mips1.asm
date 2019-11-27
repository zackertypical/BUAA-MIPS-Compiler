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

.macro flabel_merge
addiu $sp, $sp, -1024
lw $t8, -20($fp)
sw $t8, 0($sp)
lw $t8, -16($fp)
sw $t8, 4($sp)
lw $t8, -12($fp)
sw $t8, 8($sp)
lw $t8, -8($fp)
sw $t8, 12($sp)
li $t8, 0
sw $t8, 20($sp)
li $t8, 0
sw $t8, 24($sp)
lw $t8, 0($sp)
sw $t8, 28($sp)
label_0:
lw $t8, 20($sp)
sw $t8, 32($sp)
lw $t8, 4($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
bgez $t8, label_1
lw $t8, 24($sp)
sw $t8, 32($sp)
lw $t8, 12($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
bgez $t8, label_2
lw $t8, 0($sp)
lw $t9, 20($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 44($sp)
sw $t8, 32($sp)
lw $t8, 8($sp)
lw $t9, 24($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 44($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
blez $t8, label_3
lw $t8, 8($sp)
lw $t9, 24($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 28($sp)
lw $t9, 44($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 4000
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 24($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 24($sp)
j label_4
label_3:
lw $t8, 0($sp)
lw $t9, 20($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 28($sp)
lw $t9, 44($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 4000
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 20($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 20($sp)
label_4:
lw $t8, 28($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 28($sp)
j label_5
label_2:
label_6:
lw $t8, 20($sp)
sw $t8, 32($sp)
lw $t8, 4($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
bgez $t8, label_7
lw $t8, 0($sp)
lw $t9, 20($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 28($sp)
lw $t9, 44($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 4000
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 28($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 28($sp)
lw $t8, 20($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 20($sp)
j label_6
label_7:
label_5:
j label_0
label_1:
label_8:
lw $t8, 24($sp)
sw $t8, 32($sp)
lw $t8, 12($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
bgez $t8, label_9
lw $t8, 8($sp)
lw $t9, 24($sp)
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 0($gp)
lw $t9, 40($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 44($sp)
lw $t8, 28($sp)
lw $t9, 44($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 4000
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 28($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 28($sp)
lw $t8, 24($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 24($sp)
j label_8
label_9:
lw $t8, 0($sp)
sw $t8, 20($sp)
label_11:
lw $t8, 20($sp)
sw $t8, 32($sp)
lw $t8, 28($sp)
sw $t8, 36($sp)
lw $t8, 32($sp)
lw $t9, 36($sp)
sub $t8, $t8, $t9
bgez $t8, label_10
lw $t8, 4000($gp)
lw $t9, 20($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 4000
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 40($sp)
lw $t8, 20($sp)
lw $t9, 40($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 0
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 20($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 20($sp)
j label_11
label_10:
addiu $fp, $fp, -20
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
.asciiz "start = "
__str1:
.asciiz "length = "
__str2:
.asciiz "1start = "
__str3:
.asciiz "1mid = "
__str4:
.asciiz "1length = "
__str5:
.asciiz "2start+mid = "
__str6:
.asciiz "2length-mid = "

.text
la $fp, fplabel
la $gp, gplabel
jal flabel_main
flabel_inner_merge_sort:
addiu $sp, $sp, -1024
sw $ra, 8($sp)
lw $t8, -12($fp)
sw $t8, 0($sp)
lw $t8, -8($fp)
sw $t8, 4($sp)
la $a3, __str0
printf_string
lw $t8, 0($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
la $a3, __str1
printf_string
lw $t8, 4($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
lw $t8, 4($sp)
sw $t8, 24($sp)
li $t8, 1
sw $t8, 28($sp)
lw $t8, 24($sp)
lw $t9, 28($sp)
sub $t8, $t8, $t9
bgtz $t8, label_12
lw $ra, 8($sp)
addiu $fp, $fp, -12
addiu $sp, $sp, 1024
jr $ra
j label_13
label_12:
label_13:
lw $t8, 4($sp)
sw $t8, 24($sp)
li $t8, 2
sw $t8, 28($sp)
lw $t8, 24($sp)
lw $t9, 28($sp)
sub $t8, $t8, $t9
bnez $t8, label_14
lw $t8, 0($gp)
lw $t9, 0($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 24($sp)
lw $t8, 0($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 0($gp)
lw $t9, 32($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 36($sp)
lw $t8, 36($sp)
sw $t8, 28($sp)
lw $t8, 24($sp)
lw $t9, 28($sp)
sub $t8, $t8, $t9
blez $t8, label_15
lw $t8, 0($gp)
lw $t9, 0($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 12($sp)
lw $t8, 0($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 0($gp)
lw $t9, 32($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 36($sp)
lw $t8, 0($sp)
lw $t9, 36($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 0
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 0($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
lw $t9, 12($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 0
add $t8, $t8, $gp
sw $t9, 0($t8)
j label_16
label_15:
label_16:
lw $ra, 8($sp)
addiu $fp, $fp, -12
addiu $sp, $sp, 1024
jr $ra
j label_17
label_14:
label_17:
lw $t8, 4($sp)
li $t9, 2
div $t8, $t9
mflo $t8
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 16($sp)
la $a3, __str2
printf_string
lw $t8, 0($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
la $a3, __str3
printf_string
lw $t8, 16($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
la $a3, __str4
printf_string
lw $t8, 4($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
lw $t8, 0($sp)
sw $t8, 40($sp)
lw $t8, 40($sp)
push $t8
lw $t8, 16($sp)
sw $t8, 44($sp)
lw $t8, 44($sp)
push $t8
push $0
jal flabel_inner_merge_sort
sw $v1, 48($sp)
lw $t8, 48($sp)
sw $t8, 52($sp)
lw $t8, 52($sp)
sw $t8, 56($sp)
la $a3, __str5
printf_string
lw $t8, 0($sp)
lw $t9, 16($sp)
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
la $a3, __str6
printf_string
lw $t8, 4($sp)
lw $t9, 16($sp)
sub $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 20($sp)
lw $a0, 20($sp)
printf_int
printf_line
lw $t8, 0($sp)
lw $t9, 16($sp)
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 40($sp)
lw $t8, 40($sp)
push $t8
lw $t8, 4($sp)
lw $t9, 16($sp)
sub $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 44($sp)
lw $t8, 44($sp)
push $t8
push $0
jal flabel_inner_merge_sort
sw $v1, 48($sp)
lw $t8, 48($sp)
sw $t8, 52($sp)
lw $t8, 52($sp)
sw $t8, 56($sp)
lw $t8, 0($sp)
sw $t8, 40($sp)
lw $t8, 40($sp)
push $t8
lw $t8, 16($sp)
sw $t8, 44($sp)
lw $t8, 44($sp)
push $t8
lw $t8, 0($sp)
lw $t9, 16($sp)
add $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 64($sp)
lw $t8, 64($sp)
push $t8
lw $t8, 4($sp)
lw $t9, 16($sp)
sub $t8, $t8, $t9
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 68($sp)
lw $t8, 68($sp)
push $t8
push $0
flabel_merge
sw $v1, 48($sp)
lw $t8, 48($sp)
sw $t8, 52($sp)
lw $t8, 52($sp)
sw $t8, 56($sp)
lw $ra, 8($sp)
addiu $fp, $fp, -12
addiu $sp, $sp, 1024
jr $ra

flabel_main:
addiu $sp, $sp, -1024
scanf_int
sw $v0, 0($sp)
li $t8, 0
sw $t8, 4($sp)
label_19:
lw $t8, 4($sp)
sw $t8, 12($sp)
lw $t8, 0($sp)
sw $t8, 16($sp)
lw $t8, 12($sp)
lw $t9, 16($sp)
sub $t8, $t8, $t9
bgez $t8, label_18
scanf_int
sw $v0, 8($sp)
lw $t8, 4($sp)
lw $t9, 8($sp)
sll $t8, $t8, 2
addiu $t8, $t8, 0
add $t8, $t8, $gp
sw $t9, 0($t8)
lw $t8, 4($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 4($sp)
j label_19
label_18:
li $t8, 0
sw $t8, 20($sp)
lw $t8, 20($sp)
push $t8
lw $t8, 0($sp)
sw $t8, 24($sp)
lw $t8, 24($sp)
push $t8
push $0
jal flabel_inner_merge_sort
sw $v1, 28($sp)
lw $t8, 28($sp)
sw $t8, 32($sp)
lw $t8, 32($sp)
sw $t8, 36($sp)
li $t8, 0
sw $t8, 4($sp)
label_21:
lw $t8, 4($sp)
sw $t8, 12($sp)
lw $t8, 0($sp)
sw $t8, 16($sp)
lw $t8, 12($sp)
lw $t9, 16($sp)
sub $t8, $t8, $t9
bgez $t8, label_20
lw $t8, 0($gp)
lw $t9, 4($sp)
sll $t9, $t9, 2
addiu $t9, $t9, 0
add $t9, $t9, $gp
lw $t8, 0($t9)
sw $t8, 40($sp)
lw $t8, 40($sp)
sw $t8, 44($sp)
lw $a0, 44($sp)
printf_int
printf_line
lw $t8, 4($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 4($sp)
j label_21
label_20:

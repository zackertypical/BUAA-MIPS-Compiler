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
.asciiz "Let's foo! p = "
__str1:
.asciiz "Let's fee! s = "
__str2:
.asciiz "Test char print\\n"
__str3:
.asciiz "Test char print\\n"
__str4:
.asciiz "End!"

.text
la $fp, fplabel
la $gp, gplabel
li $t8, 99
sw $t8 0($gp)
jal flabel_main
flabel_main:
addiu $sp, $sp, -1024
li $v0, 5
syscall
sw $v0, 4($gp)
li $t8, 118
sw $t8, 8($sp)
la $a3, __str2
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
la $a0, slabeln
li $v0, 4
syscall
lw $t8, 0($gp)
sw $t8, 12($sp)
lw $a0, 12($sp)
li $v0, 11
syscall
la $a0, slabeln
li $v0, 4
syscall
la $a3, __str3
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
la $a0, slabeln
li $v0, 4
syscall
lw $t8, 8($sp)
sw $t8, 12($sp)
lw $a0, 12($sp)
li $v0, 1
syscall
la $a0, slabeln
li $v0, 4
syscall
li $t8, 0
sw $t8, 0($sp)
label_1:
lw $t8, 0($sp)
sw $t8, 16($sp)
lw $t8, 4($gp)
sw $t8, 20($sp)
lw $t8, 16($sp)
lw $t9, 20($sp)
sub $t8, $t8, $t9
bgez $t8, label_0
lw $t8, 0($sp)
sw $t8, 12($sp)
lw $a0, 12($sp)
li $v0, 1
syscall
la $a0, slabeln
li $v0, 4
syscall
lw $t8, 0($sp)
sw $t8, 24($sp)
lw $t8, 24($sp)
sw $t8, 0($fp)
addiu $fp, $fp, 4
lw $t8, 4($gp)
sw $t8, 28($sp)
lw $t8, 28($sp)
sw $t8, 0($fp)
addiu $fp, $fp, 4
addiu $fp, $fp, 4
# inline function foo
addiu $sp, $sp, -1024
lw $t8, -12($fp)
sw $t8, 0($sp)
lw $t8, -8($fp)
sw $t8, 4($sp)
la $a3, __str0
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
lw $t8, 4($sp)
sw $t8, 12($sp)
lw $a0, 12($sp)
li $v0, 1
syscall
la $a0, slabeln
li $v0, 4
syscall
lw $t8, 0($sp)
sw $t8, 16($sp)
lw $v1, 16($sp)
addiu $fp, $fp, -12
addiu $sp, $sp, 1024
# inline end
sw $v1, 32($sp)
lw $t8, 32($sp)
sw $t8, 36($sp)
lw $t8, 36($sp)
sw $t8, 4($sp)
lw $t8, 4($sp)
sw $t8, 24($sp)
lw $t8, 24($sp)
sw $t8, 0($fp)
addiu $fp, $fp, 4
addiu $fp, $fp, 4
# inline function fee
addiu $sp, $sp, -1024
lw $t8, -8($fp)
sw $t8, 0($sp)
la $a3, __str1
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
lw $t8, 0($sp)
sw $t8, 8($sp)
lw $a0, 8($sp)
li $v0, 1
syscall
la $a0, slabeln
li $v0, 4
syscall
addiu $fp, $fp, -8
addiu $sp, $sp, 1024
# inline end
sw $v1, 32($sp)
lw $t8, 32($sp)
sw $t8, 36($sp)
lw $t8, 36($sp)
sw $t8, 44($sp)
lw $t8, 0($sp)
li $t9, 1
add $t8, $t8, $t9
sw $t8, 0($sp)
j label_1
label_0:
la $a3, __str4
move $a0, $a3
li $v0, 4
syscall
la $a3, slabelb
addiu $fp, $fp, 4
# inline function faa
addiu $sp, $sp, -1024
li $t8, 122
sw $t8 4($sp)
lw $t8, 4($sp)
sw $t8, 8($sp)
lw $v1, 8($sp)
addiu $fp, $fp, -4
addiu $sp, $sp, 1024
# inline end
sw $v1, 32($sp)
lw $t8, 32($sp)
sw $t8, 36($sp)
lw $t8, 36($sp)
sw $t8, 12($sp)
lw $a0, 12($sp)
li $v0, 11
syscall
la $a0, slabeln
li $v0, 4
syscall

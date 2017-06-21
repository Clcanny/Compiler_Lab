.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
f1:
sub $sp, $sp, 0
move $sp, $fp
lw $fp, -4($sp)
lw $t0, -12($fp)
move $v0, $t0
jr $ra
f2:
sub $sp, $sp, 4
sw $fp, -4($sp)
lw $t0, -12($fp)
sw $t0, -12($sp)
sw $ra, -8($fp)
move $fp, $sp
sub $sp, $sp, 12
jal f1
sw $t0, -16($fp)
lw $ra, -8($fp)
move $sp, $fp
lw $fp, -4($sp)
lw $t0, -16($fp)
move $v0, $t0
jr $ra
main:
move $fp, $sp
sw $fp, -4($fp)
sub $sp, $sp, 12
li $v0, 5
syscall
move $t0, $v0
sw $t0, -12($fp)
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -16($fp)
sw $fp, -4($sp)
lw $t0, -16($fp)
sw $t0, -12($sp)
sw $ra, -8($fp)
move $fp, $sp
sub $sp, $sp, 12
jal f2
sw $t0, -20($fp)
lw $ra, -8($fp)
lw $t0, -20($fp)
move $a0, $t0
li $v0, 1
syscall
move $sp, $fp
lw $fp, -4($sp)
lw $t0, -16($fp)
move $v0, $t0
jr $ra

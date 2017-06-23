.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
main:
move $fp, $sp
sw $fp, -4($fp)
sub $sp, $sp, 8
sub $sp, $sp, 56
li $t1, 0
move $t0, $t1
sw $t0, -12($fp)
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -16($fp)
li $t1, 1
move $t0, $t1
sw $t0, -20($fp)
lw $t1, -20($fp)
move $t0, $t1
sw $t0, -24($fp)
li $t1, 0
move $t0, $t1
sw $t0, -28($fp)
lw $t1, -28($fp)
move $t0, $t1
sw $t0, -32($fp)
li $v0, 5
syscall
move $t0, $v0
sw $t0, -36($fp)
lw $t1, -36($fp)
move $t0, $t1
sw $t0, -40($fp)
l1:
lw $t0, -32($fp)
lw $t1, -40($fp)
blt $t0, $t1, l4
li $t1, 0
move $t0, $t1
sw $t0, -44($fp)
j l5
l4:
li $t1, 1
move $t0, $t1
sw $t0, -44($fp)
l5:
lw $t0, -44($fp)
li $t1, 0
beq $t0, $t1, l3
lw $t0, -16($fp)
lw $t1, -24($fp)
add $t2, $t0, $t1
sw $t2, -48($fp)
lw $t1, -48($fp)
move $t0, $t1
sw $t0, -52($fp)
lw $t0, -24($fp)
move $a0, $t0
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
lw $t1, -24($fp)
move $t0, $t1
sw $t0, -16($fp)
lw $t1, -52($fp)
move $t0, $t1
sw $t0, -24($fp)
li $t1, 1
move $t0, $t1
sw $t0, -56($fp)
lw $t0, -32($fp)
lw $t1, -56($fp)
add $t2, $t0, $t1
sw $t2, -60($fp)
lw $t1, -60($fp)
move $t0, $t1
sw $t0, -32($fp)
j l1
l3:
li $t1, 0
move $t0, $t1
sw $t0, -64($fp)
move $sp, $fp
lw $t0, -64($fp)
lw $fp, -4($sp)
move $v0, $t0
jr $ra

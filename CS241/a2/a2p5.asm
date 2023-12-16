lis $3 ; $3 = 0xFFFF000C
.word 0xFFFF000C
lis $4 ; $4 = 10 (LFs)
.word 10
lis $5 ; $5 = 48 (0)
.word 48
add $29, $31, $0 ; $29 = $31 + $0

bne $1, $0, 3 ; if ($1 == $0) jump to label 3
sw $5, 0($3) ; MEM [$3 + 0] = $5
sw $4, 0($3) ; MEM [$3 + 0] = $4
jr $31 ; pc = $31

slt $6, $1, $0 ; $6 = 1 if $1 < $0; 0 otherwise
beq $6, $0, recurse ; if ($6 == $0) jump to label recurse
lis $6 ; $6 = -1
.word -1
lis $7 ; $6 = 45(-)
.word 45
mult $1, $6 ; hi:lo = $1 * $6
mflo $1 ; $1 =  lo
sw $7, 0($3) ; MEM [$3 + 0] = $7 

recurse:
    sw $1, -4($30) ; MEM [$30 + -4] = $1
    sw $7, -8($30) ; MEM [$30 + -8] = $7 
    sw $31, -12($30) ; MEM [$30 + -12] = $31 
    lis $6 ; $6 = 12 
    .word 12
    sub $30, $30, $6 ; $30 = $30 - $6
    
    beq $1, $0, end ; if ($1 == $0) jump to label end
    
    div $1, $4 ; lo = $1 / $4, hi = $1 % $4
    mflo $1 ; $1 =  lo
    mfhi $7 ; $7 =  hi

    lis $6 ; $6 = recurse 
    .word recurse
    jalr $6 ; $31 = pc and $pc = $6

    add $7, $7, $5 ; $7 = $7 + $5
    sw $7, 0($3) ; MEM [$3 + 0] = $7
    
end:
    lis $6 ; $6 = 12
    .word 12
    add $30, $30, $6 ; $30 = $30 + $6
    lw $1, -4($30) ; $1 = MEM [$30 + -4]
    lw $7, -8($30) ; $7 = MEM [$30 + -8]
    lw $31, -12($30) ; $31 = MEM [$30 + -12]
    bne $31, $29, 1 ; if ($31 =! $29) jump to label 1
    sw $4, 0($3) ; MEM [$3 + 0] = $4 
    jr $31 ; return to $31
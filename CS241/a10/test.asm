.import print
.import init
.import new
.import delete
lis $4
.word 4
lis $10
.word print
lis $11
.word 1
sub $29, $30, $4
sw $1, 0($29)
sub $30, $30, $4
sw $2, -4($29)
sub $30, $30, $4
lis $5
.word init
sw $31, -4($30)
sub $30, $30, $4
jalr $5
add $30, $30, $4
lw $31, -4($30)
sw $11, -8($29)
sub $30, $30, $4
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word new
jalr $5
add $30, $30, $4
lw $31, -4($30)
bne $3, $0, 1
add $3, $11, $0
sw $3, -8($29)
sw $1, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
jalr $10
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)
lw $3, -4($29)
add $30, $30, $4
add $30, $30, $4
add $30, $30, $4
jr $31

.import print
lis $4
.word 4
sub $29, $30, $4
sw $1, 0($29)
sub $30, $30, $4
sw $2, -4($29)
sub $30, $30, $4
sw $1, -4($30)
sub $30, $30, $4
lw $3, 0($29)
add $1, $3, $0
sw $31, -4($30)
sub $30, $30, $4
lis $5
.word print
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)
lw $3, 0($29)
add $30, $30, $4
add $30, $30, $4
jr $31

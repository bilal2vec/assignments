lis $4 ; $4 = 4 
.word 4
lis $5 ; $5 = 1 
.word 1
add $6, $0, $0 ; $6 = 0
add $7, $1, $0 ; $7 = $1

lw $3, 0($7) ; $3 = MEM [$6 + 0]
beq $2, $5, end ; if ($2 == 1) jump to end

add $6, $6, $5 ; $6 = $6 + 1
add $7, $7, $4 ; $7 = $7 + 4

loop:
    lw $8, 0($7) ; $8 = MEM [$7 + 0]

    slt $9, $3, $8 ; $9 = 1 if $3 < $8; 0 otherwise
    beq $9, $0, 1 ; if ($9 == $0) jump 1
    add $3, $8, $0; $3 = $8

    add $6, $6, $5 ; $6 = $6 + 1
    add $7, $7, $4 ; $7 = $7 + 4

    bne $6, $2, loop ; if ($6 =! $2) jump to label loop

end:
    jr $31          ; pc = $31
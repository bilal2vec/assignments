lis $4 ; $4 = 4 
.word 4
lis $5 ; $5 = 1 
.word 1
lis $6 ; $6 = 20 
.word 32
lis $7 ; $8 = 64
.word 64
lis $8 ; $8 = 0xFFFF000C
.word 0xFFFF000C

add $9, $0, $0 ; $9 = 0
add $10, $1, $0 ; $10 = $1

beq $2, $0, end ; if ($2 == $0) jump to label end

loop:
    lw $11, 0($10) ; $11 = MEM [$10 + 0]

    bne $11, $0, atoz ; if ($11 != $0) jump to label atoz
    zero:
        sw $6, 0($8) ; MEM [$8 + 0] = $6 
    beq $11, $0, zerotwo ; if ($11 == $0) jump to label zerotwo
    atoz:
        add $12, $7, $11 ; $12 = $7 + $11
        sw $12, 0($8) ; MEM [$8 + 0] = $12 

zerotwo:
    add $9, $9, $5 ; $9 = $9 + 1
    add $10, $10, $4 ; $10 = $10 + 4

    bne $9, $2, loop ; if ($9 =! $2) jump to label loop

end:
    jr $31          ; pc = $31
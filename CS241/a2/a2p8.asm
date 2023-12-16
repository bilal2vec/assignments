; save registers
sw $1, -4($30)
sw $2, -8($30)
sw $31, -12($30)
lis $4
.word 12
sub $30, $30, $4

add $8, $1, $0

lis $4
.word recurse
jalr $4

; load registers
lis $4
.word 12
add $30, $30, $4
lw $1, -4($30)
lw $2, -8($30)
lw $31, -12($30)
jr $31

recurse:
    ; save registers
    sw $1, -4($30)
    sw $2, -8($30)
    sw $5, -12($30)
    sw $6, -16($30)
    sw $7, -20($30)
    sw $31, -24($30)
    lis $4
    .word 24
    sub $30, $30, $4

    ; add 1 to current height
    lis $4 
    .word 1
    add $5, $5, $4

    ; load left and right children
    lw $6, 4($1)
    lw $7, 8($1)

    beq $6, $7, done    

    add $9, $1, $0

    lis $4 
    .word -1
    beq $6, $4, endleftchild
        lis $4 
        .word 4
        mult $6, $4
        mflo $6
        
        add $1, $8, $6
        
        lis $4
        .word recurse
        jalr $4    
    endleftchild:

    add $1, $9, $0

    lis $4 
    .word -1
    beq $7, $4, endrightchild
        lis $4 
        .word 4
        mult $7, $4
        mflo $7

        add $1, $8, $7

        lis $4
        .word recurse
        jalr $4    
    endrightchild:

    done:
        ;update max depth if necessary
        slt $4, $3, $5
        beq $4, $0, 1
        add $3, $5, $0

        ; load registers from stack
        lis $4 
        .word 24
        add $30, $30, $4        
        
        lw $1, -4($30)
        lw $2, -8($30)
        lw $5, -12($30)
        lw $6, -16($30)
        lw $7, -20($30)
        lw $31, -24($30)
        jr $31

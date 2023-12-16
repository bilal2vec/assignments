sw $31, -4($30)
lis $31
.word -4
add $30, $30, $31

beq $2, $0, realend2 ; if ($2 == $0) jump to label realend

lis $3 ; $3 = 4 
.word 4
lis $4 ; $4 = 1 
.word 1
lis $7 ; $7 =  print
.word print

add $5, $0, $0 ; $5 = $0 + $0
add $6, $1, $0 ; $6 = $1 + $0

loop:
    lw $1, 0($6) ; $1 = MEM [$6 + 0]    
    jalr $7 ; $31 = pc and $pc = $7
    

    add $5, $5, $4 ; $5 = $6 + $4
    add $6, $6, $3 ; $6 = $6 + $3        

    bne $5, $2, loop ; if ($5 =! $2) jump to label loop
    
realend2:
    lis $31
    .word 4
    add $30, $30, $31
    lw $31, -4($30)

    add $2, $0, $0 ; $2 = $0 + $0
    add $3, $0, $0 ; $2 = $0 + $0
    add $4, $0, $0 ; $2 = $0 + $0
    add $5, $0, $0 ; $2 = $0 + $0
    add $6, $0, $0 ; $2 = $0 + $0
    add $7, $0, $0 ; $2 = $0 + $0
    

    jr $31 ; pc = $31

; push $31 on stack
;sw $31, -4($30)
;lis $31
;.word -4
;add $30, $30, $31
;
;lis $1
;.word 12345
;lis $2
;.word print
;jalr $2 ; call procedure
;
;; pop $31 from stack
;lis $31
;.word 4
;add $30, $30, $31
;lw $31, -4($30)
;
;jr $31

; your code for print procedure should be added heres

    sw $1, -4($30) ; MEM [$30 + -4] = $1 
    sw $3, -8($30) ; MEM [$30 + -4] = $1 
;print:
;    sw $4, -12($30) ; MEM [$30 + -4] = $1 
;    sw $5, -16($30) ; MEM [$30 + -4] = $1 
;    sw $6, -20($30) ; MEM [$30 + -4] = $1 
;    sw $7, -24($30) ; MEM [$30 + -4] = $1 
;    sw $29, -28($30) ; MEM [$30 + -4] = $1 
;    lis $3 ; $3 = 28
;    .word 28
;    sub $30, $30, $3 ; $30 = $30 - $3
;    lis $3 ; $3 = 0xFFFF000C
;    .word 0xFFFF000C
;    lis $4 ; $4 = 10 (LFs)
;
;    .word 10
;    lis $5 ; $5 = 48 (0)
;    .word 48
;    add $29, $31, $0 ; $29 = $31 + $0
;    bne $1, $0, 3 ; if ($1 == $0) jump to label 3
;    sw $5, 0($3) ; MEM [$3 + 0] = $5
;    sw $4, 0($3) ; MEM [$3 + 0] = $4
;
;    jr $31 ; pc = $31
;    slt $6, $1, $0 ; $6 = 1 if $1 < $0; 0 otherwise
;    beq $6, $0, recurse ; if ($6 == $0) jump to label recurse
;    lis $6 ; $6 = -1
;
;    .word -1
;    lis $7 ; $6 = 45(-)
;    .word 45
;    mult $1, $6 ; hi:lo = $1 * $6
;    mflo $1 ; $1 =  lo
;    sw $7, 0($3) ; MEM [$3 + 0] = $7 
;    recurse:
;        sw $1, -4($30) ; MEM [$30 + -4] = $1
;        sw $7, -8($30) ; MEM [$30 + -8] = $7 
;
;        sw $31, -12($30) ; MEM [$30 + -12] = $31 
;        lis $6 ; $6 = 12 
;        .word 12
;        sub $30, $30, $6 ; $30 = $30 - $6
;        
;        beq $1, $0, end ; if ($1 == $0) jump to label end
;        
;        div $1, $4 ; lo = $1 / $4, hi = $1 % $4
;        mflo $1 ; $1 =  lo
;        mfhi $7 ; $7 =  hi
;        lis $6 ; $6 = recurse 
;        .word recurse
;        jalr $6 ; $31 = pc and $pc = $6
;
;        add $7, $7, $5 ; $7 = $7 + $5
;        sw $7, 0($3) ; MEM [$3 + 0] = $7
;        
;
;    end:
;        lis $6 ; $6 = 12
;        .word 12
;        add $30, $30, $6 ; $30 = $30 + $6
;        lw $1, -4($30) ; $1 = MEM [$30 + -4]
;        lw $7, -8($30) ; $7 = MEM [$30 + -8]
;        lw $31, -12($30) ; $31 = MEM [$30 + -12]
;        bne $31, $29, realend ; if ($31 =! $29) jump to label 1
;        sw $4, 0($3) ; MEM [$3 + 0] = $4 
;        lis $3
;        .word 28
;        add $30, $30, $3 ; $30 = $30 + $1
;        
;        lw $1, -4($30)
;        lw $3, -8($30)
;        lw $4, -12($30)
;        lw $5, -16($30)
;        lw $6, -20($30)
;        lw $7, -24($30)
;        lw $29, -28($30)
;        jr $31 ; pc = $31
;        realend:
;        jr $31 ; return to $31

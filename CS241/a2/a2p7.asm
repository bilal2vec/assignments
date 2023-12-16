; save registers to stack
;sw $1, -4($30)
;sw $2, -8($30)
;sw $31, -12($30)
;lis $4 
;.word 12
;sub $30, $30, $4
;
;lis $4
;.word stirling
;jalr $4
;
;; load registers from stack
;lis $4 
;.word 12
;add $30, $30, $4
;lw $1, -4($30)
;lw $2, -8($30)
;lw $31, -12($30)
;jr $31

;lis $4
;.word 4
;
;lis $5
;.word stirling
;
;add $1, $0, $0
;add $2, $0, $0
;
;sw $31, -4($30)
;sub $30, $30, $4
;
;jalr $5
;
;add $30, $30, $4
;lw $31, -4($30)
;jr $31

stirling:
    ; save registers to stack
    sw $1, -4($30)
    sw $2, -8($30)
    sw $4, -12($30)
    sw $5, -16($30)
    sw $31, -20($30)
    lis $4 
    .word 20
    sub $30, $30, $4

    ; n == k => 1
    beq $1, $2, neqk
    ; k == 0 => 0
    beq $2, $0, keq0
    ; n < k => 0
    slt $4, $1, $2
    lis $5
    .word 1
    beq $4, $5, nltk

    ; n = n - 1
    lis $4
    .word 1
    sub $1, $1, $4

    ; f(n - 1, k)    
    lis $4 
    .word stirling
    jalr $4

    ;$5 = (n - 1) * f(n - 1, k)
    mult $3, $1
    mflo $5

    ; k = k - 1
    lis $4
    .word 1
    sub $2, $2, $4

    ; f(n - 1, k - 1)    
    lis $4 
    .word stirling
    jalr $4
    
    ; (n - 1) * f(n - 1, k) + f(n - 1, k - 1) 
    add $3, $5, $3

    done:
        ; load registers from stack
        lis $4 
        .word 20
        add $30, $30, $4        
        
        lw $1, -4($30)
        lw $2, -8($30)
        lw $4, -12($30)
        lw $5, -16($30)
        lw $31, -20($30)
        jr $31

    neqk:
        lis $4
        .word 1
        add $3, $0, $4

        lis $4
        .word done
        jr $4
    
    keq0:
        add $3, $0, $0

        lis $4
        .word done
        jr $4
    
    nltk:
        add $3, $0, $0

        lis $4
        .word done
        jr $4s
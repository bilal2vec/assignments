add     $3, $1, $0		; $3 = $1 + $0
sltu    $4, $1, $2      ; $4 = 1 if $1 < $2; 0 otherwise
beq     $4, $0, 1       ; if ($4 == $0) jump to label 1
add     $3, $2, $0      ; $3 = $2 + $0
jr      $31				; return

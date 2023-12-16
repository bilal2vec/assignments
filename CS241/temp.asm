; reverse($1 = start, $2 = size)
; recursively reverses an array with starting address 'start' and size 'size'
reverse:
; define constants
lis $4
.word 4
lis $6
.word reverse

;add $0, $4, $0
;add $6, $4, $0

; save return address
sw $31, -4($30)
sub $30, $30, $4
; back up used registers
add $11, $1, $0
add $12, $2, $0
add $13, $3, $0
add $14, $4, $0
add $15, $5, $0
add $16, $6, $0
; compute end = start + 4 * size
mult $2, $4
mflo $2
add $2, $2, $1
; swap first and last element
lw $3, 0($1)    ; $3 = first element
lw $5, -4($2)   ; $5 = last element
sw $3, -4($2)   ; last element = $3
sw $5, 0($1)    ; first element = $5
; compute new values of 'start' and 'end'
add $1, $1, $4  ; start += 4
sub $2, $2, $4  ; end -= 4
slt $3, $1, $2  ; if start >= end, then slt returns 0
beq $3, $0, end ; skip to end if we're in the base case
; recursive call
sub $2, $2, $1  ; new size is (end - start) / 4
div $2, $4
mflo $2
jalr $6         ; reverse(start+4, size-2)
end:
; restore return address
add $30, $30, $4
lw $31, -4($30)
; restore used registers
add $1, $11, $0
add $2, $12, $0
add $3, $13, $0
add $4, $14, $0
add $5, $15, $0
add $6, $16, $0
jr $31
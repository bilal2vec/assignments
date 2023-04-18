| x |
x := 'findZeroBadly = ^r. bind (get r) ^x. if (= x 0) then (writeNum x) else bind (put r (- x 1)) ^ig. findZeroBadly r ; main = bind (ref 42000) ^r. findZeroBadly r' runHasntkell.
x displayNl.
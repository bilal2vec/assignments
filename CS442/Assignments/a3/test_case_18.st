| x |
x := 'refFac = ^i. ^o. bind (get i) ^x. bind (get o) ^y. if (= x 0) then writeNum y else bind (put i (- x 1)) ^ig. bind (put o (* x y)) ^ig. refFac i o; main = bind (ref 10) ^i. bind (ref 1) (refFac i)' runHasntkell.
x displayNl.
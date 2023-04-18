| x |
x := 'refFac = ^i. ^o. bind (get i) ^x. bind (get o) ^y. if (= x 0) then writeNum y else bind (put i (- x 1)) ^ig. bind (put o (* x y)) ^ig. refFac i o; refFacIn = bind readNum ^i. bind (ref i) ^i. bind (ref 1) ^o. refFac i o; refFacN = ^i. if (le i 1) then refFacIn else bind refFacIn ^ig. (refFacN (- i 1)); main = bind readNum refFacN' runHasntkell.
x displayNl.
| x |
x := 'two = ^f. ^x. f (f x); three = ^f. ^x. f (f (f x)); mul = ^m. ^n. ^f. m (n f); main = mul two three (^x. (+ x 1)) 0' runHasntkell.
x displayNl.
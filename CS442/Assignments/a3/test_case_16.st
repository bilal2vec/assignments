| x |
x := 'main = writeNum 42' runHasntkell.
x displayNl.
x := 'main = bind readNum (^x. writeNum (+ x 30))' runHasntkell.
x displayNl.
x := 'main = bind readNum writeNum' runHasntkell.
x displayNl.
x := 'echo = bind readNum (^x. writeNum (+ x 30)); main = bind echo (^x. echo)' runHasntkell.
x displayNl.
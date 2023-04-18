| x s l t |
x := LambdaParser parse: '^m.^n.^f.^x.m(n f)x'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l eta.
t displayNl.
t := l eta.
t displayNl.
x := LambdaParser parse: '^m.^n.^f.^x.n m f x'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l eta.
t displayNl.
t := l eta.
t displayNl.
t := l eta.
t displayNl.
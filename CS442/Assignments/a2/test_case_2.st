| x s l t |
x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
t := l nor.
t displayNl.
x := l nor dup.
x displayNl.
t := l toDeBruijn.
t displayNl.
l := Lambda new: x.
l displayNl.
t := l nor.
t displayNl.
x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
x displayNl.
l := Lambda new: x.
l displayNl.
x := l nor: 1000.
x displayNl.
s := x displayString.
s displayNl.
t := l toDeBruijn.
t displayNl.
x := LambdaParser parse: s , 'f x'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l nor: 1000.
t displayNl.
x := LambdaParser parse: '(^head.^tail.^cons.^zero.^two.(^succ.(^pred.(pred two)) (^n.(tail(n(^p.cons(succ(head p)) (head p))(cons zero zero))))) (^n.^f.^x.n f(f x))) (^l.l^x.^y.x) (^l.l^x.^y.y) (^h.^t.^s.s h t) (^f.^x.x) (^f.^x.f (f x)) f x'.l := Lambda new: x.
x displayNl.
x := l nor: 1000.
x displayNl.
t := l toDeBruijn.
t displayNl.
x := LambdaParser parse: '(^head.^tail.^cons.^isNull.^nil.^zero.^succ.(^Y.^F.(^len.(len (cons zero (cons zero nil)))) (Y F)) (^f.(^x.f(x x))(^x.f(x x))) (^f.^l.(isNull l) zero (succ (f(tail l))))) (^l.l(^x.^y.x)) (^l.l(^x.^y.y))(^h.^t.^s.s h t) (^l.l^h.^t.^x.^y.y) (^s.^x.^y.x) (^f.^x.x) (^n.^f.^x.n f(f x))'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l nor: 1000.
t displayNl.

| x s l |
x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
x := l aoe dup.
x displayNl.
t := l toDeBruijn.
t displayNl.
l := Lambda new: x.
l displayNl.
t := l aoe.
t displayNl.
x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
x displayNl.
l := Lambda new: x.
l displayNl.
x := l aoe: 1000.
x displayNl.
s := x displayString.
s displayNl.
t := l toDeBruijn.
t displayNl.
x := LambdaParser parse: s , 'f x'.
x displayNl.
l := Lambda new: x.
l displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
t := l aoe.
t displayNl.
x := LambdaParser parse: '(^head.^tail.^cons.^zero.^two.(^succ.(^pred.(pred two)) (^n.(tail(n(^p.cons(succ(head p))
(head p))(cons zero zero))))) (^n.^f.^x.n f(f x))) (^l.l^x.^y.x) (^l.l^x.^y.y) (^h.^t.^s.s h t) (^f.^x.x) (^f.^x.f
(f x)) f x'.
x displayNl.
l := Lambda new: x.
l displayNl.
x := l aoe: 1000.
x displayNl.
t := l toDeBruijn.
t displayNl.

| x s l |

x := LambdaParser parse: '(^f.f)x'.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.

x := LambdaParser parse: '(^f.^g. g f)x'.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.

x := LambdaParser parse: '(^f.^g. g f)g'.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.

x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.
'------------------------' displayNl.
l := Lambda new: x.
x := l aoe.
s := x displayString.
s displayNl.
'------------------------' displayNl.
x := LambdaParser parse: '(^mul.^two.mul two two) (^m.^n.^f.m(n f)) (^f.^x.f (f x))'.
l := Lambda new: x.
x := l aoe: 1000.
s := x displayString.
s displayNl.

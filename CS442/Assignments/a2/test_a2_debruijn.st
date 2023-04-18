| x s l |

x := LambdaParser parse: 'x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.f'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: 'f x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.f x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.x f'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.x x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.f f'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.f f x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.^x.f x'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.^x.x f'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^f.^x.f (f x)'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.

x := LambdaParser parse: '^a.^f.f a ^f.^x.f (f x) a'.
l := Lambda new: x.
x := l toDeBruijn.
s := x displayString.
s displayNl.
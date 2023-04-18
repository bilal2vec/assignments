| x s l |

x := LambdaParser parse: '(^f.((^f.(^x.(f (f x)))) ((^f.(^x.(f (f x)))) f)))'.
x displayNl.
l := Lambda new: x.
x := l nor.
s := x displayString.
s displayNl.

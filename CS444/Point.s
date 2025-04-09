; Point.s
section .text
global Point$$setX$I
Point$$setX$I:
	…
global Point$$moveX$I
Point$$moveX$I:
	…
global Point$$$
Point$$$:
	…

section .data
global Point
Point:
extern java$lang$Object$$getClass$
dd java$lang$Object$$getClass$
extern java$lang$Object$$clone$
dd java$lang$Object$$clone$
extern java$lang$Object$$hashCode$
dd java$lang$Object$$hashCode$
extern java$lang$Object$$toString$
dd java$lang$Object$$toString$
extern java$lang$Object$$equals$Ljava.lang.Object
dd java$lang$Object$$equals$Ljava.lang.Object
dd Point$$setX$I
dd Point$$moveX$I


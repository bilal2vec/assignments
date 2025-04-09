; ColoredPoint.s
section .text
global ColoredPoint$$getColor$
ColoredPoint$$getColor$:
	…
global ColoredPoint$$setX$I
ColoredPoint$$setX$I:
	…
global ColoredPoint$$$
ColoredPoint$$$:
	…

section .data
global ColoredPoint
ColoredPoint:
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
dd ColoredPoint$$setX$I
extern Point$$moveX$I
dd Point$$moveX$I
dd ColoredPoint$$getColor$

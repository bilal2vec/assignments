#include <stdio.h>
#include "matrix.h"

struct matrix matrixCreate(int a11, int a12, int a21, int a22)
{
    struct matrix temp = {a11, a12, a21, a22};
    return temp;
}

int determinant(struct matrix m)
{
    return (m.a * m.d) - (m.b * m.c);
}

int trace(struct matrix m)
{
    return m.a + m.d;
}
struct matrix transpose(struct matrix m)
{
    struct matrix temp = {m.a, m.c, m.b, m.d};

    return temp;
}
void matrixPrint(struct matrix m)
{
    printf("%d %d\n%d %d\n", m.a, m.b, m.c, m.d);
}
struct matrix matrixAdd(struct matrix m, struct matrix n)
{
    struct matrix temp = {m.a + n.a, m.b + n.b, m.c + n.c, m.d + n.d};
    return temp;
}
struct matrix matrixMultiply(struct matrix m, struct matrix n)
{
    int row1col1 = (m.a * n.a) + (m.b * n.c);
    int row1col2 = (m.a * n.b) + (m.b * n.d);
    int row2col1 = (m.c * n.a) + (m.d * n.c);
    int row2col2 = (m.c * n.b) + (m.d * n.d);

    struct matrix temp = {row1col1, row1col2, row2col1, row2col2};

    return temp;
}

struct matrix matrixHadamardProduct(struct matrix m, struct matrix n)
{
    struct matrix temp = {m.a * n.a, m.b * n.b, m.c * n.c, m.d * n.d};

    return temp;
}
struct matrix matrixInverse(struct matrix m)
{
    int det = determinant(m);

    if ((det == 0) || (m.a % det != 0) || (m.b % det != 0) || (m.c % det != 0) || (m.d % det != 0))
    {
        printf("Error: Matrix not invertible over the integers\n");
        struct matrix temp = {0, 0, 0, 0};
        return temp;
    }
    else
    {

        struct matrix temp = {m.d / det, -m.b / det, -m.c / det, m.a / det};

        return temp;
    }
}

// int main(void)
// {
//     struct matrix m = matrixCreate(1, 2, 3, 4);
//     printf("Determinant: %d \n", determinant(m));
//     printf("Trace: %d\n", trace(m));
//     matrixPrint(m);
//     printf("\n");
//     matrixPrint(transpose(m));
//     printf("\n");
//     struct matrix n = matrixCreate(5, 2, 2, 1);
//     struct matrix p = matrixCreate(0, 0, 3, 4);
//     matrixPrint(n);
//     printf("\n");
//     matrixPrint(matrixAdd(m, n));
//     printf("\n");
//     matrixPrint(matrixMultiply(m, n));
//     printf("\n");
//     matrixPrint(matrixHadamardProduct(m, n));
//     printf("\n");
//     matrixPrint(matrixInverse(m));
//     printf("\n");
//     matrixPrint(matrixInverse(n));
//     printf("\n");
//     matrixPrint(matrixInverse(p));
//     printf("\n");
// }

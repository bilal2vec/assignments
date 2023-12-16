#ifndef MATRIX_H
#define MATRIX_H

struct matrix
{
    int a;
    int b;
    int c;
    int d;
};

struct matrix matrixCreate(int a11, int a12, int a21, int a22);

int determinant(struct matrix m);
int trace(struct matrix m);
struct matrix transpose(struct matrix m);
void matrixPrint(struct matrix m);
struct matrix matrixAdd(struct matrix m, struct matrix n);
struct matrix matrixMultiply(struct matrix m, struct matrix n);
struct matrix matrixHadamardProduct(struct matrix m, struct matrix n);
struct matrix matrixInverse(struct matrix m);

#endif
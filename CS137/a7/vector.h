#ifndef VECTOR_H
#define VECTOR_H

struct vector
{               //vector is a structure with two elements
    int *arr;   // pointer to array
    int length; //vector is always full
};

struct vector *vectorCreate(void);

void vectorDestroy(struct vector *v);
void vectorAdd(struct vector *v, int value);
void vectorSet(struct vector *v, int index, int value);
int vectorGet(struct vector *v, int index);
int vectorLength(struct vector *v);
void vectorRemoveVal(struct vector *v, int value);
void vectorRemoveNth(struct vector *v, int n);

#endif
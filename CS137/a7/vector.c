#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "vector.h"

struct vector *vectorCreate(void)
{ // malloc returns NULL if it was not completed successfully
    struct vector *v = malloc(sizeof(struct vector));
    assert(v); // to check that malloc was completed successfully
    v->arr = NULL;
    v->length = 0; // no values in the vector yet.
    return v;
}

void vectorDestroy(struct vector *v)
{
    free(v);
}

void vectorAdd(struct vector *v, int value)
{
    int *temp = malloc(sizeof(int) * (v->length + 1));

    for (int i = 0; i < v->length; i++)
    {
        temp[i] = v->arr[i];
    }
    temp[v->length] = value;

    v->arr = temp;
    v->length += 1;
}

void vectorSet(struct vector *v, int index, int value)
{
    v->arr[index] = value;
}

int vectorGet(struct vector *v, int index)
{
    return v->arr[index];
}

int vectorLength(struct vector *v)
{
    return v->length;
}
void vectorRemoveVal(struct vector *v, int value)
{

    int *old = v->arr;

    int *temp = malloc(sizeof(int) * (v->length - 1));
    int tempidx = 0;
    int done = 0;

    for (int i = 0; i < v->length; i++)
    {
        int val = v->arr[i];

        if (val != value)
        {
            temp[tempidx] = val;
            tempidx += 1;
        } else {

            if (done == 1) {
                temp[tempidx] = val;
                tempidx += 1;
            }

            done = 1;
        }
    }

    v->arr = temp;
    free(old);
    if (done == 1) {
        v->length -= 1;
    }
}
void vectorRemoveNth(struct vector *v, int n)
{
    int *old = v->arr;
    int *temp = malloc(sizeof(int) * ((v->length) - 1));
    int tempidx = 0;
    int done = 0;

    for (int i = 0; i < v->length; i++)
    {
        if ((i + 1) != n)
        {
            temp[tempidx] = v->arr[i];
            tempidx += 1;
        } else {
            done = 1;
        }
    }

    v->arr = temp;
    free(old);

    if (done == 1) {
        v->length -= 1;
    }
}

// int main(void)
// {
//     struct vector *v = vectorCreate();
//     for (int i = 0; i < 12; ++i)
//     {
//         vectorAdd(v, i);
//     }
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n\n");
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         vectorSet(v, i, i * i);
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorRemoveVal(v, 36);
//     vectorRemoveVal(v, 121);
//     vectorRemoveVal(v, 200);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorSet(v, 5, 200);
//     vectorSet(v, 8, 200);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorRemoveVal(v, 200);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorRemoveNth(v, 1);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorRemoveNth(v, 4);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     printf("\n\n");
//     vectorRemoveNth(v, 20);
//     printf("%d\n\n", vectorLength(v));
//     for (int i = 0; i < vectorLength(v); ++i)
//     {
//         printf("v[%d]=%d  ", i, vectorGet(v, i));
//     }
//     vectorDestroy(v); // Make sure to check that there is no memory leak
//     return 0;
// }
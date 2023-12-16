#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int **split(int a[], int n, int p, int *n1, int *n2)
{

    *n1 = 0;
    *n2 = 0;

    for (int i = 0; i < n; i++)
    {
        int val = a[i];

        if (val <= p)
        {
            *n1 += 1;
        }
        else
        {
            *n2 += 1;
        }
    }

    int *arr1 = malloc(sizeof(int) * *n1);
    int *arr2 = malloc(sizeof(int) * *n2);

    int arr1idx = 0;
    int arr2idx = 0;

    for (int i = 0; i < n; i++)
    {
        int val = a[i];

        if (val <= p)
        {
            arr1[arr1idx] = val;
            arr1idx += 1;
        }
        else
        {
            arr2[arr2idx] = val;
            arr2idx += 1;
        }
    }

    int **temp = malloc(8 * 2);
    temp[0] = arr1;
    temp[1] = arr2;

    return temp;
}

// int main(void)
// {
//     int a[] = {9, 3, 2, 6, -1, 3, 6, 6, 7, 8, 5, 2, 3, 4, 1, 0};
//     int n = sizeof(a) / sizeof(a[0]);
//     int n1, n2;
//     int **ans = split(a, n, 5, &n1, &n2);
//     assert(n1 == 10);
//     assert(n2 == 6);
//     printf("First Array\n");
//     for (int i = 0; i < n1; i++)
//     {
//         printf("%d\n", ans[0][i]);
//     }
//     printf("Second Array\n");
//     for (int i = 0; i < n2; i++)
//     {
//         printf("%d\n", ans[1][i]);
//     }
//     free(ans[1]);
//     free(ans[0]);
//     free(ans);
//     return 0;
// }
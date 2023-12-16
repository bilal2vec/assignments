#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// bool isMult3(int n) { return n % 3 == 0; }

// void printarr(int *a, int len)
// {
//     for (int i = 0; i < len; ++i)
//         printf("%d ", a[i]);
//     printf("\n");
// }

void filter(int *a, int *n, bool (*f)(int))
{
    int tempn = *n;
    int new_elements = 0;
    for (int i = 0; i < tempn; i++)
    {
        if (f(a[i]) == false)
        {
            for (int j = (i + 1); j < tempn; j++)
            {
                if (f(a[j]) == true)
                {
                    int temp = a[i];
                    a[i] = a[j];
                    a[j] = temp;
                    new_elements += 1;
                    break;
                }
            }
        }
        else
        {
            new_elements += 1;
        }
    }

    a = realloc(a, new_elements);
    *n = new_elements;
}

// int main(void)
// {
//     int size = 10;
//     int *arr = malloc(size * sizeof(int));
//     for (int i = 0; i < size; ++i)
//         arr[i] = i * 7;
//     printarr(arr, size);
//     filter(arr, &size, isMult3);
//     printarr(arr, size);
//     assert(size == 4);
//     free(arr);
// }
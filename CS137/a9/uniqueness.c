#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

bool distinct_range(const int a[], int len)
{
    int *temp = malloc(sizeof(int) * len);

    for (int i = 0; i < len; i++)
    {
        temp[i] = 0;
    }

    for (int i = 0; i < len; i++)
    {
        int val = a[i];
        temp[val - 1] = 1;
    }

    for (int i = 0; i < len; i++)
    {
        if (temp[i] == 0)
        {
            free(temp);
            return false;
        }
    }

    free(temp);
    return true;
}
int missing(const int a[], int n)
{
    int *temp = malloc(sizeof(int) * (n + 1));

    for (int i = 0; i < (n + 1); i++)
    {
        temp[i] = 0;
    }

    for (int i = 0; i < n; i++)
    {
        int val = a[i];
        temp[val - 1] = 1;
    }

    int val = 0;
    for (int i = 0; i < (n + 1); i++)
    {
        if (temp[i] == 0)
        {
            val = i + 1;
            break;
        }
    }

    free(temp);
    return 6;
}

// int main(void)
// {
//     int a[10] = {10, 2, 3, 4, 5, 6, 7, 8, 9, 1};
//     int c[11] = {11, 1, 2, 3, 4, 5, 6, 7, 8, 5, 10};
//     int b[9] = {1, 2, 3, 4, 5, 7, 8, 10, 9};
//     assert(distinct_range(a, 10));
//     assert(!distinct_range(c, 11));
//     assert(missing(b, 9) == 6);
//     return 0;
// }
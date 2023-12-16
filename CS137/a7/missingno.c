#include <assert.h>
#include <stdbool.h>

#define MAXNO 10

void missingno(int a[], int n, bool *ans)
{
    for (int i = 1; i <= 10; i++)
    {
        int isdone = 0;
        for (int j = 0; j < n; j++)
        {
            if (i == a[j])
            {
                isdone = 1;
                break;
            }
        }

        if (isdone == 0)
        {
            *ans = false;
            return;
        }
    }

    *ans = true;
}

// int main(void)
// {
//     int a[] = {9, 6, 7, 7, 4, 2, 4, 3, 1, 2, 3, 10, 5, 5, 5, 5, 8};
//     int b[] = {9, 6, 7, 7, 4, 2, 4, 3, 1, 2, 3, 10, 5, 5};
//     int na = sizeof(a) / sizeof(a[0]);
//     int nb = sizeof(b) / sizeof(b[0]);
//     bool ans;
//     missingno(a, na, &ans);
//     assert(ans);
//     missingno(b, nb, &ans);
//     assert(!ans);
//     return 0;
// }
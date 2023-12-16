#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

bool newSearch(int arr[], int len, int x)
{
    int BB = 0;
    int BA = len - 1;
    printf("start with the range %d to %d\n", arr[BB], arr[BA]);

    while (true)
    {
        int first = arr[BB];
        int last = arr[BA];

        if ((x < first) || (x > last))
        {
            printf("%d not in the range between %d and %d\n", x, first, last);
            return false;
        }

        if (first == last)
        {
            if (first == x)
            {
                printf("%d was found in position %d\n", x, BB);
                return true;
            }
            else
            {
                printf("%d not in the range between %d and %d\n", x, first, last);
                return false;
            }
        }

        int pos = (int)(BB + ((float)(x - arr[BB]) / (float)(arr[BA] - arr[BB])) * (BA - BB));

        if (arr[pos] == x)
        {
            printf("%d was found in position %d\n", x, pos);
            return true;
        }

        else
        {
            if (x < arr[pos])
            {
                BA = pos - 1;
                printf("move to search in the range between %d and %d\n", arr[BB], arr[BA]);
            }
            else if (x > arr[pos])
            {
                BB = pos + 1;
                printf("move to search in the range between %d and %d\n", arr[BB], arr[BA]);
            }
        }
    }
}

// int main(void)
// {
//     int a[1] = {14};
//     assert(!newSearch(a, 1, 10));
//     printf("\n\n");
//     assert(newSearch(a, 1, 14));
//     printf("\n\n");
//     int b[13] = {1, 4, 5, 7, 12, 23, 44, 67, 89, 100, 120, 121, 122};
//     assert(!newSearch(b, 13, 0));
//     printf("\n\n");
//     assert(!newSearch(b, 13, 150));
//     printf("\n\n");
//     assert(newSearch(b, 13, 4));
//     printf("\n\n");
//     assert(newSearch(b, 13, 121));
//     printf("\n\n");
//     assert(newSearch(b, 13, 23));
//     printf("\n\n");

//     // int b[20] = {-10, -4, 5, 7, 12, 23, 44, 67, 89, 100, 120, 121, 122, 200, 210, 222, 300, 301, 303, 500};

//     // assert(!newSearch(b, 20, 55));
//     // printf("\n\n");
//     // assert(newSearch(b, 20, 12));
//     return 0;
// }
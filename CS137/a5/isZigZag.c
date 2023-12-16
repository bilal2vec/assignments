#include <stdio.h>
#include <assert.h>

int isZigZag(int array[], int n)
{
    if (n == 1)
    {
        return 1;
    }
    else if ((n == 2) && (array[0] != array[1]))
    {
        return 1;
    }
    else if (n > 2)
    {
        int isZig = 1;
        for (int i = 1; i < n - 1; ++i)
        {
            int prev = array[i - 1];
            int curr = array[i];
            int next = array[i + 1];

            int prevIncreasing = 0;
            if (curr > prev)
            {
                prevIncreasing = 1;
            }

            int nextIncreasing = 0;
            if (next > curr)
            {
                nextIncreasing = 1;
            }

            if (prevIncreasing == nextIncreasing)
            {
                return 0;
            }
        }
        return 1;
    }
    else
    {
        return 0;
    }
}

// int main(void)
// {
//     int testArray[5] = {-10, -1, -3, 10, 5};
//     assert(isZigZag(testArray, 5));
//     int testArray2[5] = {10, 2, 5, 1, 100};
//     assert(isZigZag(testArray2, 5));
//     int testArray3[5] = {10, 20, 15, 12, 40};
//     assert(!isZigZag(testArray3, 5));

//     int testArray4[5] = {-10};
//     assert(isZigZag(testArray4, 1));
//     int testArray5[5] = {-10, -1};
//     assert(isZigZag(testArray5, 2));
//     int testArray6[5] = {-10, -12};
//     assert(isZigZag(testArray6, 2));
//     int testArray7[5] = {-10, -10};
//     assert(!isZigZag(testArray7, 2));
//     int testArray8[5] = {10, 11, 10, 10};
//     assert(!isZigZag(testArray8, 4));
// }

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

bool multiplication(int a, int b)
{
    if (a < 0)
    {
        a = -a;
    }

    if (b < 0)
    {
        b = -b;
    }

    if (a < b)
    {
        return false;
    }
    else if (a == b)
    {
        return true;
    }
    else
    {
        return multiplication(a - b, b);
    }
}

// int main(void)
// {
//     assert(multiplication(10, 2));
//     assert(multiplication(5, 5));
//     assert(multiplication(-10, 2));
//     assert(multiplication(10, -2));
//     assert(multiplication(-10, -5));
//     assert(!multiplication(1, 7));
//     assert(!multiplication(10, 15));
//     assert(!multiplication(22, 7));
//     assert(!multiplication(34, 7));
// }
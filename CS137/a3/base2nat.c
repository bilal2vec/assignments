#include <stdio.h>
#include <assert.h>

int power(int num, int power)
{
    int powed = 1;

    for (int i = 0; i < power; i++)
    {
        powed *= num;
    }

    return powed;
}

int base2nat(int bs, int num)
{
    int dec = 0;

    int current_power = 0;

    while (num > 0)
    {
        int leftmost_digit = num % 10;

        dec += leftmost_digit * power(bs, current_power);

        current_power += 1;
        num /= 10;
    }

    return dec;
}

// int main(void)
// {
//     assert(base2nat(5, 23114) == 1659);
//     assert(base2nat(7, 1) == 1);
//     assert(base2nat(3, 1211012) == 1328);
//     assert(base2nat(8, 715) == 461);
// }
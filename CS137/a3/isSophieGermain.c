#include <stdio.h>
#include <assert.h>

int isprime(int p)
{
    if (p == 2)
    {
        return 1;
    }
    else if (p > 2)
    {
        for (int i = 2; i < p; i++)
        {
            if (p % i == 0)
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

int isSophieGermainPrime(int p)
{

    if (isprime(p) && isprime(2 * p + 1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

// int main(void)
// {

//     assert(isSophieGermainPrime(2));
//     assert(isSophieGermainPrime(11));
//     assert(isSophieGermainPrime(41));
//     assert(isSophieGermainPrime(83));
//     assert(isSophieGermainPrime(113));
//     assert(!isSophieGermainPrime(7));
//     assert(!isSophieGermainPrime(103));
//     assert(!isSophieGermainPrime(-5));
//     assert(!isSophieGermainPrime(112));
//     assert(!isSophieGermainPrime(114));
//     assert(!isSophieGermainPrime(13));

//     return 0;
// }
#include <stdio.h>
#include <assert.h>
#include "fraction.h"

int GCD(int num1, int num2)
{
    int intdivpart = num1 / num2;
    int r = num1 % num2;

    if (r == 0)
    {
        return num2;
    }
    else
    {
        return GCD(num2, r);
    }
}

struct fraction fractionCreate(int numerator, int denominator)
{

    int divzero = 0;
    if (denominator == 0)
    {
        divzero = 1;
    }

    struct fraction temp = {numerator, denominator, divzero};

    return temp;
}

int get_numerator(struct fraction a)
{
    int gcd = GCD(a.num, a.denom);
    int num = a.num / gcd;

    return num;
}
int get_denominator(struct fraction a)
{
    int gcd = GCD(a.num, a.denom);
    int denom = a.denom / gcd;

    return denom;
}
struct fraction fractionAdd(struct fraction a, struct fraction b)
{
    int divzero = 0;
    if ((a.denom == 0) || (b.denom == 0))
    {
        divzero = 1;
    }

    int newnum = (a.num * b.denom) + (b.num * a.denom);
    int newdenom = a.denom * b.denom;

    struct fraction temp = {newnum, newdenom, divzero};
    return temp;
}
struct fraction fractionSubtract(struct fraction a, struct fraction b) {
    int divzero = 0;
    if ((a.denom == 0) || (b.denom == 0))
    {
        divzero = 1;
    }

    int newnum = (a.num * b.denom) - (b.num * a.denom);
    int newdenom = a.denom * b.denom;

    struct fraction temp = {newnum, newdenom, divzero};
    return temp;
}
struct fraction fractionMultiply(struct fraction a, struct fraction b) {
    int divzero = 0;
    if ((a.denom == 0) || (b.denom == 0))
    {
        divzero = 1;
    }

    struct fraction temp = {a.num * b.num, a.denom * b.denom, divzero};
    return temp;
}
struct fraction fractionDivide(struct fraction a, struct fraction b) {
    int divzero = 0;
    if ((a.denom == 0) || (b.denom == 0))
    {
        divzero = 1;
    }

    struct fraction temp = {a.num * b.denom, a.denom * b.num, divzero};
    return temp;
}
void fractionPrint(struct fraction f)
{

    if (f.divzero == 1)
    {
        printf("Divide by zero!\n");
    }
    else if (f.num % f.denom == 0)
    {
        printf("%d\n", f.num / f.denom);
    }
    else
    {
        if (f.num > f.denom)
        {
            int mixed_part = f.num / f.denom;

            int fraction_part = f.num % f.denom;
            int gcd = GCD(fraction_part, f.denom);
            int num = (f.num - (mixed_part * f.denom)) / gcd;
            int denom = f.denom / gcd;
            printf("%d %d/%d\n", mixed_part, num, denom);
        }
        else
        {
            int fraction_part = f.num % f.denom;
            int gcd = GCD(fraction_part, f.denom);
            int num = f.num / gcd;
            int denom = f.denom / gcd;
            printf("%d/%d\n", num, denom);
        }
    }
}

// int main(void)
// {
//     struct fraction a, b, c, d, r, bad;
//     a = fractionCreate(5, 6);
//     fractionPrint(a);
//     b = fractionCreate(8, 9);
//     c = fractionCreate(56, 160);
//     fractionPrint(c);
//     d = fractionCreate(35, 150);
//     bad = fractionCreate(8, 0);
//     fractionPrint(bad);
//     r = fractionAdd(a, b);
//     fractionPrint(r);
//     r = fractionSubtract(c, d);
//     fractionPrint(r);
//     r = fractionMultiply(a, b);
//     fractionPrint(r);
//     r = fractionDivide(c, d);
//     fractionPrint(r);
//     r = fractionAdd(a, bad);
//     fractionPrint(r);
//     r = fractionAdd(b, r);
//     fractionPrint(r);
//     r = fractionCreate(15, 3);
//     fractionPrint(r);
//     assert(5 == get_numerator(r));
//     assert(1 == get_denominator(r));
//     return 0;
// }

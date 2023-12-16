#include <stdio.h>

int main()
{

    int num;
    int denom;

    scanf("%d", &num);
    scanf("%d", &denom);

    if (denom == 0)
    {
        printf("0\n");
    }
    else
    {
        int intfrac = num / denom;

        int remainder = (num % denom) != 0 ? 1 : 0;

        int frac;

        if (num < 0 && denom < 0)
        {
            frac = intfrac + remainder;
        }
        else if (num < 0 || denom < 0)
        {
            frac = intfrac - remainder;
        }
        else
        {
            frac = intfrac + remainder;
        }

        printf("%d\n", frac);
    }

    return 0;
}
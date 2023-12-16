#include <stdio.h>

int main()
{

    //22wide
    //19deep

    // 1-10 $

    int dollars = 2;
    int ats = 20;

    int half = 0;

    for (int r = 0; r < 19; r++)
    {
        for (int i = 0; i < dollars / 2; i++)
        {
            printf("$");
        }

        for (int i = 0; i < ats / 2; i++)
        {
            printf("@");
        }

        for (int i = 0; i < ats / 2; i++)
        {
            printf("@");
        }

        for (int i = 0; i < dollars / 2; i++)
        {
            printf("$");
        }
        printf("\n");

        if (r == 9)
        {
            half = 1;
        }

        if (half == 0)
        {
            dollars += 2;
            ats -= 2;
        }
        else
        {
            dollars -= 2;
            ats += 2;
        }
    }

    return 0;
}

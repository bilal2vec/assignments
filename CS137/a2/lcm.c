#include <stdio.h>

int main()
{
    int a, b;

    scanf("%d %d", &a, &b);

    if (a == 0 || b == 0)
    {
        printf("0\n");
    }
    else
    {
        for (int i = 1; i <= 1000000000; i++)
        {
            if (i % a == 0 && i % b == 0)
            {
                printf("%d\n", i);
                break;
            }
        }
    }

    return 0;
}

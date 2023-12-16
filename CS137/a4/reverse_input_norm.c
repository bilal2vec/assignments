#include <stdio.h>

int recursive_min(int min_so_far)
{
    int temp;
    int min = min_so_far;
    if (scanf("%d", &temp))
    {
        if (temp < min)
        {
            min = temp;
        }

        min = recursive_min(min);

        int t2 = temp - min;
        printf("%d\n", t2);

        return min;
    }
    else
    {
        return min;
    }
}

int reverse_input_norm(int n)
{
    int min = recursive_min(n);
    // printf("%d\n", n - min);
    return min;
}

int main(void)
{
    int n;
    scanf("%d", &n);
    int min = reverse_input_norm(n);
    printf("%d\n", n - min);
}
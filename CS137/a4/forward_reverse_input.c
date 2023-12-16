#include <stdio.h>

void forward_reverse_input(void)
{
    int x;

    if (scanf("%d", &x))
    {
        printf("%d\n", x);
        forward_reverse_input();
        printf("%d\n", x);
    }
}

// int main(void)
// {
//     forward_reverse_input();

//     return 1;
// }
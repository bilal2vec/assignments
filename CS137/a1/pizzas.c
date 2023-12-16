#include <stdio.h>

int main()
{
    int num_people;
    int slices_per_pizza;
    int slices_each;

    scanf("%d %d %d", &num_people, &slices_per_pizza, &slices_each);

    if (num_people == 0 || slices_per_pizza == 0 || slices_each == 0)
    {
        printf("0\n");
    }
    else
    {
        int slices_needed = num_people * slices_each;

        int pizzas_whole = slices_needed / slices_per_pizza;

        pizzas_whole += slices_needed % slices_per_pizza == 0 ? 0 : 1;

        printf("%d\n", pizzas_whole);
    }
}

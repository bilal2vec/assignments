#include <stdio.h>

struct point
{
    int x;
    int y;
};

int smallerthan(struct point point1, struct point point2)
{
    int temp1 = (point1.x * point1.x) + (point1.y * point1.y);
    int temp2 = (point2.x * point2.x) + (point2.y * point2.y);

    if (temp1 < temp2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void sortArrayPoints(struct point parr[], int len)
{
    for (int i = 0; i < (len - 1); i++)
    {
        int smallest = i;
        for (int j = (i + 1); j < len; j++)
        {
            if (smallerthan(parr[j], parr[smallest]) == 1)
            {
                smallest = j;
            }
        }
        struct point temp = parr[smallest];
        parr[smallest] = parr[i];
        parr[i] = temp;
    }
}

// int main(void)
// {
//     struct point points[4] = {{10, 5}, {0, 0}, {-4, -5}, {5, 10}};
//     sortArrayPoints(points, 4);
//     for (int i = 0; i < 4; i++)
//         printf("%d %d\n", points[i].x, points[i].y);
//     return 0;
// }
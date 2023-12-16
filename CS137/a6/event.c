#include <stdio.h>
#include "event.h"
#include <assert.h>

int freetime(struct event schedule[], int n, int hour, int min)
{
    int isfree = 1;
    for (int i = 0; i < n; i++)
    {
        struct event e = schedule[i];
        struct tod start = e.start;
        struct tod end = e.end;

        if ((hour >= start.hour) && (hour <= end.hour))
        {
            if ((min >= start.minute) && (min < end.minute))
            {
                isfree = 0;
            }
        }
    }

    return isfree;
}

// int main(void)

// {
//     struct event schedule[] = {{{9, 45}, {9, 55}}, {{13, 0}, {14, 20}}, {{15, 0}, {16, 30}}};
//     assert(freetime(schedule, 3, 10, 0));
//     assert(!freetime(schedule, 3, 9, 50));
//     return 0;
// }
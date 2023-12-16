#ifndef EVENT_H
#define EVENT_H

struct tod
{
    int hour, minute;
};

struct event
{
    struct tod start, end;
};

int freetime(struct event schedule[], int n, int hour, int min);

#endif
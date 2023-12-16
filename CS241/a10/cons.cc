#include <iostream>
#include <cstdio>
#include <cstdint>
#include "a10p1.h"
#include "a10p2.h"

#include <cstdint>

/*
 * Example usage:
 * cons(1, cons(2, cons(3, nullptr)));
 * This creates a linked list of the form 1 -> 2 -> 3.
 */

int64_t *free_ptr = arena();

int64_t *start = arena();
int64_t size = *arena();
int64_t initialized = 0;

//returns the address of a pair of words from the arena, initialized to a and b respectively, or 0 if no memory is available.
int64_t *cons(int64_t a, int64_t *b)
{

    int64_t max_addr = (int64_t)start + size;

    if ((int64_t)free_ptr >= max_addr)
    {
        return 0;
    }

    if (initialized == 0)
    {
        *free_ptr = (int64_t)(free_ptr + 16);

        initialized = 1;
    }

    int64_t new_free_ptr_loc = *free_ptr;

    *free_ptr = a;
    *(free_ptr + 1) = (int64_t)b;

    int64_t *memory_location = free_ptr;

    free_ptr = (int64_t *)new_free_ptr_loc;

    if (*free_ptr == 0)
    {
        *free_ptr = (int64_t)(free_ptr + 16);
    }

    return memory_location;
}

//returns the first element of the pair whose address is p.
int64_t car(int64_t *p)
{
    return *p;
}

//returns the second element of the pair whose address is p.
int64_t *cdr(int64_t *p)
{
    return (int64_t *)(*(p + 1));
}

//sets the first element of p to the value v and returns p.
int64_t *setcar(int64_t *p, int64_t v)
{
    *p = v;
    return p;
}

//sets the second element of p to the value v and returns p.
int64_t *setcdr(int64_t *p, int64_t *v)
{
    *(p + 1) = (int64_t)v;
    return p;
}

//deletes the pair whose address is p, or does nothing if p is null.
void snoc(int64_t *p)
{
    if (p == 0)
    {
        return;
    }
    else
    {

        int64_t *old_fp_loc = free_ptr;
        int64_t old_fp_next_loc = *free_ptr;

        int64_t *p_as_fp_loc = p;

        // if (old_fp_loc < p_as_fp_loc)
        // {
        //     *old_fp_loc = (int64_t)p_as_fp_loc;
        //     *p_as_fp_loc = old_fp_next_loc;
        // }
        // else
        // {
        *p = (int64_t)free_ptr;
        free_ptr = p;
        // }
    }
}
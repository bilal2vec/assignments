#include <iostream>
#include <cstdio>
#include <cstdint>
#include "a10p1.h"

int64_t *free_ptr = arena();

int64_t *start = arena();
int64_t heap_size = *arena();

int64_t temp2 = 0;

// allocates a block of memory of at least size words (not bytes),
// and returns the address of that memory or 0 (NULL) if memory could not be allocated.
int64_t *mymalloc(int64_t size)
{

    int64_t size_to_allocate = (size * 8) + 8;
    int64_t *p = free_ptr;
    int64_t next_free_ptr = *(p + 1);

    if (size_to_allocate > heap_size)
    {
        return 0;
    }

    while (size_to_allocate > *p)
    {
        p = (int64_t *)*(p + 1);
    }

    if (p + size_to_allocate >= start + heap_size)
    {
        return 0;
    }

    if (*p - size_to_allocate <= 16)
    {
        free_ptr = (int64_t *)*(p + 1);
    }
    else
    {
        free_ptr = p + size_to_allocate;
        int64_t temp = *p - size_to_allocate;
        temp2 += 1;
        if (temp2 > 78125)
        {
            return 0;
        }
        // std::cout << temp2 << " " << *p + size_to_allocate << " " << start << " " << heap_size << std::endl;
        *free_ptr = temp;
        *(free_ptr + 1) = next_free_ptr;
    }

    *p = size_to_allocate;
    return p + 1;
}

// deallocates the memory stored at address.
// assumes that address contains either an address allocated by mymalloc, in which case it deallocates that memory,
// or the value 0 (NULL), in which case myfree does nothing.
void myfree(int64_t *address)
{
    if (address == 0)
    {
        return;
    }

    int64_t *real_addr = address - 1;

    if (real_addr < free_ptr)
    {
        *(real_addr + 1) = (int64_t)free_ptr;
        free_ptr = real_addr;
    }
    else
    {
        int64_t *p = free_ptr;
        while ((int64_t *)*(p + 1) < real_addr)
        {
            p = (int64_t *)*(p + 1);
        }
        int64_t free_ptr_next_free_ptr = *(p + 1);

        *(real_addr + 1) = free_ptr_next_free_ptr;
        *(p + 1) = (int64_t)real_addr;

        // // left merge
        // if (*p = real_addr - p)
        // {
        //     std::cout << "left merge" << std::endl;
        //     *p = *p + *real_addr;
        //     *(p + 1) = *(real_addr + 1);
        // }
    }

    // left merge
    if (free_ptr < real_addr)
    {
        int64_t *p = free_ptr;
        while ((int64_t *)*(p + 1) < real_addr)
        {
            p = (int64_t *)*(p + 1);
        }

        if ((int64_t)p + (*p * 8) == (int64_t)real_addr)
        {
            free_ptr = p;
            *free_ptr = *p + *real_addr;
            *(free_ptr + 1) = *(real_addr + 1);
            real_addr = p;
        }
    }

    // right merge
    if (*(real_addr + 1) - (int64_t)real_addr == *real_addr * 8)
    {
        *real_addr = *real_addr + *(int64_t *)(*(real_addr + 1));
        *(real_addr + 1) = *((int64_t *)*(real_addr + 1) + 1);
    }

    return;
}

int main(int argc, char *argv[])
{
    // int64_t *a = mymalloc(10000000);

    // int i = 0;
    // while (true)
    // {
    //     int64_t *a = mymalloc(1);

    //     if (a == 0)
    //     {
    //         break;
    //     }
    //     i I
    // }
    // std::cout << "heap size: " << i << std::endl;

    // int64_t *b = mymalloc(1);
    // int64_t *c = mymalloc(1);

    // std::cout << std::endl;

    // // myfree(a);
    // // myfree(c);
    // // myfree(b);

    // std::cout << std::endl;

    // for (int i = 0; i < 8; i++)
    // {
    //     std::cout << "address " << start + i * 8 << " : " << *(start + i * 8) << " 0x" << std::hex << *(start + i * 8 + 1) << std::dec << std::endl;
    // }

    // std::cout << "free_ptr: " << free_ptr << " *free_ptr: " << *free_ptr << std::endl;

    // // int64_t *b = mymalloc(5);

    // // std::cout << "a-1: " << a - 1 << " *a-1: " << *(a - 1) << std::endl;
    // // std::cout << "a: " << a << " *a: " << *a << std::endl;
    // // std::cout << "a+1: " << a + 1 << " *a+1: " << *(a + 1) << std::endl
    // //           << std::endl;

    // // std::cout << "b-1: " << b - 1 << " *b-1: " << *(b - 1) << std::endl;
    // // std::cout << "b: " << b << " *b: " << *b << std::endl;
    // // std::cout << "b+1: " << b + 1 << " *b+1: " << *(b + 1) << std::endl
    // //           << std::endl;

    // // std::cout << "free_ptr: " << free_ptr << " *free_ptr: " << *free_ptr << std::endl;
    // // std::cout << "free_ptr+1: " << free_ptr + 1 << " *free_ptr+1: " << *(free_ptr + 1) << std::endl
    // //           << std::endl;

    // return 0;
}

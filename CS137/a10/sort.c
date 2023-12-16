#include <stdio.h>
#include <stdbool.h>

void sort(void *arr, int n, size_t elemsize, bool (*compare)(const void *a, const void *b),
          void (*swap)(void *a, void *b))
{
   for (int i = 0; i < n - 1; i++)
   {
      int min = i;
      for (int j = i + 1; j < n; j++)
      {
         if (compare(((char *)arr) + j * elemsize, ((char *)arr) + min * elemsize))
         {
            min = j;
         }
      }
      swap(((char *)arr) + i * elemsize, ((char *)arr) + min * elemsize);
   }
}

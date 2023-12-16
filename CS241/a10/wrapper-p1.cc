#include <iostream>
#include <cstdint>
#include "a10p1.h"

// === Insert any helper functions here

int64_t wain(int64_t *a, int64_t n)
{

  int64_t *pointer = arena();

  int64_t most_common_value = 0;
  int64_t most_common_count = 0;

  for (int i = 0; i < n; i++)
  {
    int64_t cur = a[i];

    int64_t *cur_pointer = pointer + cur;

    *cur_pointer += 1;

    if (*cur_pointer > most_common_count)
    {
      most_common_value = cur;
      most_common_count = *cur_pointer;
    }
  }

  return most_common_count;
}

// Do not modify the code below.
int main(int argc, char *argv[])
{
  int64_t l, c;
  int64_t *a;
  std::cout << "Enter length of array: ";
  std::cin >> l;
  a = new int64_t[l];
  for (int64_t i = 0; i < l; ++i)
  {
    std::cout << "Enter value of array element " << i << " ";
    std::cin >> a[i];
  }
  c = wain(a, l);
  delete[] a;
  std::cerr << c << std::endl;
  return c;
}

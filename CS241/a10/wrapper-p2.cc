#include <iostream>
#include <cstdint>
#include "a10p2.h"

// === Insert any helper functions here

int64_t wain(int64_t *, int64_t)
{

  int64_t *list = nullptr, *last = nullptr;
  // Let's fill up memory
  while (true)
  {
    last = list;
    list = cons(241, list);
    // I hope your cons implementation returns 0 when memory runs out
    if (!list)
      break;
  }
  // Now let's unfill memory
  while (true)
  {
    list = cdr(last);
    snoc(last);
    last = list;
    if (!last)
      break;
  }
  // It should be possible to allocate something now
  int64_t *cs = cons(-241, nullptr), ret = 0;
  if (cs != 0)
  {
    list = cs;
    // Fill memory again
    while (true)
    {
      last = list;
      list = cons(241, list);
      if (!list)
        break;
    }
    // Get return value (iterate until  end of list)
    while (true)
    {
      ret = car(last);
      last = cdr(last);
      if (!last)
        break;
    }
  }
  // Return a nonzero exit code if the result is wrong
  return ret != -241;

  // std::cin >> std::noskipws;

  // int64_t *p = nullptr;

  // int64_t *start = nullptr;
  // int64_t *prev = nullptr;

  // char c;
  // int64_t counter = 0;
  // while (std::cin >> c)
  // {
  //   p = cons((int64_t)c, nullptr);

  //   if (prev != nullptr)
  //   {
  //     setcdr(prev, p);
  //   }

  //   if (start == nullptr)
  //   {
  //     start = p;
  //   }

  //   prev = p;
  //   counter += 1;
  // }

  // int64_t *curr = start;
  // while (curr != nullptr)
  // {
  //   std::cout << (char)car(curr);
  //   curr = cdr(curr);
  // }

  // curr = start;
  // while (curr != nullptr)
  // {
  //   std::cout << (char)car(curr);
  //   curr = cdr(curr);
  // }

  // curr = start;
  // while (curr != nullptr)
  // {
  //   int64_t *temp = cdr(curr);
  //   snoc(curr);
  //   curr = temp;
  // }

  // return counter;
}

// Do not modify the code below.
int main(int argc, char *argv[])
{
  int ret = wain(0, 0);

  std::cerr << ret << std::endl;
  return ret;
}

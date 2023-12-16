#include <iostream>

int main(int argc, char **argv)
{
  int x = std::stoi(argv[1]);

  std::cout << x % 2 << std::endl;

  return 0;
}

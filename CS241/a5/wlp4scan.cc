#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include "scanner.h"

int main()
{
    std::vector<Token> outputs;
    std::string line;
    try
    {
        while (getline(std::cin, line))
        {
            std::vector<Token> instruction = scan(line);
            for (auto token : instruction)
            {
                outputs.push_back(token);
            }
        }

        for (auto token : outputs)
        {
            std::cout << token << std::endl;
        }
    }

    catch (ScanningFailure &f)
    {
        std::cerr << f.what() << std::endl;
        return 1;
    }

    return 0;
}

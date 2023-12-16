#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

// #include <cassert>

using namespace std;

int scrabbleValue(char chr)
{
    chr = tolower(chr);

    if ((chr == 'e') || (chr == 'a') || (chr == 'i') || (chr == 'o') || (chr == 'n') || (chr == 'r') || (chr == 't') || (chr == 'l') || (chr == 's') || (chr == 'u'))
    {
        return 1;
    }
    else if ((chr == 'd') || (chr == 'g'))
    {
        return 2;
    }
    else if ((chr == 'b') || (chr == 'c') || (chr == 'm') || (chr == 'p'))
    {
        return 3;
    }
    else if ((chr == 'f') || (chr == 'h') || (chr == 'v') || (chr == 'w') || (chr == 'y'))
    {
        return 4;
    }
    else if ((chr == 'k'))
    {
        return 5;
    }
    else if ((chr == 'j') || (chr == 'x'))
    {
        return 8;
    }
    else if ((chr == 'q') || (chr == 'z'))
    {
        return 10;
    }
    else
    {
        return 0;
    }
}

int scrabbleValue(string str)
{

    int sum = 0;

    for (int i = 0; i < str.length(); i++)
    {
        char temp = str[i];

        sum += scrabbleValue(temp);
    }

    return sum;
}

// int main(int argc, char *argv[])
// {

//     assert(scrabbleValue('\0') == 0);
//     assert(scrabbleValue('1') == 0);

//     assert(scrabbleValue('E') == 1);
//     assert(scrabbleValue('A') == 1);
//     assert(scrabbleValue('I') == 1);
//     assert(scrabbleValue('O') == 1);
//     assert(scrabbleValue('N') == 1);
//     assert(scrabbleValue('R') == 1);
//     assert(scrabbleValue('T') == 1);
//     assert(scrabbleValue('L') == 1);
//     assert(scrabbleValue('S') == 1);
//     assert(scrabbleValue('U') == 1);
//     assert(scrabbleValue('e') == 1);
//     assert(scrabbleValue('a') == 1);
//     assert(scrabbleValue('i') == 1);
//     assert(scrabbleValue('o') == 1);
//     assert(scrabbleValue('n') == 1);
//     assert(scrabbleValue('r') == 1);
//     assert(scrabbleValue('t') == 1);
//     assert(scrabbleValue('l') == 1);
//     assert(scrabbleValue('s') == 1);
//     assert(scrabbleValue('u') == 1);

//     assert(scrabbleValue('D') == 2);
//     assert(scrabbleValue('G') == 2);
//     assert(scrabbleValue('d') == 2);
//     assert(scrabbleValue('g') == 2);

//     assert(scrabbleValue('B') == 3);
//     assert(scrabbleValue('C') == 3);
//     assert(scrabbleValue('M') == 3);
//     assert(scrabbleValue('P') == 3);
//     assert(scrabbleValue('b') == 3);
//     assert(scrabbleValue('c') == 3);
//     assert(scrabbleValue('m') == 3);
//     assert(scrabbleValue('p') == 3);

//     assert(scrabbleValue('F') == 4);
//     assert(scrabbleValue('H') == 4);
//     assert(scrabbleValue('V') == 4);
//     assert(scrabbleValue('W') == 4);
//     assert(scrabbleValue('Y') == 4);
//     assert(scrabbleValue('f') == 4);
//     assert(scrabbleValue('h') == 4);
//     assert(scrabbleValue('v') == 4);
//     assert(scrabbleValue('w') == 4);
//     assert(scrabbleValue('y') == 4);

//     assert(scrabbleValue('K') == 5);
//     assert(scrabbleValue('k') == 5);

//     assert(scrabbleValue('J') == 8);
//     assert(scrabbleValue('X') == 8);
//     assert(scrabbleValue('j') == 8);
//     assert(scrabbleValue('x') == 8);

//     assert(scrabbleValue('Q') == 10);
//     assert(scrabbleValue('Z') == 10);
//     assert(scrabbleValue('q') == 10);
//     assert(scrabbleValue('z') == 10);

//     assert(scrabbleValue("") == 0);
//     assert(scrabbleValue("aaaaaaaaaa") == 10);
//     assert(scrabbleValue("aaaaaaaa a") == 9);
//     assert(scrabbleValue("aaaaaa 11a") == 7);
//     assert(scrabbleValue("aAaAEa 11a") == 7);

//     assert(scrabbleValue("  aAaAEa 11a") == 7);

//     assert(scrabbleValue("QZ EA F90") == (10 + 10 + 1 + 1 + 4));

//     return 0;
// }
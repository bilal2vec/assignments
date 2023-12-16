#include <string>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <list>
#include "merl.h"

/*
Starter code for the linker you are to write for CS241 Assignment 10 Problem 5.
The code uses the MERL library defined in "merl.h". This library depends on the object file
"merl.o" and this program must be compiled with those object files.

The starter code includes functionality for processing command-line arguments, reading the input 
MERL files, calling the linker, and producing output on standard output.

You need to implement the "linking constructor" for MERL files, which takes two MERL file objects
as input and produces a new MERL file object representing the result of linking the files:

MERL::MERL(MERL& m1, MERL& m2)

A definition of this function is at the end of the file. The default implementation creates a
dummy MERL file to demonstrate the use of the MERL library. You should replace it with code that
links the two input files.

You are free to modify the existing code as needed, but it should not be necessary.
You are free to add your own helper functions to this file.

The functions in the MERL library will throw std::runtime_error if an error occurs. The starter
code is set up to catch these exceptions and output an error message to standard error. You may
wish to use this functionality for your own error handling.
*/

// Links all the MERL objects in the given list, by recursively
// calling the "linking constructor" that links two MERL objects.
// You should not need to modify this function.
MERL link(std::list<MERL> &merls)
{
    if (merls.size() == 1)
    {
        return merls.front();
    }
    else if (merls.size() == 2)
    {
        return MERL(merls.front(), merls.back());
    }
    MERL first = merls.front();
    merls.pop_front();
    MERL linkedRest = link(merls);
    return MERL(first, linkedRest);
}

// Main function, which reads the MERL files and passes them into
// the link function, then outputs the linked MERL file.
// You should not need to modify this function.
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        std::cerr << "Usage: " << argv[0] << " <file1.merl> <file2.merl> ..." << std::endl;
        return 1;
    }
    std::list<MERL> merls;
    try
    {
        for (int i = 1; i < argc; i++)
        {
            std::ifstream file;
            file.open(argv[i]);
            merls.emplace_back(file);
            file.close();
        }
        // Link all the MERL files read from the command line.
        MERL linked = link(merls);
        // Print a readable representation of the linked MERL file to standard error for debugging.
        linked.print(std::cerr);
        // Write the binary representation of the linked MERL file to standard output.
        std::cout << linked;
    }
    catch (std::runtime_error &re)
    {
        std::cerr << "ERROR: " << re.what() << std::endl;
        return 1;
    }
    return 0;
}

// Linking constructor for MERL objects.
// Implement this, which constructs a new MERL object by linking the two given MERL objects.
// The function is allowed to modify the inputs m1 and m2. It does not need to leave them in
// the same state as when the function was called. The default implementation creates a dummy
// MERL file that does not depend on either input, to demonstrate the use of the MERL library.
// You should not output anything to standard output here; the main function handles output.
MERL::MERL(MERL &m1, MERL &m2)

{

    std::vector<Entry> m1_table = m1.table;
    std::vector<Entry> m2_table = m2.table;

    for (Entry e : m1_table)
    {
        if (e.type == Entry::Type::ESD)
        {
            for (Entry e2 : m2_table)
            {
                if (e2.type == Entry::Type::ESD)
                {
                    if (e.name == e2.name)
                    {
                        throw std::runtime_error("duplicate exports");
                    }
                }
            }
        }
    }

    for (auto c : m1.code)
    {
        code.push_back(c);
    }

    for (auto c : m2.code)
    {
        code.push_back(c);
    }

    int reloc_offset = m1.endCode - 12;

    for (int i = 0; i < m2.table.size(); i++)
    {
        m2.table[i].location += reloc_offset;
    }

    for (int i = 0; i < m2.table.size(); i++)
    {
        if (m2.table[i].type == Entry::Type::REL)
        {
            int index = (m2.table[i].location - 12) / 4;
            code[index] += reloc_offset;
        }
    }

    int temp = 0;

    for (int i = 0; i < m1.table.size(); i++)
    {
        if (m1.table[i].type == Entry::Type::ESR)
        {
            for (int j = 0; j < m2.table.size(); j++)
            {
                if (m2.table[j].type == Entry::Type::ESD)
                {
                    if (m1.table[i].name == m2.table[j].name)
                    {
                        int index = (m1.table[i].location - 12) / 4;
                        code[index] = m2.table[j].location;
                        m1.table[i].type = Entry::Type::REL;
                        temp += 1;
                    }
                }
            }
        }
    }

    for (int i = 0; i < m2.table.size(); i++)
    {
        if (m2.table[i].type == Entry::Type::ESR)
        {
            for (int j = 0; j < m1.table.size(); j++)
            {
                if (m1.table[j].type == Entry::Type::ESD)
                {
                    if (m2.table[i].name == m1.table[j].name)
                    {
                        int index = (m2.table[i].location - 12) / 4;
                        code[index] = m1.table[j].location;
                        m2.table[i].type = Entry::Type::REL;
                        temp += 1;
                    }
                }
            }
        }
    }

    int counter = 0;
    for (auto e : m1.table)
    {
        counter += e.size();
        table.push_back(e);
    }

    for (auto e : m2.table)
    {
        counter += e.size();
        table.push_back(e);
    }

    endCode = 12 + (code.size() * 4);
    // std::cerr
    //     << "edcode: " << endCode << " " << temp << std::endl;
    endModule = endCode + counter; // - (4 * temp);
    // std::cerr
    //     << "tesst: " << table.size() * 4 << " " << counter << std::endl;
    // // endModule *= 2;

    // std::cerr << "endModule: " << endModule << std::endl;

    //     endCode = 20;
    // endModule = 108;
    // code.push_back(0x03e00008);
    // code.push_back(0x00000000);
    // table.emplace_back(Entry::Type::REL, 16);
    // table.emplace_back(Entry::Type::ESD, 12, "Not");
    // table.emplace_back(Entry::Type::ESR, 12, "Implemented");
}

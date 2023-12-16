// Don't change the includes or the LexTree class declaration!
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <map>
#include <vector> // You can use a vector in main to set up test data

// #include <cassert>

using namespace std;

class LexTree
{
public:
    LexTree();
    virtual ~LexTree();
    void addWord(const string &s);
    void addFile(const string &filename);
    bool hasWord(const string &s) const;
    void print() const;

private:
    void printHelper(const string &s) const;
    bool isWord;
    map<char, LexTree *> children;
};

LexTree::LexTree()
{
    this->isWord = false;
    this->children = map<char, LexTree *>{};
}

LexTree::~LexTree()
{
    for (auto it = this->children.begin(); it != this->children.end(); it++)
    {
        delete it->second;
    }
}

void LexTree::addWord(const string &s)
{

    if (s.size() == 0)
    {
        this->isWord = true;
    }
    else
    {
        char first_char = s[0];
        first_char = tolower(first_char);

        string rest_of_string = s.substr(1);

        if (this->children[first_char])
        {
            this->children[first_char]->addWord(rest_of_string);
        }
        else
        {
            LexTree *new_tree = new LexTree{};
            new_tree->addWord(rest_of_string);

            this->children[first_char] = new_tree;
        }
    }
}

void LexTree::addFile(const string &filename)
{

    ifstream file{filename};

    if (!file)
    {
        cerr << "Couldn't open " << filename << " for reading" << endl;
        exit(1);
    }
    else
    {
        vector<string> words{};

        string current_word;

        while (file >> current_word)
        {
            words.push_back(current_word);
        }

        for (const auto &w : words)
        {
            this->addWord(w);
        }
    }

    file.close();
}

bool LexTree::hasWord(const string &s) const
{
    if (s.size() == 0)
    {
        if (this->isWord)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        const char first_char = tolower(s[0]);

        const string rest_of_string = s.substr(1);

        if (this->children.find(first_char) != this->children.end())
        {
            return this->children.at(first_char)->hasWord(rest_of_string);
        }
        else
        {
            return false;
        }
    }
}

void LexTree::print() const
{
    this->printHelper("");
}

void LexTree::printHelper(const string &s) const
{
    if (this->isWord)
    {
        cout << s << endl;
    }

    for (auto it = this->children.begin(); it != this->children.end(); it++)
    {
        string new_string = s;
        new_string.push_back(it->first);
        it->second->printHelper(new_string);
    }
}

// int main(int argc, char *argv[])
// {
//     LexTree lex{};

//     assert(lex.hasWord("") == false);
//     assert(lex.hasWord("a") == false);

//     vector<string> words{"Apple", "app", "appstore", "applepie",
//                          "baker", "baked", "charlie", "bake"};
//     for (const auto &w : words)
//     {
//         lex.addWord(w);
//     }

//     lex.print();

//     assert(lex.hasWord("") == false);
//     assert(lex.hasWord("app") == true);
//     assert(lex.hasWord("ApP") == true);
//     assert(lex.hasWord("apple") == true);
//     assert(lex.hasWord("apples") == false);
//     assert(lex.hasWord("Apples") == false);
//     assert(lex.hasWord("apps") == false);

//     cout << endl;

//     LexTree lex2{};
//     lex2.addFile("./test.txt");
//     assert(lex.hasWord("") == false);
//     assert(lex.hasWord("app") == true);
//     assert(lex.hasWord("apple") == true);
//     assert(lex.hasWord("apples") == false);
//     assert(lex.hasWord("apps") == false);
//     lex2.print();

//     return 0;

//     //empty string
//     //duplicates
// }

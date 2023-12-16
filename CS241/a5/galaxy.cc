#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>

using std::istream;
using std::ostream;
using std::string;

//Skip the grammar part of the input.
void skipLine(istream &in)
{
    string s;
    getline(in, s);
}

void skipGrammar(istream &in)
{
    int i, numTerm, numNonTerm, numRules;

    // read the number of terminals and move to the next line
    in >> numTerm;
    skipLine(in);

    // skip the lines containing the terminals
    for (i = 0; i < numTerm; i++)
    {
        skipLine(in);
    }

    // read the number of non-terminals and move to the next line
    in >> numNonTerm;
    skipLine(in);

    // skip the lines containing the non-terminals
    for (i = 0; i < numNonTerm; i++)
    {
        skipLine(in);
    }

    // skip the line containing the start symbol
    skipLine(in);

    // read the number of rules and move to the next line
    in >> numRules;
    skipLine(in);

    // skip the lines containing the production rules
    for (i = 0; i < numRules; i++)
    {
        skipLine(in);
    }
}

string trim(const string &str)
{
    size_t begin = str.find_first_not_of(" \t\n");
    if (begin == string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t\n");

    return str.substr(begin, end - begin + 1);
}

// Prints the derivation with whitespace trimmed.
void printDerivation(istream &in, ostream &out)
{
    string line;

    while (getline(in, line))
    {
        out << trim(line) << '\n';
    }
}

std::vector<std::string> returnDerivation(istream &in)
{
    std::vector<std::string> derivation;
    string line;
    while (getline(in, line))
    {
        string line2 = trim(line);

        derivation.push_back(line2);
    }

    return derivation;
}

class Node
{
public:
    std::string value;
    int expr_value;
    int n_children;
    std::vector<Node *> children;

    Node(std::string value, int n_children) : value(value), expr_value(0), n_children(n_children) {}

    void add_child(Node *child)
    {
        children.push_back(child);
    }

    ~Node()
    {
        for (Node *node : children)
        {
            delete node;
        }
    }
};

Node *create_tree(std::vector<std::string> &lines)
{
    std::string cur_line = lines[0];

    lines.erase(lines.begin());

    std::string value = cur_line;
    int n_children;

    if (cur_line == "S BOF expr EOF")
    {
        n_children = 1;
    }
    else if (cur_line == "expr term")
    {
        n_children = 1;
    }
    else if (cur_line == "expr expr - term")
    {
        n_children = 2;
    }
    else if (cur_line == "term id")
    {
        n_children = 0;
    }
    else if (cur_line == "term ( expr )")
    {
        n_children = 1;
    }

    Node *node = new Node(value, n_children);

    for (int i = 0; i < n_children; i++)
    {

        Node *child = create_tree(lines);
        node->add_child(child);
    }

    return node;
}

void print_tree(Node *root)
{
    std::cout << root->value << " " << root->n_children << " (";
    for (int i = 0; i < root->n_children; i++)
    {
        std::cout << root->children[i]->value << ", ";
    }
    std::cout << ")" << std::endl;

    for (int i = 0; i < root->n_children; i++)
    {
        print_tree(root->children[i]);
    }
}

int evaluate_tree(Node *root)
{

    std::vector<int> child_values;

    for (int i = 0; i < root->n_children; i++)
    {
        int child_value = evaluate_tree(root->children[i]);
        child_values.push_back(child_value);
    }

    if ((root->value == "S BOF expr EOF") || (root->value == "expr term") || (root->value == "term ( expr )"))
    {
        root->expr_value = child_values[0];
    }
    else if (root->value == "term id")
    {
        root->expr_value = 42;
    }
    else if (root->value == "expr expr - term")
    {
        root->expr_value = child_values[0] - child_values[1];
    }

    return root->expr_value;
}

void print_tree_post_order(Node *root)
{

    for (int i = 0; i < root->n_children; i++)
    {
        print_tree_post_order(root->children[i]);
    }

    std::cout << root->value << " " << root->n_children << std::endl;
}

int main()
{
    skipGrammar(std::cin);
    // printDerivation(std::cin, std::cout);
    std::vector<std::string> lines = returnDerivation(std::cin);

    // for (auto line : lines)
    // {
    //     std::cout << line << std::endl;
    // }

    Node *root = create_tree(lines);

    // print_tree(root);

    // print_tree_post_order(root);

    int result = evaluate_tree(root);

    std::cout << result << std::endl;

    delete root;

    return 0;
}

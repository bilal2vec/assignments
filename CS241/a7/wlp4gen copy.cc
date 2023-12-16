#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

std::vector<std::string> terminals = {"BOF", "BECOMES", "COMMA", "ELSE", "EOF", "EQ", "GE", "GT", "ID", "IF", "INT", "LBRACE", "LE", "LPAREN", "LT", "MINUS", "NE", "NUM", "PCT", "PLUS", "PRINTLN", "RBRACE", "RETURN", "RPAREN", "SEMI", "SLASH", "STAR", "WAIN", "WHILE", "AMP", "LBRACK", "RBRACK", "NEW", "DELETE", "NULL"};

std::vector<std::vector<std::string>> lines;
std::map<std::string, std::tuple<std::vector<std::string>, std::map<std::string, std::string>>> table;

std::string current_procedure;

class Error
{
public:
    std::string message;
    Error(std::string message) : message(message) {}
    std::string what() { return message; }
};

class Tree
{
public:
    std::vector<std::string> rule;
    std::string type;
    int n_children;
    std::vector<Tree *> children;

    Tree(std::vector<std::string> rule, int n_children) : rule(rule), n_children(n_children) {}

    void add_child(Tree *child)
    {
        children.push_back(child);
    }

    ~Tree()
    {
        for (Tree *child : children)
        {
            delete child;
        }
    }
};

Tree *create_tree()
{
    std::vector<std::string> line = lines[0];

    int n_children;
    if (std::find(terminals.begin(), terminals.end(), line[0]) == terminals.end())
    {
        n_children = line.size() - 1;
    }
    else
    {
        n_children = 0;
    }

    Tree *tree = new Tree(line, n_children);
    lines.erase(lines.begin());

    for (int i = 0; i < n_children; i++)
    {
        Tree *child = create_tree();
        tree->add_child(child);
    }

    return tree;
}

void print_tree(Tree *tree)
{
    for (auto token : tree->rule)
    {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    std::cout << "type: " << tree->type << std::endl;
    std::cout << "n_children: " << tree->n_children << std::endl;
    std::cout << std::endl;

    for (auto child : tree->children)
    {
        print_tree(child);
    }
}

void parse_tree(Tree *tree)
{
    std::vector<std::string> rule = tree->rule;
    std::string rule_name = tree->rule[0];

    if ((rule_name == "main") || (rule_name == "procedure"))
    {
        if (rule_name == "main")
        {
            current_procedure = "wain";
        }
        else if (rule_name == "procedure")
        {
            current_procedure = tree->children[1]->children[0]->rule[1];
        }

        if (table.find(current_procedure) == table.end())
        {
            table[current_procedure] = std::tuple<std::vector<std::string>, std::map<std::string, std::string>>();
        }
        else
        {
            throw Error("procedure " + current_procedure + " being redefined");
        }
    }

    for (auto child : tree->children)
    {

        parse_tree(child);
    }

    if ((rule_name == "main") || (rule_name == "procedure"))
    {
        std::vector<std::string> param_types;

        if (rule_name == "main")
        {
            param_types.push_back(tree->children[3]->type);
            param_types.push_back(tree->children[5]->type);
        }
        else
        {
        }

        table[current_procedure] = std::make_tuple(param_types, std::get<1>(table[current_procedure]));
    }

    std::string type = "";

    if (rule_name == "type")
    {
        if (rule.size() == 2)
        {
            type = "int";
        }
        else if (rule.size() == 3)
        {
            type = "int*";
        }
    }
    else if (rule_name == "dcl")
    {
        type = tree->children[0]->type;

        std::string id = tree->children[1]->rule[1];

        std::map<std::string, std::string> sym_table = std::get<1>(table[current_procedure]);

        if (sym_table.find(id) == sym_table.end())
        {
            std::get<1>(table[current_procedure])[id] = type;
        }
        else
        {
            throw Error("redefining of symbol " + id);
        }
    }
    else if (((rule_name == "factor") || (rule_name == "lvalue")) && (rule[1] == "ID"))
    {
        type = tree->children[0]->type;

        std::string id = tree->children[0]->rule[1];

        std::map<std::string, std::string> sym_table = std::get<1>(table[current_procedure]);

        if (sym_table.find(id) == sym_table.end())
        {
            throw Error("use of symbol " + id + " before/without defenition");
        }
    }

    tree->type = type;
}

int main()
{
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokenized_line;
        while (ss >> token)
        {
            tokenized_line.push_back(token);
        }

        lines.push_back(tokenized_line);
    }

    Tree *tree = create_tree();

    try
    {
        parse_tree(tree);

        // print_tree(tree);

        for (auto procedure : table)
        {
            std::string procedure_name = procedure.first;
            std::vector<std::string> param_types = std::get<0>(table[procedure_name]);

            std::cerr << procedure_name << ": ";
            for (auto param_type : param_types)
            {
                std::cerr << param_type << " ";
            }
            std::cerr << std::endl;
        }

        std::map<std::string, std::string> sym_table = std::get<1>(table["wain"]);
        for (auto sym : sym_table)
        {
            std::cerr << sym.first << " " << sym.second << std::endl;
        }
    }
    catch (Error &f)
    {
        std::cerr << "ERROR " << f.what() << std::endl;

        delete tree;

        return 1;
    }

    delete tree;
    return 0;
}
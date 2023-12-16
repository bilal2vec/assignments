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
bool in_procedure_signature = false;

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
    std::string rule_str = "";

    std::string type = "nothing";

    int n_children;
    std::vector<Tree *> children;

    Tree(std::vector<std::string> rule, int n_children) : rule(rule), n_children(n_children)
    {
        for (auto &r : rule)
        {
            rule_str += r + " ";
        }
        rule_str.pop_back();
    }

    std::string get_type()
    {
        if (type == "nothing")
        {
            for (auto child : children)
            {
                child->get_type();
            }

            std::string rule_name = rule[0];

            if (rule_name == "NUM")
            {
                type = "int";
            }
            else if (rule_name == "NULL")
            {
                type = "int*";
            }
            else if (rule_str == "factor NUM")
            {
                type = "int";
            }
            else if (rule_str == "factor NULL")
            {
                type = "int*";
            }
            else if (rule_str == "factor LPAREN expr RPAREN")
            {
                type = children[1]->type;
            }
            else if (rule_str == "lvalue LPAREN lvalue RPAREN")
            {
                type = children[1]->type;
            }
            else if (rule_str == "factor AMP lvalue")
            {
                if (children[1]->type == "int")
                {
                    type = "int*";
                }
                else
                {
                    throw Error("can only reference (&) int");
                }
            }
            else if (rule_str == "factor STAR factor")
            {
                if (children[1]->type == "int*")
                {
                    type = "int";
                }
                else
                {
                    throw Error("can only dereference int pointer");
                }
            }
            else if (rule_str == "lvalue STAR factor")
            {
                if (children[1]->type == "int*")
                {
                    type = "int";
                }
                else
                {
                    throw Error("can only dereference int pointer");
                }
            }

            else if (rule_str == "factor NEW INT LBRACK expr RBRACK")
            {
                if (children[3]->type == "int")
                {
                    type = "int*";
                }
                else
                {
                    throw Error("can only create int array");
                }
            }
            else if (rule_str == "term factor")
            {
                type = children[0]->type;
            }
            else if (rule_str == "expr term")
            {
                type = children[0]->type;
            }
            else if (rule_str == "expr expr PLUS term")
            {
                if ((children[0]->type == "int") && (children[2]->type == "int"))
                {
                    type = "int";
                }
                else if ((children[0]->type == "int*") && (children[2]->type == "int"))
                {
                    type = "int*";
                }
                else if ((children[0]->type == "int") && (children[2]->type == "int*"))
                {
                    type = "int*";
                }
                else
                {
                    throw Error("wrong parameters for add");
                }
            }
            else if (rule_str == "expr expr MINUS term")
            {
                if ((children[0]->type == "int") && (children[2]->type == "int"))
                {
                    type = "int";
                }
                else if ((children[0]->type == "int*") && (children[2]->type == "int"))
                {
                    type = "int*";
                }
                else if ((children[0]->type == "int*") && (children[2]->type == "int*"))
                {
                    type = "int";
                }
                else
                {
                    throw Error("wrong parameters for sub");
                }
            }
            else if (rule_str == "term term STAR factor")
            {

                if ((children[0]->type == "int") && (children[2]->type == "int"))
                {
                    type = "int";
                }
                else
                {
                    throw Error("wrong parameters for star");
                }
            }
            else if (rule_str == "term term SLASH factor")
            {
                if ((children[0]->type == "int") && (children[2]->type == "int"))
                {
                    type = "int";
                }
                else
                {
                    throw Error("wrong parameters for slash");
                }
            }
            else if (rule_str == "term term PCT factor")
            {
                if ((children[0]->type == "int") && (children[2]->type == "int"))
                {
                    type = "int";
                }
                else
                {
                    throw Error("wrong parameters for pct");
                }
            }
            else if (rule_name == "main")
            {
                if (children[5]->get_type() != "int")
                {
                    throw Error("second dcl in wain must be int");
                }
                if (children[11]->get_type() != "int")
                {
                    throw Error("function must return int");
                }
            }
            else if (rule_name == "procedure")
            {
                if (children[9]->get_type() != "int")
                {
                    throw Error("function must return int");
                }
            }
            else if (rule_str == "statement lvalue BECOMES expr SEMI")
            {
                if (children[0]->type != children[2]->type)
                {
                    throw Error("lvalue and expr must same type");
                }
            }
            else if (rule_str == "statement PRINTLN LPAREN expr RPAREN SEMI")
            {
                if (children[2]->type != "int")
                {
                    throw Error("expr must be int");
                }
            }
            else if (rule_str == "statement DELETE LBRACK RBRACK expr SEMI")
            {
                if (children[3]->type != "int*")
                {
                    throw Error("can only delete int*");
                }
            }
            else if (rule_name == "test")
            {
                if (children[0]->type != children[2]->type)
                {
                    throw Error("both operands of test must be same");
                }
            }
            else if (rule_str == "dcls dcls dcl BECOMES NUM SEMI")
            {
                if (children[1]->type != "int")
                {
                    throw Error("dcl deriving num must be int");
                }
            }
            else if (rule_str == "dcls dcls dcl BECOMES NULL SEMI")
            {
                if (children[1]->type != "int*")
                {
                    throw Error("dcl deriving null must be int*");
                }
            }
        }

        return type;
    }

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

std::string indent_spaces(int indent)
{
    std::string temp = "";
    for (int i = 0; i < indent; i++)
    {
        temp += " ";
    }
    return temp;
}

void print_tree(Tree *tree, int indent)
{
    std::cout << indent_spaces(indent) << "rule: ";
    for (auto token : tree->rule)
    {
        std::cout << token << " ";
    }

    std::cout << "type: " << tree->type << " ";
    std::cout << "n_children: " << tree->n_children << " ";
    std::cout << std::endl;

    for (auto child : tree->children)
    {
        print_tree(child, indent + 4);
    }
}

void print_symbol_table()
{
    for (auto procedure : table)
    {
        std::cerr << procedure.first << ": ";

        for (auto signature : std::get<0>(procedure.second))
        {
            std::cerr << signature << " ";
        }
        std::cerr << std::endl;

        for (auto decl : std::get<1>(procedure.second))
        {
            std::cerr << decl.first << " " << decl.second << std::endl;
        }
    }
}

void build_symbol_table(Tree *tree)
{
    std::string rule_name = tree->rule[0];

    if (rule_name == "procedure")
    {
        std::string procedure_name = tree->children[1]->rule[1];
        if (table.find(procedure_name) == table.end())
        {
            table[procedure_name] = std::tuple<std::vector<std::string>, std::map<std::string, std::string>>();
            current_procedure = procedure_name;
        }
        else
        {
            throw Error("procedure " + procedure_name + " already defined");
        }

        in_procedure_signature = true;
    }
    else if (rule_name == "main")
    {
        if (table.find("wain") == table.end())
        {
            table["wain"] = std::tuple<std::vector<std::string>, std::map<std::string, std::string>>();
            current_procedure = "wain";
        }
        else
        {
            throw Error("procedure " + current_procedure + " already defined");
        }

        in_procedure_signature = true;
    }

    for (auto child : tree->children)
    {
        build_symbol_table(child);
    }

    if (rule_name == "RPAREN")
    {
        in_procedure_signature = false;
    }

    if (rule_name == "dcl")
    {
        std::string id = tree->children[1]->rule[1];
        int type_n_children = tree->children[0]->rule.size();

        std::string type;
        if (type_n_children == 2)
        {
            type = "int";
        }
        else if (type_n_children == 3)
        {
            type = "int*";
        }
        else
        {
            throw Error("weird type error that should never happen");
        }

        if (in_procedure_signature)
        {
            std::get<0>(table[current_procedure]).push_back(type);
        }

        std::map<std::string, std::string> sym_table = std::get<1>(table[current_procedure]);
        if (sym_table.find(id) == sym_table.end())
        {
            std::get<1>(table[current_procedure])[id] = type;
        }
        else
        {
            throw Error(id + " already defined");
        }
        tree->type = type;
    }
    else if (((rule_name == "factor") || (rule_name == "lvalue")) && (tree->rule[1] == "ID") && (tree->rule.size() == 2))
    {
        std::string lexeme = tree->children[0]->rule[1];

        std::map<std::string, std::string> sym_table = std::get<1>(table[current_procedure]);
        if (sym_table.find(lexeme) == sym_table.end())
        {
            throw Error("variable " + lexeme + " is not defined");
        }
        tree->type = sym_table[lexeme];
    }
    else if ((rule_name == "factor") && (tree->rule[1] == "ID") && (tree->rule[2] == "LPAREN"))
    {
        std::string lexeme = tree->children[0]->rule[1];

        if (table.find(lexeme) == table.end())
        {
            throw Error("function " + lexeme + " isn't defined");
        }

        std::vector<std::string> signature = std::get<0>(table[lexeme]);

        if (tree->rule.size() == 4)
        {
            tree->type = "int";
            if (signature.size() != 0)
            {
                throw Error("function " + lexeme + " takes 0 parameters");
            }
        }
        else
        {
            Tree *temp = tree->children[2];
            int i = 1;
            std::vector<std::string> param_types;

            while (true)
            {
                if ((temp->rule.size() == 4) && (temp->children[2]->rule[0] == "arglist"))
                {
                    i += 1;
                    param_types.push_back(temp->children[0]->get_type());
                    temp = temp->children[2];
                }

                else
                {
                    break;
                }
            }

            param_types.push_back(temp->children[0]->get_type());

            if (signature.size() != i)
            {
                throw Error("wrong number of parameters for function " + lexeme);
            }

            for (auto i = 0; i < signature.size(); i++)
            {
                if (signature[i] != param_types[i])
                {
                    throw Error("Parameter of type " + param_types[i] + " does not match expected type " + signature[i]);
                }
            }

            tree->type = "int";
        }
    }

    // if ((rule_name == "expr") || (rule_name == "lvalue"))
    // {
    tree->get_type();
    // }
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

        build_symbol_table(tree);
        print_symbol_table();
        // print_tree(tree, 0);
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
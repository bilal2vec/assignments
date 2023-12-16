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

std::vector<std::string> codegen;
std::map<std::string, int> fp_offsets;
int sp_pushes = 0;
int fp_max_offset = 0;

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

void prologue()
{
    codegen.push_back(".import print");
    codegen.push_back("lis $4");
    codegen.push_back(".word 4");
    codegen.push_back("lis $10");
    codegen.push_back(".word print");
    codegen.push_back("lis $11");
    codegen.push_back(".word 1");
    codegen.push_back("sub $29, $30, $4");
}

void push(std::string r)
{
    sp_pushes += 1;

    codegen.push_back("sw $" + r + ", -4($30)");
    codegen.push_back("sub $30, $30, $4");
}

void pop(std::string r)
{
    sp_pushes -= 1;

    codegen.push_back("add $30, $30, $4");
    codegen.push_back("lw $" + r + ", -4($30)");
}

void epilogue()
{
    for (int i = 0; i < sp_pushes; i++)
    {
        codegen.push_back("add $30, $30, $4");
    }

    codegen.push_back("jr $31");
}

std::string lvalue_unwrapping(Tree *tree)
{
    if (tree->rule[1] == "ID")
    {
        return tree->children[0]->rule[1];
    }
    else if (tree->rule[1] == "LPAREN")
    {
        return lvalue_unwrapping(tree->children[1]);
    }
    else
    {
        throw Error("todo implement lvalue rules");
    }
}

void code(Tree *tree)
{
    if (tree->rule[0] == "start")
    {
        code(tree->children[1]);
    }
    else if (tree->rule[0] == "procedures")
    {
        //reset frame pointer for each procedure
        fp_offsets = std::map<std::string, int>();
        fp_max_offset = 0;

        if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
        }
        else
        {
            throw Error("todo: implement procedures → procedure procedures");
        }
    }
    else if (tree->rule[0] == "main")
    {
        std::string variable_name;

        variable_name = tree->children[3]->children[1]->rule[1];
        fp_offsets[variable_name] = fp_max_offset;

        codegen.push_back("sw $1, 0($29)");
        fp_max_offset += 4;

        codegen.push_back("sub $30, $30, $4");
        sp_pushes += 1;

        variable_name = tree->children[5]->children[1]->rule[1];
        fp_offsets[variable_name] = fp_max_offset;

        codegen.push_back("sw $2, -4($29)");
        fp_max_offset += 4;

        codegen.push_back("sub $30, $30, $4");
        sp_pushes += 1;

        code(tree->children[8]);  // dcls
        code(tree->children[9]);  // statements
        code(tree->children[11]); // expr
    }
    else if (tree->rule[0] == "dcls")
    {
        if (tree->rule.size() == 1)
        {
            return;
        }
        else if (tree->rule.size() == 6)
        {
            if (tree->rule[4] == "NUM")
            {
                code(tree->children[0]);
                std::string variable_name;
                std::string variable_value;

                variable_name = tree->children[1]->children[1]->rule[1];
                variable_value = tree->children[3]->rule[1];
                fp_offsets[variable_name] = fp_max_offset;
                codegen.push_back("lis $3");
                codegen.push_back(".word " + variable_value);
                codegen.push_back("sw $3, -" + std::to_string(fp_max_offset) + "($29)");
                codegen.push_back("sub $30, $30, $4");
                sp_pushes += 1;
                fp_max_offset += 4;
            }
            else
            {
                throw Error("todo: implement dcls rules");
            }
        }
        else
        {
            throw Error("todo: implement dcls rules");
        }
    }
    else if (tree->rule[0] == "statements")
    {
        if (tree->rule.size() == 1)
        {
            return;
        }
        else if (tree->rule.size() == 3)
        {
            code(tree->children[0]);
            code(tree->children[1]);
        }
    }
    else if (tree->rule[0] == "statement")
    {
        if (tree->rule.size() == 5)
        {
            code(tree->children[2]); // expr

            std::string variable_name = lvalue_unwrapping(tree->children[0]);

            int offset = fp_offsets[variable_name];
            codegen.push_back("sw $3, -" + std::to_string(offset) + "($29)");
        }
        else if (tree->rule[1] == "PRINTLN")
        {
            push("1");
            code(tree->children[2]);
            codegen.push_back("add $1, $3, $0");
            push("31");
            codegen.push_back("jalr $10");
            pop("31");
            pop("1");
        }
        else if (tree->rule.size() == 8) //while
        {
            int label_unique_value = codegen.size();
            codegen.push_back("loop" + std::to_string(label_unique_value) + ":");
            code(tree->children[2]);
            codegen.push_back("beq $3, $0, endWhile" + std::to_string(label_unique_value));
            code(tree->children[5]);
            codegen.push_back("beq $0, $0, loop" + std::to_string(label_unique_value));
            codegen.push_back("endWhile" + std::to_string(label_unique_value) + ":");
        }
        else if (tree->rule.size() == 12)
        {
            int label_unique_value = codegen.size();
            code(tree->children[2]);
            codegen.push_back("beq $3, $0, else" + std::to_string(label_unique_value));
            code(tree->children[5]);
            codegen.push_back("beq $0, $0, endif" + std::to_string(label_unique_value));
            codegen.push_back("else" + std::to_string(label_unique_value) + ":");
            code(tree->children[9]);
            codegen.push_back("endif" + std::to_string(label_unique_value) + ":");
        }
    }

    else if (tree->rule[0] == "test")
    {
        std::string op = tree->rule[2];

        code(tree->children[0]);
        push("3");
        code(tree->children[2]);
        pop("5");

        if (op == "LT")
        {
            codegen.push_back("slt $3, $5, $3");
        }
        else if (op == "EQ")
        {
            codegen.push_back("slt $6, $3, $5");
            codegen.push_back("slt $7, $5, $3");
            codegen.push_back("add $3, $6, $7");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "NE")
        {
            codegen.push_back("slt $6, $3, $5");
            codegen.push_back("slt $7, $5, $3");
            codegen.push_back("add $3, $6, $7");
        }
        else if (op == "LE")
        {
            codegen.push_back("slt $3, $3, $5");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "GE")
        {
            codegen.push_back("slt $3, $5, $3");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "GT")
        {
            codegen.push_back("slt $3, $3, $5");
        }
    }

    else if (tree->rule[0] == "expr")
    {
        if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
        }
        else if (tree->rule.size() == 4)
        {
            code(tree->children[0]);
            push("3");
            code(tree->children[2]);
            pop("5");

            if (tree->rule[2] == "PLUS")
            {
                codegen.push_back("add $3, $5, $3");
            }
            else if (tree->rule[2] == "MINUS")
            {
                codegen.push_back("sub $3, $5, $3");
            }
            else
            {
                throw Error("todo: not PLUS or MINUS");
            }
        }
        else
        {
            throw Error("todo: implement expr rules");
        }
    }
    else if (tree->rule[0] == "term")
    {
        if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
        }
        else if (tree->rule.size() == 4)
        {

            code(tree->children[0]);
            push("3");
            code(tree->children[2]);
            pop("5");

            if (tree->rule[2] == "STAR")
            {
                codegen.push_back("mult $5, $3");
                codegen.push_back("mflo $3");
            }
            else if (tree->rule[2] == "SLASH")
            {
                codegen.push_back("div $5, $3");
                codegen.push_back("mflo $3");
            }
            else if (tree->rule[2] == "PCT")
            {
                codegen.push_back("div $5, $3");
                codegen.push_back("mfhi $3");
            }
            else
            {
                throw Error("todo: not START/SLASH/PCT");
            }
        }

        else
        {
            throw Error("todo: implement term rules");
        }
    }
    else if (tree->rule[0] == "factor")
    {
        if (tree->rule.size() == 2)
        {
            if (tree->rule[1] == "ID")
            {
                int fp_offset = fp_offsets[tree->children[0]->rule[1]];
                std::string offset;
                if (fp_offset == 0)
                {
                    offset = "0";
                }
                else
                {
                    offset = "-" + std::to_string(fp_offset);
                }

                codegen.push_back("lw $3, " + offset + "($29)");
            }
            else if (tree->rule[1] == "NUM")
            {
                codegen.push_back("lis $3");
                codegen.push_back(".word " + tree->children[0]->rule[1]);
            }
            else
            {
                throw Error("todo: implement factor rules");
            }
        }
        else if (tree->rule[1] == "LPAREN")
        {
            code(tree->children[1]);
        }
        else
        {
            throw Error("todo: implement factor rules");
        }
    }

    // else if (tree->rule[0] == "") {}
    else
    {
        throw Error("undefined rule " + tree->rule[0]);
    }

    // for (auto token : tree->rule)
    // {
    //     std::cout << token << " ";
    // }

    // std::cout << "type: " << tree->type << " ";
    // std::cout << "n_children: " << tree->n_children << " ";
    // std::cout << std::endl;

    // for (auto child : tree->children)
    // {
    //     code(child);
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
        // print_symbol_table();
        // print_tree(tree, 0);
        prologue();
        code(tree);
        epilogue();

        // std::cout << std::endl
        //   << std::endl;

        // for (auto entry : fp_offsets)
        // {
        // std::cout << entry.first << ": " << entry.second << std::endl;
        // }

        // std::cout << std::endl
        //   << std::endl;

        for (auto line : codegen)
        {
            std::cout << line << std::endl;
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

// procedures → procedure procedures
// procedure → INT ID LPAREN params RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
// params →
// params → paramlist
// paramlist → dcl
// paramlist → dcl COMMA paramlist
// dcls → dcls dcl BECOMES NULL SEMI
// dcl → type ID
// statement → IF LPAREN test RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE
// statement → WHILE LPAREN test RPAREN LBRACE statements RBRACE
// statement → DELETE LBRACK RBRACK expr SEMI
// test → expr EQ expr
// test → expr NE expr
// test → expr LT expr
// test → expr LE expr
// test → expr GE expr
// test → expr GT expr
// term → factor
// factor → NULL
// factor → AMP lvalue
// factor → STAR factor
// factor → NEW INT LBRACK expr RBRACK
// factor → ID LPAREN RPAREN
// factor → ID LPAREN arglist RPAREN
// arglist → expr
// arglist → expr COMMA arglist

// lvalue → STAR factor
// lvalue → LPAREN lvalue RPAREN

// g++ -g -std=c++14 -Wall wlp4gen.cc -o wlp4gen
// cat test5.wlp4 | wlp4scan | wlp4parse | ./wlp4gen | cs241.linkasm > output.merl
// cs241.linker output.merl print.merl | cs241.merl 0 > foo.mips
// mips.twoints foo.mips
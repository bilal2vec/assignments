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
std::string first_var_type;
bool init_called = false;

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

Tree *unwrap_lvalue(Tree *tree)
{
    if (tree->rule[1] == "LPAREN")
    {
        return unwrap_lvalue(tree->children[1]);
    }
    else
    {
        return tree;
    }
}

std::string get_offset(std::string variable_name)
{
    std::string offset_str;

    int offset = fp_offsets[variable_name];

    if (offset == 0)
    {
        offset_str = "0";
    }
    else if (offset > 0)
    {
        offset_str = "-" + std::to_string(offset);
    }
    else
    {
        offset_str = std::to_string(offset * -1);
    }

    return offset_str;
}

void code(Tree *tree)
{
    if (tree->rule[0] == "start")
    {
        code(tree->children[1]);
    }
    else if (tree->rule[0] == "procedures")
    {

        if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
        }
        else if (tree->rule.size() == 3)
        {
            code(tree->children[1]); // procedures
            code(tree->children[0]); // procedure
        }
        else
        {
            throw Error("not in procuedunes rule");
        }
    }
    else if (tree->rule[0] == "procedure")
    {
        //reset frame pointer for each procedure
        fp_offsets = std::map<std::string, int>();
        fp_max_offset = -4;

        std::string name = tree->children[1]->rule[1];

        codegen.push_back("Q" + name + ":");
        codegen.push_back("sub $29, $30, $4");
        push("5");
        push("6");
        push("7");

        code(tree->children[3]); // params

        fp_max_offset = 12;

        code(tree->children[6]); // dcls
        code(tree->children[7]); // statements
        code(tree->children[9]); // expr
        pop("7");
        pop("6");
        pop("5");
        codegen.push_back("add $30, $29, $4");
        codegen.push_back("jr $31");

        // for (auto entry : fp_offsets)
        // {
        //     std::cout << entry.first << ": " << entry.second << std::endl;
        // }

        // std::cout << std::endl;
    }
    else if (tree->rule[0] == "main")
    {

        codegen.push_back(".import print");
        codegen.push_back(".import init");
        codegen.push_back(".import new");
        codegen.push_back(".import delete");
        codegen.push_back("lis $4");
        codegen.push_back(".word 4");
        codegen.push_back("lis $10");
        codegen.push_back(".word print");
        codegen.push_back("lis $11");
        codegen.push_back(".word 1");
        codegen.push_back("sub $29, $30, $4");

        //reset frame pointer for each procedure
        fp_offsets = std::map<std::string, int>();
        fp_max_offset = 0;

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

        first_var_type = tree->children[3]->get_type();

        if (init_called == false)
        {
            if (first_var_type == "int")
            {
                push("2");
                codegen.push_back("add $2, $0, $0");
                codegen.push_back("lis $5");
                codegen.push_back(".word init");
                push("31");
                codegen.push_back("jalr $5");
                pop("31");
                pop("2");
            }
            else if (first_var_type == "int*")
            {
                codegen.push_back("lis $5");
                codegen.push_back(".word init");
                push("31");
                codegen.push_back("jalr $5");
                pop("31");
            }
            else
            {
                throw Error("main function must take a first paramter of type int or int*");
            }

            init_called = true;
        }

        code(tree->children[8]);  // dcls
        code(tree->children[9]);  // statements
        code(tree->children[11]); // expr

        for (int i = 0; i < sp_pushes; i++)
        {
            codegen.push_back("add $30, $30, $4");
        }

        codegen.push_back("jr $31");

        // for (auto entry : fp_offsets)
        // {
        //     std::cout << entry.first << ": " << entry.second << std::endl;
        // }

        // std::cout << std::endl;
    }
    else if (tree->rule[0] == "params")
    {
        if (tree->rule.size() == 1)
        {
            return;
        }
        else if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
        }
        else
        {
            throw Error("not in params rule");
        }
    }
    else if (tree->rule[0] == "paramlist")
    {
        if (tree->rule.size() == 2)
        {
            std::string variable_name;
            variable_name = tree->children[0]->children[1]->rule[1];

            fp_offsets[variable_name] = fp_max_offset;

            fp_max_offset -= 4;
        }
        else if (tree->rule.size() == 4)
        {
            code(tree->children[2]);

            std::string variable_name;
            variable_name = tree->children[0]->children[1]->rule[1];

            fp_offsets[variable_name] = fp_max_offset;

            fp_max_offset -= 4;
        }
        else
        {
            throw Error("not in paramlist rule");
        }
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
            else if (tree->rule[4] == "NULL")
            {
                code(tree->children[0]);
                std::string variable_name;

                variable_name = tree->children[1]->children[1]->rule[1];
                fp_offsets[variable_name] = fp_max_offset;
                codegen.push_back("sw $11, -" + std::to_string(fp_max_offset) + "($29)");
                codegen.push_back("sub $30, $30, $4");
                sp_pushes += 1;
                fp_max_offset += 4;
            }
            else
            {
                throw Error("not in dcls rules");
            }
        }
        else
        {
            throw Error("not in dcls rules");
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
            Tree *unwrapped_lvalue = unwrap_lvalue(tree->children[0]);

            std::string lvalue_type = unwrapped_lvalue->rule[1];
            std::string variable_name;

            if (lvalue_type == "ID")
            {
                code(tree->children[2]); // expr
                variable_name = unwrapped_lvalue->children[0]->rule[1];
                std::string offset = get_offset(variable_name);
                codegen.push_back("sw $3, " + offset + "($29)");
            }
            else if (lvalue_type == "STAR")
            {
                code(tree->children[2]); // expr
                push("3");
                code(tree->children[0]); // lvalue
                pop("5");
                codegen.push_back("sw $5, 0($3)");
            }
            else
            {
                throw Error("unknown lvalue rule");
            }
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
            codegen.push_back("Qloop" + std::to_string(label_unique_value) + ":");
            code(tree->children[2]);
            codegen.push_back("beq $3, $0, QendWhile" + std::to_string(label_unique_value));
            code(tree->children[5]);
            codegen.push_back("beq $0, $0, Qloop" + std::to_string(label_unique_value));
            codegen.push_back("QendWhile" + std::to_string(label_unique_value) + ":");
        }
        else if (tree->rule.size() == 12)
        {
            int label_unique_value = codegen.size();
            code(tree->children[2]);
            codegen.push_back("beq $3, $0, Qelse" + std::to_string(label_unique_value));
            code(tree->children[5]);
            codegen.push_back("beq $0, $0, Qendif" + std::to_string(label_unique_value));
            codegen.push_back("Qelse" + std::to_string(label_unique_value) + ":");
            code(tree->children[9]);
            codegen.push_back("Qendif" + std::to_string(label_unique_value) + ":");
        }
        else if (tree->rule[1] == "DELETE")
        {
            code(tree->children[3]);
            int label_unique_value = codegen.size();
            codegen.push_back("beq $3, $11, QskipDelete" + std::to_string(label_unique_value));
            codegen.push_back("add $1, $3, $0");
            push("31");
            codegen.push_back("lis $5");
            codegen.push_back(".word delete");
            codegen.push_back("jalr $5");
            pop("31");
            codegen.push_back("QskipDelete" + std::to_string(label_unique_value) + ":");
        }
        else
        {
            throw Error("statmenet error");
        }
    }

    else if (tree->rule[0] == "test")
    {
        std::string op = tree->rule[2];

        code(tree->children[0]);
        push("3");
        code(tree->children[2]);
        pop("5");

        std::string expr_type = tree->children[0]->type;

        std::string cmp_cmd;
        if (expr_type == "int")
        {
            cmp_cmd = "slt";
        }
        else if (expr_type == "int*")
        {
            cmp_cmd = "sltu";
        }
        else
        {
            throw Error("test only supports int or int*");
        }

        if (op == "LT")
        {
            codegen.push_back(cmp_cmd + " $3, $5, $3");
        }
        else if (op == "EQ")
        {
            codegen.push_back(cmp_cmd + " $6, $3, $5");
            codegen.push_back(cmp_cmd + " $7, $5, $3");
            codegen.push_back("add $3, $6, $7");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "NE")
        {
            codegen.push_back(cmp_cmd + " $6, $3, $5");
            codegen.push_back(cmp_cmd + " $7, $5, $3");
            codegen.push_back("add $3, $6, $7");
        }
        else if (op == "LE")
        {
            codegen.push_back(cmp_cmd + " $3, $3, $5");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "GE")
        {
            codegen.push_back(cmp_cmd + " $3, $5, $3");
            codegen.push_back("sub $3, $11, $3");
        }
        else if (op == "GT")
        {
            codegen.push_back(cmp_cmd + " $3, $3, $5");
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

            std::string expr_type = tree->children[0]->type;
            std::string term_type = tree->children[2]->type;

            if (tree->rule[2] == "PLUS")
            {

                if ((expr_type == "int") && (term_type == "int"))
                {
                    code(tree->children[0]);
                    push("3");
                    code(tree->children[2]);
                    pop("5");
                    codegen.push_back("add $3, $5, $3");
                }
                else if ((expr_type == "int*") & (term_type == "int"))
                {
                    code(tree->children[0]);
                    push("3");
                    code(tree->children[2]);
                    codegen.push_back("mult $3, $4");
                    codegen.push_back("mflo $3");
                    pop("5");
                    codegen.push_back("add $3, $5, $3");
                }
                else if ((expr_type == "int") & (term_type == "int*"))
                {
                    code(tree->children[2]);
                    push("3");
                    code(tree->children[0]);
                    codegen.push_back("mult $3, $4");
                    codegen.push_back("mflo $3");
                    pop("5");
                    codegen.push_back("add $3, $5, $3");
                }
                else
                {
                    throw Error("expr add error");
                }
            }
            else if (tree->rule[2] == "MINUS")
            {
                if ((expr_type == "int") && (term_type == "int"))
                {
                    code(tree->children[0]);
                    push("3");
                    code(tree->children[2]);
                    pop("5");
                    codegen.push_back("sub $3, $5, $3");
                }
                else if ((expr_type == "int*") & (term_type == "int"))
                {
                    code(tree->children[0]);
                    push("3");
                    code(tree->children[2]);
                    codegen.push_back("mult $3, $4");
                    codegen.push_back("mflo $3");
                    pop("5");
                    codegen.push_back("sub $3, $5, $3");
                }
                else if ((expr_type == "int*") & (term_type == "int*"))
                {
                    code(tree->children[0]);
                    push("3");
                    code(tree->children[2]);
                    pop("5");
                    codegen.push_back("sub $3, $5, $3");
                    codegen.push_back("div $3, $4");
                    codegen.push_back("mflo $3");
                }
                else
                {
                    throw Error("expr minus error");
                }
            }
            else
            {
                throw Error(" not PLUS or MINUS");
            }
        }
        else
        {
            throw Error(" implement expr rules");
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
                throw Error("not START/SLASH/PCT");
            }
        }

        else
        {
            throw Error("not in term rules");
        }
    }
    else if (tree->rule[0] == "factor")
    {
        if (tree->rule.size() == 2)
        {
            if (tree->rule[1] == "ID")
            {
                std::string variable_name = tree->children[0]->rule[1];
                std::string offset = get_offset(variable_name);

                codegen.push_back("lw $3, " + offset + "($29)");
            }
            else if (tree->rule[1] == "NUM")
            {
                codegen.push_back("lis $3");
                codegen.push_back(".word " + tree->children[0]->rule[1]);
            }
            else if (tree->rule[1] == "NULL")
            {
                codegen.push_back("add $3, $0, $11");
            }
            else
            {
                throw Error("not in factor rules");
            }
        }
        else if (tree->rule[1] == "LPAREN")
        {
            code(tree->children[1]);
        }
        else if (tree->rule.size() == 3)
        {
            if (tree->rule[1] == "AMP")
            {
                std::string lvalue_type = tree->children[1]->rule[1];

                if (lvalue_type == "ID")
                {
                    codegen.push_back("lis $3");
                    std::string offset = get_offset(tree->children[1]->children[0]->rule[1]);
                    codegen.push_back(".word " + offset);
                    codegen.push_back("add $3, $3, $29");
                }
                else if (lvalue_type == "STAR")
                {
                    code(tree->children[1]->children[1]);
                }
                else
                {
                    throw Error("uknown lvalue rule");
                }
            }
            else if (tree->rule[1] == "STAR")
            {
                code(tree->children[1]);
                codegen.push_back("lw $3, 0($3)");
            }
            else
            {
                throw Error("unknown factor rules");
            }
        }
        else if (tree->rule[1] == "NEW")
        {
            code(tree->children[3]);
            codegen.push_back("add $1, $3, $0");
            push("31");
            codegen.push_back("lis $5");
            codegen.push_back(".word new");
            codegen.push_back("jalr $5");
            pop("31");
            codegen.push_back("bne $3, $0, 1");
            codegen.push_back("add $3, $11, $0");
        }
        else if ((tree->rule[1] == "ID") & (tree->rule.size() == 4))
        {
            push("29");
            push("31");
            codegen.push_back("lis $5");
            codegen.push_back(".word Q" + tree->children[0]->rule[1]);
            codegen.push_back("jalr $5");
            pop("31");
            pop("29");
        }
        else if ((tree->rule[1] == "ID") & (tree->rule.size() == 5))
        {
            int n_parameters = 0;
            Tree *temp = tree->children[2];
            while (true)
            {
                n_parameters += 1;

                if (temp->children.size() == 1)
                {
                    break;
                }

                temp = temp->children[2];
            }

            push("29");
            push("31");
            code(tree->children[2]);
            codegen.push_back("lis $5");
            codegen.push_back(".word Q" + tree->children[0]->rule[1]);
            codegen.push_back("jalr $5");

            for (int i = 0; i < n_parameters; i++)
            {
                pop("31");
            }

            pop("31");
            pop("29");
        }
        else
        {
            throw Error("not in factor rules");
        }
    }
    else if (tree->rule[0] == "arglist")
    {
        if (tree->rule.size() == 2)
        {
            code(tree->children[0]);
            push("3");
        }
        else if (tree->rule.size() == 4)
        {
            code(tree->children[0]);
            push("3");
            code(tree->children[2]);
        }
        else
        {
            throw Error("not in arglist rules");
        }
    }
    else if (tree->rule[0] == "lvalue")
    {
        if (tree->rule[1] == "ID")
        {
            std::string offset = get_offset(tree->children[0]->rule[1]);
            codegen.push_back("lis $3");
            codegen.push_back(".word " + offset);
            codegen.push_back("add $3, $3, $29");
        }
        else if (tree->rule[1] == "STAR")
        {
            code(tree->children[1]);
        }
        else if (tree->rule[1] == "LPAREN")
        {
            code(tree->children[1]);
        }
        else
        {
            throw Error("not in lvalue rules");
        }
    }

    // else if (tree->rule[0] == "") {}
    else
    {
        throw Error("undefined rule(s) " + tree->rule[0]);
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

        code(tree);

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

// g++ -g -std=c++14 -Wall wlp4gen.cc -o wlp4gen
// cat testcases/test1.wlp4 | wlp4scan | wlp4parse | ./wlp4gen | cs241.linkasm > output.merl
// cs241.linker output.merl print.merl alloc.merl | cs241.merl 0 > output.mips
// mips.twoints output.mips
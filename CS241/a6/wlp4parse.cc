#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <fstream>
#include <algorithm>

std::vector<std::tuple<std::string, int>> output;

class Node
{
public:
    std::string value;
    int n_children;
    std::vector<Node *> children;

    Node(std::string value, int n_children) : value(value), n_children(n_children) {}

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

Node *create_tree()
{
    std::string value = std::get<0>(output[0]);
    int n_children = std::get<1>(output[0]);

    output.erase(output.begin());

    Node *node = new Node(value, n_children);

    for (int i = 0; i < n_children; i++)
    {
        Node *child = create_tree();
        node->add_child(child);
    }

    std::reverse(node->children.begin(), node->children.end());

    return node;
}

void print_tree(Node *root)
{
    std::cout << root->value << std::endl;
    // std::cout << root->value << " " << root->n_children << " (";
    // for (int i = 0; i < root->n_children; i++)
    // {
    // std::cout << root->children[i]->value << ",";
    // }
    // std::cout << ")" << std::endl;

    for (int i = 0; i < root->n_children; i++)
    {
        print_tree(root->children[i]);
    }
}

class Token
{

public:
    std::string type;
    std::string value;

    Token(std::string type, std::string value);
    std::string get_type();
    std::string get_value();
};

Token::Token(std::string type, std::string value)
{
    type = type;
    value = value;
}

std::string Token::get_type() { return type; }
std::string Token::get_value() { return value; }

class Error
{
    std::string message;

public:
    Error(int token_number);

    const std::string &what() const;
};

Error::Error(int token_number)
{
    message = "ERROR at " + std::to_string(token_number - 1);
}

const std::string &Error::what() const { return message; }

int main()
{
    try
    {
        std::ifstream file("wlp4.rules");

        int n_rules = 49;
        std::map<int, std::vector<std::string>> rules;

        for (int i = 0; i < n_rules; i++)
        {
            std::string line;
            std::getline(file, line);

            std::stringstream ss(line);
            std::string rule_part;
            std::vector<std::string> rule;
            while (ss >> rule_part)
            {
                rule.push_back(rule_part);
            }

            rules[i] = rule;
        }

        int t = 863;
        std::map<std::pair<int, std::string>, int> Reduce;
        std::map<std::pair<int, std::string>, int> delta;

        for (int i = 0; i < t; i++)
        {
            std::string line;
            std::getline(file, line);

            std::stringstream ss(line);
            std::string token;

            std::vector<std::string> tokens;
            while (ss >> token)
            {
                tokens.push_back(token);
            }

            // for (auto token : tokens)
            // {
            //     std::cout << token << " ";
            // }
            // std::cout << std::endl;

            if (tokens[2] == "reduce")
            {
                int state = std::stoi(tokens[0]);
                std::string terminal = tokens[1];
                int rule = std::stoi(tokens[3]);

                Reduce[std::make_pair(state, terminal)] = rule;
            }
            else if (tokens[2] == "shift")
            {
                int state_1 = std::stoi(tokens[0]);
                std::string symbol = tokens[1];
                int state_2 = std::stoi(tokens[3]);

                delta[std::make_pair(state_1, symbol)] = state_2;
            }
        }

        file.close();

        // for (auto pair : Reduce)
        // {
        //     std::cout << pair.first.first << " " << pair.first.second << " " << pair.second << std::endl;
        // }

        // for (auto pair : delta)
        // {
        //     std::cout << pair.first.first << " " << pair.first.second << " " << pair.second << std::endl;
        // }

        std::string a, temp;
        std::vector<int> stateStack;
        std::vector<std::string> symStack;

        stateStack.push_back(0);

        int token_number = 0;

        std::vector<std::tuple<std::string, std::string>> templines;
        templines.push_back(std::make_pair("BOF", "BOF"));

        while (std::cin >> a >> temp)
        {
            templines.push_back(std::make_pair(a, temp));
        }

        templines.push_back(std::make_pair("EOF", "EOF"));

        for (auto el : templines)
        {

            std::string a = std::get<0>(el);
            std::string temp = std::get<1>(el);

            token_number += 1;
            while (true)
            {
                if (Reduce.find(std::make_pair(stateStack[0], a)) != Reduce.end())
                {
                    int rule = Reduce[std::make_pair(stateStack[0], a)];
                    std::vector<std::string> rule_vector = rules[rule];

                    std::string B = rule_vector[0];
                    std::vector<std::string> gamma(rule_vector.begin() + 1, rule_vector.end());

                    int n = gamma.size();

                    std::string tempout = "";

                    for (auto token : rule_vector)
                    {
                        tempout += token + " ";
                        // std::cout << token << " ";
                    }
                    // std::cout << std::endl;
                    output.push_back(std::make_tuple(tempout, rule_vector.size() - 1));

                    for (int i = 0; i < n; i++)
                    {
                        symStack.erase(symStack.begin());
                        stateStack.erase(stateStack.begin());
                    }

                    symStack.insert(symStack.begin(), B);
                    stateStack.insert(stateStack.begin(), delta[std::make_pair(stateStack[0], B)]);
                }
                else
                {
                    break;
                }
            }

            // std::cout << a << " " << temp << std::endl;
            std::string tempout = a + " " + temp;
            output.push_back(std::make_tuple(tempout, 0));

            symStack.insert(symStack.begin(), a);
            if (delta.find(std::make_pair(stateStack[0], a)) == delta.end())
            {
                throw Error(token_number);
            }

            stateStack.insert(stateStack.begin(), delta[std::make_pair(stateStack[0], a)]);
        }

        std::string tempout = "";
        for (auto token : rules[0])
        {
            tempout += token + " ";
            // std::cout << token << " ";
        }
        // std::cout << std::endl;
        output.push_back(std::make_tuple(tempout, rules[0].size() - 1));

        std::reverse(output.begin(), output.end());

        Node *root = create_tree();

        print_tree(root);

        // for (auto el : output)
        // {
        //     std::cout << std::get<0>(el) << " " << std::get<1>(el) << std::endl;
        // }

        delete root;
    }

    catch (Error &f)
    {
        std::cerr << f.what() << std::endl;
        return 1;
    }

    return 0;
}

// Node *create_tree()
// {
//     int value, n_children;
//     std::cin >> value >> n_children;

//     Node *node = new Node(value, n_children);

//     for (int i = 0; i < n_children; i++)
//     {
//         Node *child = create_tree();
//         node->add_child(child);
//     }

//     return node;
// }

#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>

class Error
{
    std::string message;

public:
    Error(int token_number);

    const std::string &what() const;
};

Error::Error(int token_number)
{
    message = "ERROR at " + std::to_string(token_number);
}

const std::string &Error::what() const { return message; }

int main()
{
    try
    {
        int n_terminals;
        std::cin >> n_terminals;
        std::vector<std::string> terminals;
        for (int i = 0; i < n_terminals; i++)
        {
            std::string terminal;
            std::cin >> terminal;
            terminals.push_back(terminal);
        }

        int n_nonterminals;
        std::cin >> n_nonterminals;
        std::vector<std::string> non_terminals;
        for (int i = 0; i < n_nonterminals; i++)
        {
            std::string non_terminal;
            std::cin >> non_terminal;
            non_terminals.push_back(non_terminal);
        }

        std::string start_symbol;
        std::cin >> start_symbol;

        int n_rules;
        std::cin >> n_rules;
        std::map<int, std::vector<std::string>> rules;

        // eat up rest of line
        std::string line;
        std::getline(std::cin, line);

        for (int i = 0; i < n_rules; i++)
        {
            std::string line;
            std::getline(std::cin, line);

            std::stringstream ss(line);
            std::string rule_part;
            std::vector<std::string> rule;
            while (ss >> rule_part)
            {
                rule.push_back(rule_part);
            }

            rules[i] = rule;
        }

        int s, t;
        std::cin >> s >> t;

        std::map<std::pair<int, std::string>, int> Reduce;
        std::map<std::pair<int, std::string>, int> delta;

        // eat up rest of line
        std::getline(std::cin, line);

        for (int i = 0; i < t; i++)
        {
            std::string line;
            std::getline(std::cin, line);

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

        // for (auto pair : Reduce)
        // {
        //     std::cout << pair.first.first << " " << pair.first.second << " " << pair.second << std::endl;
        // }

        // for (auto pair : delta)
        // {
        //     std::cout << pair.first.first << " " << pair.first.second << " " << pair.second << std::endl;
        // }

        std::string a;
        std::vector<int> stateStack;
        std::vector<std::string> symStack;

        stateStack.push_back(0);

        int token_number = 0;

        while (std::cin >> a)
        {
            token_number += 1;
            // std::cout << "input: " << a << std::endl;
            while (true)
            {
                if (Reduce.find(std::make_pair(stateStack[0], a)) != Reduce.end())
                {
                    int rule = Reduce[std::make_pair(stateStack[0], a)];
                    std::vector<std::string> rule_vector = rules[rule];

                    std::string B = rule_vector[0];
                    std::vector<std::string> gamma(rule_vector.begin() + 1, rule_vector.end());

                    int n = gamma.size();

                    for (auto token : rule_vector)
                    {
                        std::cout << token << " ";
                    }
                    std::cout << std::endl;

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

            symStack.insert(symStack.begin(), a);
            if (delta.find(std::make_pair(stateStack[0], a)) == delta.end())
            {
                throw Error(token_number);
            }

            stateStack.insert(stateStack.begin(), delta[std::make_pair(stateStack[0], a)]);
        }

        for (auto token : rules[0])
        {
            std::cout << token << " ";
        }
        std::cout << std::endl;
    }

    catch (Error &f)
    {
        std::cerr << f.what() << std::endl;
        return 1;
    }

    return 0;
}

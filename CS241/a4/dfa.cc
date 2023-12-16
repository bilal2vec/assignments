
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <map>
#include <sstream>
#include <algorithm>

int main() {

    std::vector<std::string> letters = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};  

    int n;
    std::cin >> n;

    std::vector<std::string> alphabet;
    for (int i = 0; i < n; i++) {
        std::string s;
        std::cin >> s;

        alphabet.push_back(s);
    }

    int m;
    std::cin >> m;

    std::vector<std::string> states;
    for (int i = 0; i < m; i++) {
        std::string s;
        std::cin >> s;

        states.push_back(s);
    }

    std::string initial_state;
    std::cin >> initial_state;

    int q;
    std::cin >> q;

    std::vector<std::string> accepting_states;
    for (int i = 0; i < q; i++) {
        std::string s;
        std::cin >> s;

        accepting_states.push_back(s);
    }

    int r;
    std::cin >> r;
    std::map<std::string, std::map<std::string, std::string>> mappings;
    for (int i = 0; i < r; i++) {
        std::string st1, sym, st2;
        
        std::cin >> st1;
        std::cin >> sym;
        std::cin >> st2;

        if (mappings.find(st1) == mappings.end()) {
            std::map<std::string, std::string> mapping;
            mapping[sym] = st2;
            mappings[st1] = mapping;
        } else {
            mappings[st1][sym] = st2;
        }
    }

    std::string ignore;
    std::getline(std::cin, ignore);

    std::string line;
    while(std::getline(std::cin, line)) {
        std::stringstream line2(line);
        
        std::string current_state = initial_state;
        
        std::string temp;
        while(line2 >> temp) {
            // std::cout << current_state << " ";
            std::map<std::string, std::string> possible_transitions = mappings[current_state];

            if (possible_transitions.find(temp) == possible_transitions.end()) {
                current_state = "NOT IN ACCEPTING STATES";
                break;
            } else {
                current_state = possible_transitions[temp];
            }
        }
        if (std::find(accepting_states.begin(), accepting_states.end(), current_state) != accepting_states.end()) {
            std::cout << "true" << std::endl;
        } else {
            std::cout << "false" << std::endl;
        }
    }

    return 0;
}

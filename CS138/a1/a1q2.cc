#include <iostream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;


int main(int argc, char* argv[]){

    int N;
    char command;
    vector<string> lines {};

    cin >> N;

    if (N <= 0) {
        cerr << "Error, line length must be positive." << endl;
        exit(1);
    }
    cin >> command;
    if ((command != 'f') && (command != 'r') && (command != 'g'))
    {
        cerr << "Error, command is illegal." << endl;
        exit(1);
    }

    string line;

    getline(cin, line);

    while(getline(cin, line)) {
        lines.push_back(line);
    }

    if (command == 'f') {
        for (int i = 0; i < lines.size(); ++i) {
            string line = lines[i];
            string line2 = line.substr(0, N);
            cout << line2 << endl;
        }
    } else if (command == 'r') {
        for (int i = lines.size() - 1; i >= 0; --i) {
            string line = lines[i];
            string line2 = line.substr(0, N);
            cout << line2 << endl;
        }
    } else if (command == 'g') {
        for (int i = 0; i < lines.size(); ++i) {
            string line = lines[i];
            if (line.find("fnord", 0) != -1) {
                string line2 = line.substr(0, N);
                cout << line2 << endl;
            }
        }

    }


    return 0;
}
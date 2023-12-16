#include <iostream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;


int main(int argc, char* argv[]){

    int N;
    vector<string> lines {};

    cin >> N;

    if (N <= 0) {
        cerr << "Error, line length must be positive." << endl;
        exit(1);
    }

    string line;

    getline(cin, line);

    while(getline(cin, line)) {
        lines.push_back(line);
    }

    for (int i = 0; i < lines.size(); ++i) {
        string line = lines[i];
        string line2 = line.substr(0, N);
        cout << line2 << endl;
    }

    return 0;
}
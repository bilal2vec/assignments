#include <iostream>
#include <string>

using namespace std;

void printReverseRecursive () {
    string temp;

    if (cin >> temp) {
        printReverseRecursive();
    }

    if (temp != "") {
        cout << temp << endl;
    }

}

int main(int argc, char* argv[]){

    printReverseRecursive();

    return 0;
}
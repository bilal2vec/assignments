#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

struct Node{
    string val;
    Node* next;
};

void printPairInOrder (Node* p1, Node* p2) {
    assert((p1 != nullptr) && (p2 != nullptr));

    if (p1->val < p2->val) {
        cout << p1->val << endl << p2->val << endl;
    } else {
        cout << p2->val << endl << p1->val << endl;
    }
}

int main(int argc, char* argv[]){

    Node* one = new Node {};
    one->val = "one";
    one->next = nullptr; 

    Node* two = new Node {};
    two->val = "two";
    two->next = nullptr; 

    printPairInOrder(one, two);

    Node* three = new Node {};
    three->val = "three";
    three->next = nullptr; 

    Node* four = new Node {};
    four->val = "four";
    four->next = nullptr; 

    printPairInOrder(three, four);

    delete one;
    delete two;

    delete three;
    delete four;

    return 0;
}
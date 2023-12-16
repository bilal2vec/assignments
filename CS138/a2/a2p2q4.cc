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

void printList (Node* p) {
    Node* curr = p;
    while(curr != nullptr) {
        cout << curr->val << endl;;
        curr = curr->next;
    }
}

Node *sortPair (Node *p1, Node *p2) {
    assert((p1 != nullptr) && (p2 != nullptr));

    if (p1->val < p2->val) {
        p1->next = p2;
        p2->next = nullptr;

        return p1;
    } else {
        p2->next = p1;
        p1->next = nullptr;

        return p2;
    }
}

int main(int argc, char* argv[]){

    Node* one = new Node {};
    one->val = "one";
    one->next = nullptr; 

    Node* two = new Node {};
    two->val = "two";
    two->next = nullptr; 

    Node* list = sortPair(one, two);
    printList(list);

    Node* three = new Node {};
    three->val = "three";
    three->next = nullptr; 

    Node* four = new Node {};
    four->val = "four";
    four->next = nullptr; 

    Node* list2 = sortPair(three, four);
    printList(list2);

    delete one;
    delete two;

    delete three;
    delete four;

    return 0;
}
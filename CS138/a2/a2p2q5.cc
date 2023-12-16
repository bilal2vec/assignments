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

Node* makePairList (string s1, string s2) {
    if (s1 < s2) {
        Node* one = new Node {};
        one->val = s1;
        one->next = nullptr; 

        Node* two = new Node {};
        two->val = s2;
        two->next = nullptr; 

        one->next = two;
        return one;
    } else {
        Node* one = new Node {};
        one->val = s2;
        one->next = nullptr; 

        Node* two = new Node {};
        two->val = s1;
        two->next = nullptr; 

        one->next = two;
        return one;
    }
}

int main(int argc, char* argv[]){

    Node* list = makePairList("one", "two");

    printList(list);

    Node* list2 = makePairList("b", "a");

    printList(list2);

    delete list->next;
    delete list;

    delete list2->next;
    delete list2;

    return 0;
}
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

int main(int argc, char* argv[]){

    Node* head = new Node {};
    head->val = "one";
    head->next = nullptr; 

    Node* prev = head;

    Node* node1 = new Node {};
    node1->val = "two";
    node1->next = nullptr; 

    prev->next = node1;
    prev = node1;

    Node* node2 = new Node {};
    node2->val = "three";
    node2->next = nullptr; 

    prev->next = node2;
    prev = node2;

    printList(head);

    delete head;
    delete node1;
    delete node2;

    return 0;
}
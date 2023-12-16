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

Node *makeList () {
    string word;
    cin >> word;
    
    if (word == "") {
        return nullptr;
    }

    Node* head = new Node {};
    head->val = word;
    head->next = nullptr; 
    
    Node* prev = head;

    while(cin >> word) {
        Node* node = new Node {};
        node->val = word;
        node->next = nullptr;

        prev->next = node;
        prev = node;
    }

    return head;
}

int main(int argc, char* argv[]){

    Node* head = makeList();

    delete head->next->next;
    delete head->next;
    delete head;

    return 0;
}
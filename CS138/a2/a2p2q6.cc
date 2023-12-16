#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

struct Node
{
    string val;
    Node *next;
};

void printList(Node *p)
{
    Node *curr = p;
    while (curr != nullptr)
    {
        cout << curr->val << endl;
        ;
        curr = curr->next;
    }
}

Node *makePairList(string s1, string s2)
{
    if (s1 < s2)
    {
        Node *one = new Node{};
        one->val = s1;
        one->next = nullptr;

        Node *two = new Node{};
        two->val = s2;
        two->next = nullptr;

        one->next = two;
        return one;
    }
    else
    {
        Node *one = new Node{};
        one->val = s2;
        one->next = nullptr;

        Node *two = new Node{};
        two->val = s1;
        two->next = nullptr;

        one->next = two;
        return one;
    }
}

Node *append(Node *p1, Node *p2)
{
    assert((p1 != nullptr));

    Node *p1_last = p1;
    while (p1_last->next != nullptr)
    {
        p1_last = p1_last->next;
    }
    p1_last->next = p2;

    return p1;
}

int main(int argc, char *argv[])
{

    Node *list1 = makePairList("a", "b");
    Node *list2 = makePairList("c", "d");

    Node *temp = append(list1, list2);

    printList(temp);

    delete list1->next;
    delete list1;

    delete list2->next;
    delete list2;

    return 0;
}
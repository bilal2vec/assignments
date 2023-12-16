#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct Node
{
    string val;
    Node *next;
    Node *prev;
};

struct Stew
{
    Node *first;
    Node *last;
};

void initStew(Stew &q)
{
    q.first = nullptr;
    q.last = nullptr;
}

bool isEmpty(const Stew &q)
{
    return nullptr == q.first;
}

void addFront(Stew &q, string val)
{
    Node *new_first = new Node{};
    new_first->val = val;
    new_first->next = nullptr;
    new_first->prev = nullptr;

    if (isEmpty(q))
    {
        q.first = new_first;
        q.last = new_first;
    }
    else
    {
        q.first->prev = new_first;
        new_first->next = q.first;
        q.first = new_first;
    }
}

void addBack(Stew &q, string val)
{
    Node *new_last = new Node{};
    new_last->val = val;
    new_last->next = nullptr;
    new_last->prev = nullptr;

    if (isEmpty(q))
    {
        q.first = new_last;
        q.last = new_last;
    }
    else
    {
        q.last->next = new_last;
        new_last->prev = q.last;
        q.last = new_last;
    }
}

void leaveFront(Stew &q)
{
    assert(!isEmpty(q));

    Node *first = q.first;

    if (q.first == q.last)
    {
        q.first = nullptr;
        q.last = nullptr;
    }
    else
    {
        q.first = first->next;
        q.first->prev = nullptr;
    }
    delete first;
}

string peekFront(const Stew &q)
{
    assert(!isEmpty(q));

    return q.first->val;
}

void leaveBack(Stew &q)
{
    assert(!isEmpty(q));

    Node *last = q.last;

    if (q.first == q.last)
    {
        q.first = nullptr;
        q.last = nullptr;
    }
    else
    {
        q.last = last->prev;
        q.last->next = nullptr;
    }

    delete last;
}

string peekBack(const Stew &q)
{
    assert(!isEmpty(q));

    return q.last->val;
}

void print(const Stew &q, char direction)
{
    if (direction == 'f')
    {
        Node *curr = q.first;

        while (curr != nullptr)
        {
            cout << curr->val << endl;
            curr = curr->next;
        }
    }
    else if (direction == 'r')
    {
        Node *curr = q.last;

        while (curr != nullptr)
        {
            cout << curr->val << endl;
            curr = curr->prev;
        }
    }
    else
    {
        cerr << "Error, illegal direction: " << direction << endl;
    }
}

void nuke(Stew &q)
{
    if (isEmpty(q))
    {
        if (q.first != nullptr)
        {
            delete q.first;
            q.first = nullptr;
        }

        if (q.last != nullptr)
        {
            delete q.last;
            q.last = nullptr;
        }
    }
    else
    {
        Node *curr = q.first;
        while (curr != nullptr)
        {
            Node *temp = curr;
            curr = curr->next;
            delete temp;
        }
        q.first = nullptr;
        q.last = nullptr;
    }
}

int main(int argc, char *argv[])
{
    Stew s1;
    initStew(s1);
    cout << "This prints \"\" across one lines\n";
    print(s1, 'f');
    addFront(s1, "alpha");
    addFront(s1, "beta");
    addFront(s1, "gamma");
    addBack(s1, "delta");
    cout << "This prints \"gamma beta alpha delta\" across four lines\n";
    print(s1, 'f');
    cout << "This prints \"delta alpha beta gamma\" across four lines\n";
    print(s1, 'r');
    leaveFront(s1);
    leaveBack(s1);
    cout << "This prints \"beta alpha\" in one line\n";
    cout << peekFront(s1) << " " << peekBack(s1) << endl;
    cout << "This nuke has no output, but is good form to call when done\n";
    nuke(s1);
    cout << "This assertion should succeed\n";
    assert(isEmpty(s1));
    cout << "Illegal direction should cause error mesg\n";
    print(s1, 'k');
}

#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct NodeChunk
{
    string *val;
    NodeChunk *next;
};

struct Stack
{
    int chunkSize;
    int topElt;
    NodeChunk *firstChunk;
};

NodeChunk *createNewNodeChunk(int chunkSize)
{

    if (chunkSize <= 0)
    {
        cerr << "Error, chunk size must be positive." << endl;
        assert(false);
    }
    else
    {
        NodeChunk *new_chunk = new NodeChunk{};
        new_chunk->val = new string[chunkSize];
        new_chunk->next = nullptr;

        return new_chunk;
    }
}
void initStack(int chunkSize, Stack &s)
{
    if (chunkSize <= 0)
    {
        cerr << "Error, chunk size must be positive." << endl;
        assert(false);
    }
    else
    {
        s.chunkSize = chunkSize;
        s.topElt = -1;
        s.firstChunk = nullptr;
    }
}
bool isEmpty(const Stack &s)
{
    if (s.firstChunk == nullptr)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void push(string val, Stack &s)
{
    if (isEmpty(s))
    {
        s.firstChunk = createNewNodeChunk(s.chunkSize);
        s.firstChunk->val[0] = val;

        s.topElt = 0;
    }
    else if (s.topElt == (s.chunkSize - 1))
    {
        NodeChunk *new_chunk = createNewNodeChunk(s.chunkSize);
        new_chunk->next = s.firstChunk;
        new_chunk->val[0] = val;

        s.firstChunk = new_chunk;
        s.topElt = 0;
    }
    else
    {
        s.firstChunk->val[s.topElt + 1] = val;
        s.topElt += 1;
    }
}

void pop(Stack &s)
{
    assert(!isEmpty(s));

    if (s.topElt == 0)
    {
        NodeChunk *new_first_chunk = s.firstChunk->next;

        delete[] s.firstChunk->val;
        delete s.firstChunk;

        s.firstChunk = new_first_chunk;
        s.topElt = (s.chunkSize - 1);
    }
    else
    {
        s.firstChunk->val[s.topElt] = "";
        s.topElt -= 1;
    }
}

string top(const Stack &s)
{
    assert(!isEmpty(s));

    return s.firstChunk->val[s.topElt];
}

void swap(Stack &s)
{
    assert(!isEmpty(s));

    if (s.topElt == 0)
    {
        if (s.firstChunk->next != nullptr)
        {
            string temp = s.firstChunk->val[0];
            s.firstChunk->val[0] = s.firstChunk->next->val[s.chunkSize - 1];
            s.firstChunk->next->val[s.chunkSize - 1] = temp;
        }
        else
        {
            assert(false);
        }
    }
    else if (s.topElt > 0)
    {
        string temp = s.firstChunk->val[s.topElt];
        s.firstChunk->val[s.topElt] = s.firstChunk->val[s.topElt - 1];
        s.firstChunk->val[s.topElt - 1] = temp;
    }
    else
    {
        assert(false);
    }
}

int size(const Stack &s)
{
    if (isEmpty(s))
    {
        return 0;
    }
    else
    {
        int count = 0;
        count += (s.topElt + 1);

        NodeChunk *curr = s.firstChunk->next;
        while (curr != nullptr)
        {
            count += s.chunkSize;
            curr = curr->next;
        }

        return count;
    }
}

int main(int argc, char *argv[])
{
    Stack s1;
    initStack(1, s1);

    cout << "size of s1: " << size(s1) << endl;

    push("1", s1);
    push("22", s1);
    push("333", s1);
    push("4444", s1);

    cout << "size of s1: " << size(s1) << endl;
    cout << "top: " << top(s1) << endl;

    // cout << s1.firstChunk->val[0] << endl;
    // cout << s1.firstChunk->val[1] << endl;
    // cout << s1.firstChunk->next->val[0] << endl;
    // cout << s1.firstChunk->next->val[1] << endl;
    cout << s1.firstChunk->val[0] << endl;
    cout << s1.firstChunk->next->val[0] << endl;
    cout << s1.firstChunk->next->next->val[0] << endl;
    cout << s1.firstChunk->next->next->next->val[0] << endl;

    cout << "swap" << endl;
    swap(s1);

    // cout << s1.firstChunk->val[0] << endl;
    // cout << s1.firstChunk->val[1] << endl;
    // cout << s1.firstChunk->next->val[0] << endl;
    // cout << s1.firstChunk->next->val[1] << endl;
    cout << s1.firstChunk->val[0] << endl;
    cout << s1.firstChunk->next->val[0] << endl;
    cout << s1.firstChunk->next->next->val[0] << endl;
    cout << s1.firstChunk->next->next->next->val[0] << endl;

    pop(s1);
    pop(s1);

    cout << "pop" << endl;
    cout << "size of s1: " << size(s1) << endl;
    cout << "top: " << top(s1) << endl;

    // cout << s1.firstChunk->val[0] << endl;
    // cout << s1.firstChunk->val[1] << endl;
    cout << s1.firstChunk->val[0] << endl;
    cout << s1.firstChunk->next->val[0] << endl;

    pop(s1);
    pop(s1);

    return 0;
}

#include <iostream>
#include <stdlib.h>
#include <deque>

using namespace std;

// Implementation 1 starts here
struct Node
{
    int priority;
    int timestamp;
    Node *next;
};

Node *insertLL(int priority, int timestamp, Node *head)
{
    Node *new_node = new Node;
    new_node->priority = priority;
    new_node->timestamp = timestamp;
    new_node->next = nullptr;

    if (head == nullptr)
    {
        return new_node;
    }
    else
    {
        if (new_node->priority > head->priority)
        {
            new_node->next = head;
            return new_node;
        }
        else
        {
            Node *curr = head;
            while (curr->next != nullptr && curr->next->priority >= new_node->priority)
            {
                curr = curr->next;
            }

            Node *temp = curr->next;
            curr->next = new_node;
            new_node->next = temp;
            return head;
        }
    }
}

Node *deleteLLMax(Node *head)
{
    if (head != nullptr)
    {
        Node *temp = head;
        head = head->next;
        cout << temp->priority << " " << temp->timestamp << endl;
        delete temp;
        return head;
    }
    return head;
}

void printLLMax(Node *head)
{
    if (head != nullptr)
    {
        cout << head->priority << " " << head->timestamp << endl;
    }
}

void printLLIth(Node *head, int i)
{
    Node *curr = head;
    for (int j = 0; j < i; ++j)
    {
        curr = curr->next;
    }

    cout << curr->timestamp << endl;
}

void deleteLL(Node *node)
{
    if (node != nullptr)
    {
        deleteLL(node->next);
        node->next = nullptr;
        delete node;
    }
}
// Implementation 1 ends here

// Implementation 2 starts here

struct DynVector
{
    pair<int, int> *array;
    int size;

    DynVector()
    {
        array = nullptr;
        size = 0;
    }

    void insert(int priority, int timestamp, int i)
    {
        if (array == nullptr)
        {
            array = (pair<int, int> *)malloc(sizeof(pair<int, int>));
            size = 1;

            array[0] = make_pair(0, 0);
        }

        if (i >= size)
        {
            array = (pair<int, int> *)realloc(array, size * 2 * sizeof(pair<int, int>));
            size *= 2;

            for (int j = size / 2; j < size; ++j)
            {
                array[j] = make_pair(0, 0);
            }
        }

        pair<int, int> p = make_pair(priority, timestamp);

        array[i] = p;
    }

    void reset()
    {
        if (array != nullptr)
        {
            array = (pair<int, int> *)realloc(array, sizeof(pair<int, int>));
            size = 1;

            array[0] = make_pair(0, 0);
        }
    }

    ~DynVector()
    {
        if (array != nullptr)
        {
            free(array);
        }
    }
};

struct Heap
{
    DynVector v;
    int size;

    Heap()
    {
        size = 0;
    }

    int parent(int i)
    {
        if (i == 0)
        {
            return -1;
        }
        return (i - 1) / 2;
    }

    void fixup(int i)
    {
        while (parent(i) != -1 && v.array[parent(i)].first < v.array[i].first)
        {
            pair<int, int> temp = v.array[i];
            v.array[i] = v.array[parent(i)];
            v.array[parent(i)] = temp;

            i = parent(i);
        }
    }

    void insert(int priority, int timestamp)
    {
        size += 1;
        int l = size - 1;
        v.insert(priority, timestamp, l);

        fixup(l);
    }

    void fixdown(int i)
    {
        while ((2 * i + 1 < size) && (2 * i + 2 < size))
        {
            int j = 2 * i + 1;

            if ((2 * i + 2 < size) && v.array[2 * i + 2].first > v.array[j].first)
            {
                j = 2 * i + 2;
            }

            if (v.array[i].first >= v.array[j].first)
            {
                break;
            }

            pair<int, int> temp = v.array[i];
            v.array[i] = v.array[j];
            v.array[j] = temp;

            i = j;
        }
    }

    void deleteMax()
    {
        if (size == 0)
        {
            return;
        }

        int i = size - 1;

        pair<int, int> temp = v.array[0];
        v.array[0] = v.array[i];
        v.array[i] = temp;

        size -= 1;

        fixdown(0);

        cout << v.array[size].first << " " << v.array[size].second << endl;

        if (size == 0)
        {
            v.reset();
        }
    }
};

// Implementation 2 ends here

// Implementation 3 starts here

struct PriorityQueueVector
{
    deque<int> **array;
    int size;

    PriorityQueueVector()
    {
        array = nullptr;
        size = 0;
    }

    void insert(int priority, int timestamp)
    {
        if (array == nullptr)
        {
            array = (deque<int> **)malloc(sizeof(deque<int> **));
            size = 1;

            array[0] = nullptr;
        }

        if (priority >= size)
        {
            array = (deque<int> **)realloc(array, (priority + 1) * sizeof(deque<int> **));

            for (int j = size; j < priority + 1; ++j)
            {
                array[j] = nullptr;
            }

            size = priority + 1;
        }

        if (array[priority] == nullptr)
        {
            array[priority] = new deque<int>();
        }
        array[priority]->push_back(timestamp);
    }

    void deleteMax()
    {
        if (size == 0)
        {
            return;
        }

        if (size == 1 && array[0] == nullptr)
        {
            return;
        }

        if (array[size - 1] == nullptr)
        {
            return;
        }

        int timestamp = array[size - 1]->front();
        int priority = size - 1;
        array[size - 1]->pop_front();

        if (array[size - 1]->empty())
        {
            delete array[size - 1];
            array[size - 1] = nullptr;
            size -= 1;

            int new_size = size;
            for (int i = size; i >= 0; --i)
            {
                if (array[i] != nullptr)
                {
                    new_size = i + 1;
                    break;
                }
            }

            size = new_size;
            array = (deque<int> **)realloc(array, size * sizeof(deque<int> **));
        }

        cout << priority << " " << timestamp << endl;
    }

    void reset()
    {
        if (array != nullptr)
        {

            for (int i = 0; i < size; ++i)
            {
                if (array[i] != nullptr)
                {
                    delete array[i];
                }
            }

            array = (deque<int> **)realloc(array, sizeof(deque<int> **));
            size = 1;

            array[0] = nullptr;
        }
    }

    ~PriorityQueueVector()
    {
        if (array != nullptr)
        {
            for (int i = 0; i < size; ++i)
            {
                if (array[i] != nullptr)
                {
                    delete array[i];
                }
            }

            free(array);
        }
    }
};

// Implementation 3 ends here

int main()
{

    Node *LLHead = nullptr;
    Heap h = Heap();
    PriorityQueueVector pqv;

    int timestamp = 1;

    string command_name;

    while (cin >> command_name)
    {
        if (command_name == "i")
        {
            int num, priority;

            cin >> num >> priority;

            if (num == 1)
            {
                LLHead = insertLL(priority, timestamp, LLHead);
            }
            else if (num == 2)
            {
                h.insert(priority, timestamp);
            }
            else if (num == 3)
            {
                pqv.insert(priority, timestamp);
            }

            timestamp += 1;
        }
        else if (command_name == "d")
        {
            int num;

            cin >> num;

            if (num == 1)
            {
                LLHead = deleteLLMax(LLHead);
            }
            else if (num == 2)
            {
                h.deleteMax();
            }
            else if (num == 3)
            {
                pqv.deleteMax();
            }
        }
        else if (command_name == "f")
        {
            int num;

            cin >> num;

            if (num == 1)
            {
                printLLMax(LLHead);
            }
            else if (num == 2)
            {
                if (h.size != 0)
                {
                    cout << h.v.array[0].first << " " << h.v.array[0].second << endl;
                }
            }
            else if (num == 3)
            {
                if (pqv.size != 0)
                {
                    if (pqv.array[pqv.size - 1] != nullptr)
                    {
                        cout << pqv.size - 1 << " " << pqv.array[pqv.size - 1]->front() << endl;
                    }
                }
            }
        }
        else if (command_name == "l")
        {
            int num, i;

            cin >> num >> i;

            if (num == 1)
            {
                printLLIth(LLHead, i);
            }
            else if (num == 2)
            {
                cout << h.v.array[i].second << endl;
            }
            else if (num == 3)
            {
                if (pqv.array[i] != nullptr)
                {
                    for (int j = 0; j < pqv.array[i]->size(); ++j)
                    {
                        cout << pqv.array[i]->at(j) << " ";
                    }
                    cout << endl;
                }
            }
        }
        else if (command_name == "r")
        {
            deleteLL(LLHead);
            LLHead = nullptr;

            h.v.reset();
            h.size = 0;

            pqv.reset();
        }
        else if (command_name == "x")
        {
            deleteLL(LLHead);
            LLHead = nullptr;

            h.v.reset();
            h.size = 0;

            pqv.reset();

            break;
        }
    }

    return 0;
}
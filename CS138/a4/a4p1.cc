#include <iostream>
#include <string>
#include <cassert>
using namespace std;

struct Qnode
{
    string val;
    Qnode *next;
};

struct Queue
{
    Qnode *first;
    Qnode *last;
};

void initQ(Queue &q)
{
    q.first = nullptr;
    q.last = nullptr;
}

bool isEmptyQ(const Queue &q)
{
    return nullptr == q.first;
}

int sizeQ(const Queue &q)
{
    Qnode *curr = q.first;

    if (curr == nullptr)
    {
        return 0;
    }

    int counter = 1;
    while (curr->next)
    {
        counter += 1;
        curr = curr->next;
    }

    return counter;
}

void enterQ(Queue &q, string val)
{
    Qnode *new_node = new Qnode{};
    new_node->val = val;
    new_node->next = nullptr;

    if (isEmptyQ(q))
    {
        q.first = new_node;
        q.last = new_node;
    }
    else
    {
        q.last->next = new_node;
        q.last = new_node;
    }
}

string firstQ(const Queue &q)
{
    if (isEmptyQ(q))
    {
        cerr << "Error, queue is empty." << endl;
    }
    assert(!isEmptyQ(q));

    return q.first->val;
}

void leaveQ(Queue &q)
{

    if (isEmptyQ(q))
    {
        cerr << "Error, queue is empty." << endl;
    }

    assert(!isEmptyQ(q));

    if (q.first == q.last)
    {
        Qnode *last = q.last;
        q.first = nullptr;
        q.last = nullptr;

        delete last;
    }
    else
    {
        Qnode *temp = q.first;
        q.first = q.first->next;

        delete temp;

        // Qnode *last = nullptr;
        // Qnode *second_last = nullptr;

        // Qnode *curr = q.first;
        // while (curr->next)
        // {
        //     second_last = curr;
        //     last = curr->next;

        //     curr = curr->next;
        // }

        // q.last = second_last;
        // q.last->next = nullptr;

        // delete last;
    }
}

void printQ(const Queue &q)
{
    Qnode *curr = q.first;
    while (curr)
    {
        cout << curr->val << endl;
        curr = curr->next;
    }
}

struct PQnode
{
    int priority;
    Queue *q;
    PQnode *next;
};

typedef PQnode *PQ;

void initPQ(PQ &pq)
{
    pq = nullptr;
}

bool isEmptyPQ(const PQ &pq)
{
    return pq == nullptr;
}

void enterPQ(PQ &pq, string val, int priority)
{
    if (isEmptyPQ(pq))
    {
        Queue *new_queue = new Queue{};
        new_queue->first = nullptr;
        new_queue->last = nullptr;

        enterQ(*new_queue, val);

        PQnode *new_pq = new PQnode{};
        new_pq->priority = priority;
        new_pq->q = new_queue;
        new_pq->next = nullptr;

        pq = new_pq;
    }
    else
    {
        PQnode *curr = pq;

        while (curr)
        {
            if (curr->priority == priority)
            {
                enterQ(*(curr->q), val);

                break;
            }
            else if (curr->next && ((curr->priority < priority) && (curr->next->priority > priority)))
            {
                Queue *new_queue = new Queue{};
                new_queue->first = nullptr;
                new_queue->last = nullptr;

                enterQ(*new_queue, val);

                PQnode *new_pq = new PQnode{};
                new_pq->priority = priority;
                new_pq->q = new_queue;
                new_pq->next = nullptr;

                PQnode *temp = curr->next;
                curr->next = new_pq;
                new_pq->next = temp;

                break;
            }
            else if ((curr->next == nullptr) && (curr->priority < priority))
            {
                Queue *new_queue = new Queue{};
                new_queue->first = nullptr;
                new_queue->last = nullptr;

                enterQ(*new_queue, val);

                PQnode *new_pq = new PQnode{};
                new_pq->priority = priority;
                new_pq->q = new_queue;
                new_pq->next = nullptr;

                curr->next = new_pq;

                break;
            }

            curr = curr->next;
        }
    }
}

string firstPQ(const PQ &pq)
{
    if (isEmptyPQ(pq))
    {
        cerr << "Error, priority queue is empty." << endl;
    }

    assert(!isEmptyPQ(pq));

    return pq->q->first->val;
}

void leavePQ(PQ &pq)
{
    if (isEmptyPQ(pq))
    {

        cerr << "Error, priority queue is empty." << endl;
    }

    assert(!isEmptyPQ(pq));

    if (pq->q->first == pq->q->last)
    {
        leaveQ(*(pq->q));

        PQnode *temp = pq;
        pq = pq->next;
        delete temp;
    }
    else
    {
        leaveQ(*(pq->q));
    }
}

int sizeByPriority(const PQ &pq, int priority)
{
    if (isEmptyPQ(pq))
    {
        return 0;
    }

    PQnode *curr = pq;
    int count = 0;

    while (curr)
    {
        if (curr->priority == priority)
        {
            count += sizeQ(*(curr->q));
        }

        curr = curr->next;
    }

    return count;
}

int sizePQ(const PQ &pq)
{
    if (isEmptyPQ(pq))
    {
        return 0;
    }

    PQnode *curr = pq;
    int count = 0;

    while (curr)
    {
        count += sizeQ(*(curr->q));

        curr = curr->next;
    }

    return count;
}

int numPriorities(const PQ &pq)
{
    if (isEmptyPQ(pq))
    {
        return 0;
    }

    PQnode *curr = pq;
    int count = 0;

    while (curr)
    {
        count += 1;

        curr = curr->next;
    }

    return count;
}

int main()
{

    Queue q;
    initQ(q);

    assert(isEmptyQ(q));
    assert(sizeQ(q) == 0);

    enterQ(q, "one");
    assert(sizeQ(q) == 1);
    assert(firstQ(q) == "one");

    leaveQ(q);
    assert(isEmptyQ(q));
    assert(sizeQ(q) == 0);

    enterQ(q, "one");
    assert(sizeQ(q) == 1);
    assert(firstQ(q) == "one");

    enterQ(q, "two");
    assert(sizeQ(q) == 2);
    assert(firstQ(q) == "one");

    leaveQ(q);
    assert(!isEmptyQ(q));
    assert(sizeQ(q) == 1);

    enterQ(q, "three");
    assert(sizeQ(q) == 2);
    assert(firstQ(q) == "two");

    // printQ(q);

    leaveQ(q);
    assert(!isEmptyQ(q));
    assert(sizeQ(q) == 1);

    leaveQ(q);
    assert(isEmptyQ(q));
    assert(sizeQ(q) == 0);

    PQ pq;
    initPQ(pq);
    assert(isEmptyPQ(pq));
    assert(numPriorities(pq) == 0);
    assert(sizeByPriority(pq, 0) == 0);
    assert(sizeByPriority(pq, 1) == 0);
    assert(sizePQ(pq) == 0);

    enterPQ(pq, "one", 1);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 1);
    assert(sizeByPriority(pq, 1) == 1);
    assert(sizePQ(pq) == 1);

    leavePQ(pq);
    assert(isEmptyPQ(pq));
    assert(numPriorities(pq) == 0);
    assert(sizePQ(pq) == 0);

    enterPQ(pq, "one", 1);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 1);
    assert(sizeByPriority(pq, 1) == 1);
    assert(sizePQ(pq) == 1);

    enterPQ(pq, "two", 1);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 1);
    assert(sizeByPriority(pq, 1) == 2);
    assert(sizePQ(pq) == 2);

    leavePQ(pq);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 1);
    assert(sizeByPriority(pq, 1) == 1);
    assert(sizePQ(pq) == 1);

    enterPQ(pq, "three", 3);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 2);
    assert(sizeByPriority(pq, 3) == 1);
    assert(sizePQ(pq) == 2);

    enterPQ(pq, "four", 2);
    assert(!isEmptyPQ(pq));
    assert(numPriorities(pq) == 3);
    assert(sizeByPriority(pq, 2) == 1);
    assert(sizePQ(pq) == 3);

    assert(firstPQ(pq) == "two");
    assert(sizeByPriority(pq, 1) == 1);
    assert(sizeByPriority(pq, 2) == 1);
    assert(sizeByPriority(pq, 3) == 1);

    leavePQ(pq);
    leavePQ(pq);
    leavePQ(pq);

    assert(sizePQ(pq) == 0);
    assert(sizeByPriority(pq, 1) == 0);
    assert(numPriorities(pq) == 0);

    return 0;
}
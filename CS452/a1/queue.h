#ifndef _queue_h_
#define _queue_h_

typedef struct node {
  struct node* next;
} node;

typedef struct queue {
  node* head;
  node* tail;
} queue;

int is_empty(queue* q);

int single_element_or_empty(queue* q);

int enqueue(queue* q, node* n);

int dequeue(node** n, queue* q);

#endif /* queue.h */
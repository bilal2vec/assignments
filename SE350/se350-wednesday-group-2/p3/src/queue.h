#ifndef QUEUE_H_
#define QUEUE_H_
#include "common.h"

typedef struct node {
	struct node *next;
} node;

typedef struct queue {
	node *head;
	node *tail;
} queue;

// Add element to back of queue (tail)
extern int enqueue(queue *, node *);

// Remove element from front of queue (head)
extern int dequeue(node **result, queue *);

// Get a pointer to the front of the queue (head)
extern int try_dequeue(node **result, queue *);

// Get a pointer to first element in queue that satisfies a predicate
extern int get_elt_if(node **result, queue *, BOOL (*pred)(node *, void *), void *pred_param);

// Insert an element after the first element that satisfies a predicate
extern int insert_elt_if(queue *q, node *target, BOOL (*pred)(node *, void *), void *pred_param);

// Remove first element in queue that satisfies a predicate
extern int delete_elt_if(node **deleted, queue *, BOOL (*pred)(node *, void *), void *pred_param);

extern int serialize_queue_to_dest(char *dest, queue *q, int (*serialize_node_to_dest)(char* d, node *));

// Move element in "from" queue to "to" queue that satisfies a predicate
int move_elt_if(node **moved, queue* from, queue* to, BOOL (*pred)(node *, void *), void * pred_param);

// Check if queue is empty
extern int empty(queue *);

#endif

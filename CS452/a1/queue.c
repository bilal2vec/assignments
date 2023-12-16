#include "queue.h"

#include <stddef.h>

#include "rpi.h"

int is_empty(queue* q) { return q->head == NULL && q->tail == NULL; }

int single_element_or_empty(queue* q) { return q->head == q->tail; }

int enqueue(queue* q, node* n) {
  if (q == NULL) {
    printf("Error: queue is NULL\r\n");
    return -1;
  }

  n->next = NULL;

  if (is_empty(q)) {
    q->head = n;
    q->tail = n;
    return 0;
  }

  q->tail->next = n;
  q->tail = n;
  return 0;
}

int dequeue(node** n, queue* q) {
  if (q == NULL) {
    printf("Error: queue is NULL\r\n");

    *n = NULL;

    return -1;
  }

  if (is_empty(q)) {
    printf("Error: queue is empty\r\n");

    *n = NULL;

    return -1;
  }

  *n = q->head;

  if (single_element_or_empty(q)) {
    q->head = NULL;
    q->tail = NULL;
  } else if (q->head != NULL && q->head->next != NULL) {
    q->head = q->head->next;
  } else {
    printf("Error: queue is in an invalid state\r\n");
    return -1;
  }

  return 0;
}

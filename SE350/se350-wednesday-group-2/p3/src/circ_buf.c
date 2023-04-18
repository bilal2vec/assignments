#include "circ_buf.h"
#define CIRC_BUF_SIZE 32

int
suc(int p)
{
  if (p == CIRC_BUF_SIZE - 1) {
    return 0;
  }
  return p + 1;
}

int
prev(int p)
{
  if (p == 0) {
    return CIRC_BUF_SIZE - 1;
  }
  return p - 1;
}

void
cb_init(CIRC_BUF* c)
{
  c->start = 0;
  c->end = 0;
}

int
cb_push_back(CIRC_BUF* c, void* v)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return RTX_ERR;
  }
  if (c->cur_size == CIRC_BUF_SIZE) {
    return DEQUE_FULL;
  }
  c->data[c->end] = v;
  c->end = suc(c->end);
  c->cur_size++;
  return RTX_OK;
}

int
cb_push_front(CIRC_BUF* c, void* v)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return RTX_ERR;
  }
  if (c->cur_size == CIRC_BUF_SIZE) {
    return DEQUE_FULL;
  }
  c->start = prev(c->start);
  c->data[c->start] = v;
  c->cur_size++;
  return RTX_OK;
}

int
cb_size(CIRC_BUF* c)
{
  return c->cur_size;
}

int
cb_is_empty(CIRC_BUF* c)
{
  return !cb_size(c);
}

void*
cb_peek_back(CIRC_BUF* c)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return NULL;
  }
  if (c->cur_size == 0) {
    return NULL;
  }
  return c->data[prev(c->end)];
}
void*
cb_peek_front(CIRC_BUF* c)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return NULL;
  }
  if (c->cur_size == 0) {
    return NULL;
  }
  return c->data[c->start];
}

void*
cb_pop_back(CIRC_BUF* c)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return NULL;
  }
  if (c->cur_size == 0) {
    return NULL;
  }
  c->end = prev(c->end);
  c->cur_size--;
  return c->data[c->end];
}

void*
cb_pop_front(CIRC_BUF* c)
{
  if (c == NULL) {
    debug(("Fatal error: Circular buffer not initialized! This should never "
           "happen!"));
    return NULL;
  }
  if (c->cur_size == 0) {
    return NULL;
  }
  c->start = suc(c->start);
  c->cur_size--;
  return c->data[c->start];
}

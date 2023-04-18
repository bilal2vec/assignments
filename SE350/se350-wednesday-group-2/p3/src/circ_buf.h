#include "common.h"
#include <stdint.h>
#define CIRC_BUF_SIZE 32

// this basically acts like a c++ deque
// this means it has basically the same error checking as a c++ deque but is
// slightly nicer about letting you check yourself before you wreck yourself

#define DEQUE_FULL -2
#define DEQUE_EMPTY -3

typedef struct CIRC_BUF
{
  void* data[CIRC_BUF_SIZE];
  uint32_t start, end, cur_size;
} CIRC_BUF;

extern void
cb_init(CIRC_BUF* c);

extern int
cb_push_back(CIRC_BUF* c, void* v);

extern int
cb_push_front(CIRC_BUF* c, void* v);

extern int
cb_size(CIRC_BUF* c);

extern int
cb_is_empty(CIRC_BUF* c);

extern void*
cb_peek_back(CIRC_BUF* c);

extern void*
cb_peek_front(CIRC_BUF* c);

extern void*
cb_pop_front(CIRC_BUF* c);

extern void*
cb_pop_back(CIRC_BUF* c);

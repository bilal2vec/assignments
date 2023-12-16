#ifndef _process_h_
#define _process_h_

#include <stddef.h>

#include "priority.h"
#include "state.h"

typedef struct UserContext {
  size_t registers[31];
  size_t SP_EL0;
  size_t ELR_EL1;
  size_t SPSR_EL1;
} UserContext;

typedef struct Process {
  struct Process* next_process;
  size_t process_id;
  Priority priority;
  State state;
  UserContext context;
  struct Process* parent_process;
  struct Process*
      next_send_process;  // TODO(Bilal): Understand what this is for
} Process;

#endif /* process.h */
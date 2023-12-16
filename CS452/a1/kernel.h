#ifndef _kernel_h_
#define _kernel_h_

#include "priority.h"
#include "process.h"

int initialize_kernel();
int initialize_process(Priority priority, void (*function)());

int get_process_id();
int get_parent_process_id();

void yield();
void exit();

Process* scheduler();
Process* try_scheduler();
int switch_process(Process* next_process);

#endif /* kernel.h */
#include "kernel.h"

#include "HAL.h"
#include "priority.h"
#include "queue.h"
#include "rpi.h"
#include "user_processes.h"

// TODO(bilal): Update memory layout diagram

// NOTE: Global variables are not initialized by default, so we have to remember
// to do so explicitly

// Memory layout
// NOTE: Processes are zero-indexed even if they're not shown on the graph for
// brevity (e.g. PROCESS 0 is located at PROCESS_BASE, PROCESS 0 STACK is
// located at PROCESS_STACK_BASE)

// |----------------------------|
// | STACK_TOP                  | 0x1_000_0000
// |----------------------------|
// ...
// |----------------------------|
// | PROCESS 15 STACK           |
// |----------------------------|
// ...
// |----------------------------|
// | PROCESS 2 STACK            |
// |----------------------------|
// | PROCESS 1 STACK            | 0x200 bytes each
// |----------------------------|
// | PROCESS_STACK_BASE         |
// |----------------------------|
// | PROCESS 15                 |
// |----------------------------|
// ...
// |----------------------------|
// | PROCESS 2                  |
// |----------------------------|
// | PROCESS 1                  |
// |----------------------------|
// | PROCESS_BASE / PROCESS 0   |
// |----------------------------|
// | STACK_BASE / MAIN_PROCESS  | 0x0_0010_0000
// |----------------------------|

static char* const STACK_TOP = (char*)0x10000000;
static const size_t N_MAX_PROCESSES = 16;

// These two have the same address, 0x0_0010_0000 value arbitrarily chosen to be
// far enough above the code load location (0x0_0008_0000) so that we don't have
// conflicts
static char* const STACK_BASE = (char*)0x00100000;
static char* const MAIN_PROCESS = (char*)STACK_BASE;
static char* const PROCESS_BASE = (char*)MAIN_PROCESS + sizeof(Process);

// 512 bytes, TODO(bilal): Figure out what this should be actually set to.
static const uint32_t PROCESS_STACK_SIZE = 0x200;

// Start of processes stack
static char* const PROCESS_STACK_BASE =
    (char*)(PROCESS_BASE + N_MAX_PROCESSES * sizeof(Process));

// Pointer to current process
Process* current_process = NULL;

// Keep track of current process id, lets keep this unique
size_t current_process_id = 0;

// Pointer to head of uninitialized processes linkedlist
queue uninitialized_processes;

// Priority queues for ready and blocked processes
queue ready_queues[N_PRIORITIES];
// TODO: This should eventually be blocked queues for each typed of blocked
queue blocked_queues[N_PRIORITIES];

int initialize_kernel() {
  printf("Initializing kernel\r\n");

  // Initialize console UART
  uart_init();
  uart_config_and_enable(CONSOLE, 115200);

  // Register SVC handler
  register_SVC_handler();

  // Explicitly set global variables, not set by default
  current_process = NULL;
  current_process_id = 0;

  // Initialize uninitialized_processes queue
  uninitialized_processes.head = NULL;
  uninitialized_processes.tail = NULL;

  // Add all processes to uninitialized_processes queue. Note that we're not
  // initializing the processes yet, just adding them to the queue
  for (size_t i = 0; i < N_MAX_PROCESSES; i++) {
    Process* process = (Process*)(PROCESS_BASE + i * sizeof(Process));
    if (enqueue(&uninitialized_processes, (node*)process) != 0) {
      printf(
          "Error: failed to add process to uninitialized_processes queue\r\n");
      return -1;
    }
  }

  // Initialize ready_queues and blocked_queues to null.
  for (size_t i = 0; i < N_PRIORITIES; i++) {
    ready_queues[i].head = NULL;
    ready_queues[i].tail = NULL;
    blocked_queues[i].head = NULL;
    blocked_queues[i].tail = NULL;
  }

  printf("Initialized kernel\r\n");

  return 0;
}

int k_initialize_process(Priority priority, void (*function)()) {
  Process* process = NULL;

  // Try to get a process from the uninitialized_processes queue
  if (dequeue((node**)&process, &uninitialized_processes) != 0) {
    printf("Error: no more processes available\r\n");
    return -1;
  }

  // Initialize process
  process->process_id = current_process_id++;

  process->priority = priority;
  process->state = READY;

  for (size_t i = 0; i < 32; i++) {
    process->context.registers[i] = 0;
  }

  // Sets process's stack pointer to the top of its stack
  process->context.SP_EL0 =
      (size_t)(PROCESS_STACK_BASE + process->process_id * PROCESS_STACK_SIZE);

  // Sets process's program counter to the value of the function ptr passed in
  process->context.ELR_EL1 = (size_t)function;

  process->context.SPSR_EL1 = 0;

  // This will be null if there is no parent process
  process->parent_process = current_process;
  // TODO: Set this in future assignment
  process->next_send_process = NULL;

  // Add process to ready queue

  if (process->process_id == 0) {
    current_process = process;
    current_process->state = ACTIVE;

    return 0;
  }

  if (enqueue(&ready_queues[priority], (node*)process) != 0) {
    printf("Error: failed to add process to ready queue\r\n");
    return -1;
  }

  Process* next_process = scheduler();
  if (next_process == NULL) {
    printf("Error: failed to get next process from scheduler\r\n");
    return -1;
  }

  if (switch_process(next_process) != 0) {
    printf("Error: failed to switch to next process\r\n");
    return -1;
  }

  printf("Initialized process with PID: %d\r\n", process->process_id);
  return process->process_id;
}

int initialize_process(Priority priority, void (*function)()) {
  char a = 'a';

  printf("before %d\r\n", &a);

  SVC(k_initialize_process, priority, function);

  printf("after %d\r\n", &a);

  size_t result;
  asm volatile("MOV %0, X0" : "=r"(result));
  printf("end of initialize_process\r\n");
  printf("result: %d\r\n", result);
  return result;
}

int get_process_id() {
  if (current_process == NULL) {
    printf("Error: current_process is null\r\n");
    return -1;
  }

  return current_process->process_id;
}

int get_parent_process_id() {
  if (current_process == NULL) {
    printf("Error: current_process is null\r\n");
    return -1;
  }

  if (current_process->parent_process == NULL) {
    printf("Error: current_process->parent_process is null\r\n");
    return -1;
  }

  return current_process->parent_process->process_id;
}

void k_yield() {
  Process* next_process = scheduler();
  switch_process(next_process);
}

void yield() { SVC(k_yield); }

void k_exit() {
  enqueue(&uninitialized_processes, (node*)current_process);
  current_process = NULL;
  Process* next_process = scheduler();
  switch_process(next_process);
}

void exit() {
  size_t tmp;
  // asm volatile("MOV %0, X30" ::"=r"(tmp));
  SVC(k_exit);
  // asm volatile("MOV X30, %0" : "r"(tmp));
}

Process* scheduler() {
  Process* next_process = NULL;
  Priority curr_priority = LOW;

  if (current_process != NULL) {
    curr_priority = current_process->priority;
  }

  for (int i = HIGH; i >= curr_priority; i--) {
    if (!is_empty(&ready_queues[i])) {
      // Get next process from ready queue
      if (dequeue((node**)&next_process, &ready_queues[i]) != 0) {
        printf("Error: failed to dequeue process from ready queue\r\n");
        break;
      }
      return next_process;
    }
  }

  return current_process;
}

// TODO: Logic for this should definitely be looked over closely once we add
// blocking
int switch_process(Process* next_process) {
  if (next_process == NULL) {
    return;
  }
  if (next_process != current_process && current_process != NULL) {
    current_process->state = READY;
    if (enqueue(&ready_queues[current_process->priority],
                (node*)current_process) != 0) {
      printf("Error: failed to add process to ready queue\r\n");
      current_process->state = ACTIVE;
      return -1;
    }
  }

  current_process = next_process;
  current_process->state = ACTIVE;

  // // Gracefully handle edge case if next_process is the same as
  // current_process,
  // // should *almost* never happen
  // if (next_process == current_process) {
  //   return 0;
  // }

  // if (next_process->state != READY) {
  //   printf("Error: next_process is not ready\r\n");
  //   return -1;
  // }

  // // NOTE: This assumes that any syscall that can block the process will set
  // the
  // // state to BLOCKED itself
  // if (current_process->state == ACTIVE) {
  //   current_process->state = READY;
  // }

  // current_process = next_process;
  // current_process->state = ACTIVE;

  return 0;
}
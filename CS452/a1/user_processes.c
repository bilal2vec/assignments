#include "kernel.h"
#include "priority.h"
#include "rpi.h"

void child_process_fn() {
  printf("Process ID: %d Parent Process ID: %d\r\n", get_process_id(),
         get_parent_process_id());
  yield();
  printf("Process ID: %d Parent Process ID: %d\r\n", get_process_id(),
         get_parent_process_id());
  exit();
}

void root_process_fn() {
  int child_process_1_pid = initialize_process(LOW, &child_process_fn);
  assert(child_process_1_pid, "Failed to initialize child process 1\r\n");

  printf("Created process with PID: %d\r\n", child_process_1_pid);

  int child_process_2_pid = initialize_process(LOW, &child_process_fn);
  assert(child_process_2_pid, "Failed to initialize child process 2\r\n");

  printf("Created process with PID: %d\r\n", child_process_2_pid);

  // int child_process_3_pid = initialize_process(LOW, &child_process_fn);
  // assert(child_process_3_pid, "Failed to initialize child process 3\r\n");

  // printf("Created process with PID: %d\r\n", child_process_3_pid);

  // int child_process_4_pid = initialize_process(LOW, &child_process_fn);
  // assert(child_process_4_pid, "Failed to initialize child process 4\r\n");

  // printf("Created process with PID: %d\r\n", child_process_4_pid);

  printf("Exiting root process\r\n");

  // exit();
}
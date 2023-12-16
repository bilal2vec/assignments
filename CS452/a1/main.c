#include <stdint.h>

#include "HAL.h"
#include "kernel.h"
#include "priority.h"
#include "rpi.h"
#include "user_processes.h"

extern int k_initialize_process(Priority priority, void (*function)());

int kmain() {
  if (initialize_kernel() != 0) {
    printf("Error: failed to initialize kernel\r\n");
    return -1;
  }

  k_initialize_process(MEDIUM, NULL);

  int child_process_1_pid = initialize_process(HIGH, &child_process_fn);
  assert(child_process_1_pid, "Failed to initialize child process 1\r\n");

  printf("Created process with PID: %d\r\n", child_process_1_pid);

  while (1) {
    continue;
  }

  return 0;
}

// TODO

// How to handle kernel calls
// The right way to do this is exactly like how SE 350 implements it
// SE 350 has a default SVC_HANDLER that's already registered as an SVC handler
// for you We need to register our own SVC handler for this course.
// https://student.cs.uwaterloo.ca/~cs452/F23/lectures/lec04.html has the
// details > sets PC to address determined by VBAREL1 and type of exception
// (sync, IRQ, FIQ, SError) > VBAREL1 points to an exception vector for
// exceptions taken to EL1 Rolling our own SVC handling by by setting the pc to
// the address of a label is not the right way to do this and also not how they
// expect you to do this.

// Add a null process at its own priority level
// Figure out what we have to do with exception vectors
// Store proper user context in process struct

// initialize_kernel()
//  Initialize marklin

// initialize_process()
//  Initialize Process stack
//  Run scheduler / switch_process as needed

// scheduler()

// switch_process()
//  Switch to next process

// yield()
//  Call svc to switch back to the kernel
//  Add current process to priority queue
//  Call scheduler to get the next process to run
//  Call switch_process to switch to the next process

// exit()
//  Call svc to switch back to the kernel
//  Remove process from queues
//  Call scheduler to get the next process to run
//  Call switch_process to switch to the next process

// Questions

// What is next_send_process for in process.h?

// If new process created should it be switched to immediately?
// Should the process be added to front/end of ready queue?
// My intuition is that it should be switched to immediately and added to the
// end of the ready queue, this would be similar to how we did it in SE 350 This
// would also mean that the order of prints that we're supposed to follow in the
// kernel 1 spec would run in a way to maximally show that our kernel is working
// as expected.
// TODO: fix this is in the wrong order
// It should print out:
// Process ID: 1 Parent Process ID: 0 (first low priority child process)
// Created process with PID: 1 (root process)
// Process ID: 2 Parent Process ID: 0 (second low priority child process)
// Created process with PID: 2 (root process)
// Process ID: 3 Parent Process ID: 0 (first high priority child process)
// Created process with PID: 3 (root process)
// Process ID: 4 Parent Process ID: 0 (second high priority child process)
// Created process with PID: 4 (root process)
// Process ID: 3 Parent Process ID: 0 (first high priority child process)
// Process ID: 4 Parent Process ID: 0 (second high priority child process)
// Process ID: 1 Parent Process ID: 0 (first low priority child process)
// Process ID: 2 Parent Process ID: 0 (second low priority child process)

// https://student.cs.uwaterloo.ca/~cs452/F23/assignments/k1.html
// > On return of each Create(), busy-wait IO should be used to print "Created:
// <task id>" on the terminal screen. how is this different from

// IIUC, the new bootloader reboots the rpi every time the kmain function
// returns, so this might make debugging and visually seeing the output a bit
// more difficult as the rpi reboot messages will start taking over the console
// output. Is this the intended behaviour? If so, is there an acceptable way to
// disable this?
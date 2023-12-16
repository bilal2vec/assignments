#ifndef _HAL_h_
#define _HAL_h_
#include <stddef.h>

void register_SVC_handler();

// First argument is the pointer to the syscall function to call, the next max
// 5 arguments are the arguments to pass to the syscall function
void SVC(void (*function)(), ...);

#endif /* HAL.h */
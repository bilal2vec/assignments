#include "HAL.h"

#include "kernel.h"
#include "rpi.h"
#include "user_processes.h"

extern Process* current_process;
extern Process* main_process;

void SVC_handler() {
  size_t arg_1, arg_2, arg_3, arg_4;
  asm volatile("MOV %0, X0" : "=r"(arg_1));
  asm volatile("MOV %0, X1" : "=r"(arg_2));
  asm volatile("MOV %0, X2" : "=r"(arg_3));
  asm volatile("MOV %0, X3" : "=r"(arg_4));

  asm volatile("MOV %0, X0" : "=r"(current_process->context.registers[0]));
  asm volatile("MOV %0, X1" : "=r"(current_process->context.registers[1]));
  asm volatile("MOV %0, X2" : "=r"(current_process->context.registers[2]));
  asm volatile("MOV %0, X3" : "=r"(current_process->context.registers[3]));
  asm volatile("MOV %0, X4" : "=r"(current_process->context.registers[4]));
  asm volatile("MOV %0, X5" : "=r"(current_process->context.registers[5]));
  asm volatile("MOV %0, X6" : "=r"(current_process->context.registers[6]));
  asm volatile("MOV %0, X7" : "=r"(current_process->context.registers[7]));
  asm volatile("MOV %0, X8" : "=r"(current_process->context.registers[8]));
  asm volatile("MOV %0, X9" : "=r"(current_process->context.registers[9]));
  asm volatile("MOV %0, X10" : "=r"(current_process->context.registers[10]));
  asm volatile("MOV %0, X11" : "=r"(current_process->context.registers[11]));
  asm volatile("MOV %0, X12" : "=r"(current_process->context.registers[12]));
  asm volatile("MOV %0, X13" : "=r"(current_process->context.registers[13]));
  asm volatile("MOV %0, X14" : "=r"(current_process->context.registers[14]));
  asm volatile("MOV %0, X15" : "=r"(current_process->context.registers[15]));
  asm volatile("MOV %0, X16" : "=r"(current_process->context.registers[16]));
  asm volatile("MOV %0, X17" : "=r"(current_process->context.registers[17]));
  asm volatile("MOV %0, X18" : "=r"(current_process->context.registers[18]));
  asm volatile("MOV %0, X19" : "=r"(current_process->context.registers[19]));
  asm volatile("MOV %0, X20" : "=r"(current_process->context.registers[20]));
  asm volatile("MOV %0, X21" : "=r"(current_process->context.registers[21]));
  asm volatile("MOV %0, X22" : "=r"(current_process->context.registers[22]));
  asm volatile("MOV %0, X23" : "=r"(current_process->context.registers[23]));
  asm volatile("MOV %0, X24" : "=r"(current_process->context.registers[24]));
  asm volatile("MOV %0, X25" : "=r"(current_process->context.registers[25]));
  asm volatile("MOV %0, X26" : "=r"(current_process->context.registers[26]));
  asm volatile("MOV %0, X27" : "=r"(current_process->context.registers[27]));
  asm volatile("MOV %0, X28" : "=r"(current_process->context.registers[28]));
  asm volatile("MOV %0, X29" : "=r"(current_process->context.registers[29]));
  asm volatile("MOV %0, X30" : "=r"(current_process->context.registers[30]));
  asm volatile("MRS %0, SP_EL0" : "=r"(current_process->context.SP_EL0));
  asm volatile("MRS %0, ELR_EL1" : "=r"(current_process->context.ELR_EL1));
  asm volatile("MRS %0, SPSR_EL1" : "=r"(current_process->context.SPSR_EL1));

  asm volatile("MOV X3, %0" ::"r"(arg_4));
  asm volatile("MOV X2, %0" ::"r"(arg_3));
  asm volatile("MOV X1, %0" ::"r"(arg_2));
  asm volatile("MOV X0, %0" ::"r"(arg_1));

  asm volatile("BLR X6;");

  size_t result;
  asm volatile("MOV %0, X0" : "=r"(result));

  asm volatile("MSR SPSR_EL1, %0" ::"r"(current_process->context.SPSR_EL1));
  asm volatile("MSR ELR_EL1, %0" ::"r"(current_process->context.ELR_EL1));
  asm volatile("MSR SP_EL0, %0" ::"r"(current_process->context.SP_EL0));
  asm volatile("MOV X30, %0" ::"r"(current_process->context.registers[30]));
  asm volatile("MOV X29, %0" ::"r"(current_process->context.registers[29]));
  asm volatile("MOV X28, %0" ::"r"(current_process->context.registers[28]));
  asm volatile("MOV X27, %0" ::"r"(current_process->context.registers[27]));
  asm volatile("MOV X26, %0" ::"r"(current_process->context.registers[26]));
  asm volatile("MOV X25, %0" ::"r"(current_process->context.registers[25]));
  asm volatile("MOV X24, %0" ::"r"(current_process->context.registers[24]));
  asm volatile("MOV X23, %0" ::"r"(current_process->context.registers[23]));
  asm volatile("MOV X22, %0" ::"r"(current_process->context.registers[22]));
  asm volatile("MOV X21, %0" ::"r"(current_process->context.registers[21]));
  asm volatile("MOV X20, %0" ::"r"(current_process->context.registers[20]));
  asm volatile("MOV X19, %0" ::"r"(current_process->context.registers[19]));
  asm volatile("MOV X18, %0" ::"r"(current_process->context.registers[18]));
  asm volatile("MOV X17, %0" ::"r"(current_process->context.registers[17]));
  asm volatile("MOV X16, %0" ::"r"(current_process->context.registers[16]));
  asm volatile("MOV X15, %0" ::"r"(current_process->context.registers[15]));
  asm volatile("MOV X14, %0" ::"r"(current_process->context.registers[14]));
  asm volatile("MOV X13, %0" ::"r"(current_process->context.registers[13]));
  asm volatile("MOV X12, %0" ::"r"(current_process->context.registers[12]));
  asm volatile("MOV X11, %0" ::"r"(current_process->context.registers[11]));
  asm volatile("MOV X10, %0" ::"r"(current_process->context.registers[10]));
  asm volatile("MOV X9, %0" ::"r"(current_process->context.registers[9]));
  asm volatile("MOV X8, %0" ::"r"(current_process->context.registers[8]));
  asm volatile("MOV X7, %0" ::"r"(current_process->context.registers[7]));
  asm volatile("MOV X6, %0" ::"r"(current_process->context.registers[6]));
  asm volatile("MOV X5, %0" ::"r"(current_process->context.registers[5]));
  asm volatile("MOV X4, %0" ::"r"(current_process->context.registers[4]));
  asm volatile("MOV X3, %0" ::"r"(current_process->context.registers[3]));
  asm volatile("MOV X2, %0" ::"r"(current_process->context.registers[2]));
  asm volatile("MOV X1, %0" ::"r"(current_process->context.registers[1]));
  asm volatile("MOV X0, %0" ::"r"(current_process->context.registers[0]));

  asm volatile("MOV X0, %0" ::"r"(result));

  asm volatile("ERET;");
}

void register_SVC_handler() {
  asm volatile(
      " B skip;"  // Skip over the exception handler code
      "handler:"
      ".balign 0x400;"   // The start of the exception vector table for
                         // synchronous exceptions from lower exception levels
                         // using AArch64 is at offset 0x400
      " B SVC_handler;"  // Just a hard-coded branch to the exception handler
      "skip:"
      " LDR X0, =handler;"  // Load the address of the exception handler into X0
      " MSR VBAR_EL1, X0;"  // Set the exception table for exceptions taken to
                            // EL1
  );
}

void SVC(void (*function)(), ...) {
  asm volatile(
      "MOV X6, X0;"  // The first argument is the pointer to the syscall
                     // function to call, save to R6
      "MOV X0, X1;"  // The next max 4 arguments are the arguments to pass to
                     // the syscall function, save to R0-R4
      "MOV X1, X2;"
      "MOV X2, X3;"
      "MOV X3, X4;"
      "MOV X4, X5;"
      "SVC #0;");
}

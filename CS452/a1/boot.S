// https://www.rpi4os.com/part1-bootstrapping/

// ***************************************
// SCTLR_EL1, System Control Register (EL1)
// Architecture Reference Manual Section D17.2.118
// ***************************************
#define SCTLR_RESERVED (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define USER_MASK_ACCESS (1 << 9)
#define SCTLR_WFE_WFI_ENABLED (1 << 18) | (1 << 16)
#define SCTLR_VALUE_MMU_DISABLED (SCTLR_RESERVED | USER_MASK_ACCESS | SCTLR_WFE_WFI_ENABLED)

// ***************************************
// HCR_EL2, Hypervisor Configuration Register (EL2)
// Architecture Reference Manual Section D17.2.48
// ***************************************
#define HCR_RW (1 << 31)

// ***************************************
// SPSR_EL2, Saved Program Status Register (EL2)
// Architecture Reference Manual Section C5.2.19
// ***************************************
#define SPSR_MASK_ALL (7 << 6)
#define SPSR_EL1      (5 << 0)
#define SPSR_VALUE (SPSR_MASK_ALL | SPSR_EL1)

// ensure the linker puts this at the start of the kernel image
.section ".text.boot"
.global _start
_start:
    // check processor ID is zero (executing on main core), else loop
//    mrs  x0, mpidr_el1
//    and  x0, x0, #3
//    cbnz x0, exit

    // are we already in EL1?
    mrs  x1, CurrentEL
    and  x1, x1, #8
    cbz  x1, el1_entry

    // otherwise, switch to EL1 by fake exception to return from
    ldr x2, =HCR_RW
    msr hcr_el2, x2

    ldr x3, =SPSR_VALUE
    msr spsr_el2, x3

    adr x4, el1_entry
    msr elr_el2, x4

    eret // -> el1_entry

el1_entry:
    // configure processor and mmu
    ldr x2, =SCTLR_VALUE_MMU_DISABLED
    msr sctlr_el1, x2

    // mask-out exceptions at EL1
    msr DAIFSet, #0b1111
    // initialize SP
    msr SPSel, #1
    ldr     x0, =stackend
    mov     sp, x0
    // Jump to our main() routine in C/C++
    bl      kmain

    // use watchdog to reboot
    mov x2, 36
    mov x0, 28
    movk  x2, 0xfe10, lsl 16
    movk  x0, 0xfe10, lsl 16
    mov w3, 1
    mov w1, 32
    movk  w3, 0x5a00, lsl 16
    str w3, [x2]
    movk  w1, 0x5a00, lsl 16
    str w1, [x0]

exit:
    wfi
    b    exit

.section ".bss"
.balign 16
stack:
    .rept 0x10000
    .byte 0
    .endr
stackend:
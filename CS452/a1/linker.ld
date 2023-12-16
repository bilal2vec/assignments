ENTRY(_start)
SECTIONS {
  . = 0x80000;           /* start text at this location */
  .text.boot : {
    KEEP(*(.text.boot))  /* boot code must come first */
  }
}
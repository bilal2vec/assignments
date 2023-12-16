#ifndef _rpi_h_
#define _rpi_h_ 1

#include <stddef.h>
#include <stdint.h>

#define CONSOLE 1
#define MARKLIN 2
#define printf(fmt, ...) uart_printf(CONSOLE, fmt, ##__VA_ARGS__);
#define assert(stmt, fmt, ...)                            \
  if (stmt < 0) {                                         \
    printf("Assertion failed: return code %d\r\n", stmt); \
    printf(fmt, ##__VA_ARGS__);                           \
  }

void uart_putc(size_t line, unsigned char c);
unsigned char uart_getc(size_t line);
void uart_putl(size_t line, const char *buf, size_t blen);
void uart_puts(size_t line, const char *buf);
void uart_printf(size_t line, char *fmt, ...);
void uart_config_and_enable(size_t line, uint32_t baudrate);
void uart_init();

#endif /* rpi.h */
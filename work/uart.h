#ifndef UART_H
#define UART_H

#include <stddef.h>
#include <stdint.h>

void uart_init(void);
void uart_putchar(uint8_t c);
uint8_t uart_getchar();
void uart_puts(const uint8_t *s);
void uart_gets(uint8_t *s, size_t size);

#endif
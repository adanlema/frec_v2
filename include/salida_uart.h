#ifndef SALIDA_UART_H
#define SALIDA_UART_H

#include <stdbool.h>
#include <stdint.h>

void usart_config(void);
void usart_sendstring(unsigned char *palabra);
uint8_t usart_getchar(void);

#endif
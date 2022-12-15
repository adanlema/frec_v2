#ifndef SALIDA_UART_H
#define SALIDA_UART_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Opt_uint8_t{
    bool valido;
    uint8_t valor;
}Opt_uint8_t;

void usart_config(void);
void usart_sendstring(const char *palabra);
uint8_t usart_getchar(void);
Opt_uint8_t usart_opt_getchar(void);

#endif
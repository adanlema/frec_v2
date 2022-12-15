#ifndef SALIDA_UART3_H
#define SALIDA_UART3_H

#include <stdbool.h>
#include <stdint.h>
/*
typedef struct Opt_uint8_t{
    bool valido;
    uint8_t valor;
}Opt_uint8_t;

void usart_config(void);
void usart_sendstring(const char *palabra);
uint8_t usart_getchar(void);
Opt_uint8_t usart_opt_getchar(void);
*/
void usart3_config(void);
void usart3_sendchar(char data);
void usart3_sendstring(const char *palabra);
uint8_t usart3_getchar(void);

#endif
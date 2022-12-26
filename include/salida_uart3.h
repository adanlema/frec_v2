#ifndef SALIDA_UART3_H
#define SALIDA_UART3_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Opt_uint8_t{
    bool valido;
    uint8_t valor;
}Opt_uint8_t;


typedef struct Interprete {
    enum {E_INICIO,E_F,E_T} estado;
    int canal;
    bool transmitido;
}Interprete;

uint16_t USART_BRR_Val(uint32_t Baud_Rate, uint32_t F_CK);
void usart3_config(void);
void usart3_sendchar(char data);
void usart3_sendstring(const char *palabra);
uint8_t usart3_getchar(void);
Opt_uint8_t usart3_opt_getchar(void);
void Interprete_paso(Interprete *self);
void Interprete_init(Interprete *self);

#endif
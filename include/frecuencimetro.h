#ifndef TIMER_CONFIG_H
#define TIMER_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Lectura{
    bool valida;
    uint32_t valor;
    bool valida2;
    uint32_t valor2;
}Lectura;


void frecuencimetro_init(void);
Lectura frecuencimetro_get_frecuencia(void);

#endif
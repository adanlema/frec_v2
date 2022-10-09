#ifndef I2C_H
#define I2C_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f1xx.h"
#include <main.h>

void I2C__init(void);
void I2C__start(void);
void I2C__write(uint8_t data);
void I2C__direccion(uint8_t direccion);
void I2C__stop(void);
void I2C__writemulti(uint8_t *data, uint8_t size);
void I2C__read(uint8_t direccion, uint8_t *buffer, uint8_t size);

#endif
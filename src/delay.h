#ifndef DELAY_H
#define DELAY_H

#include <stdbool.h>
#include <stdint.h>

void tim6_init();
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif
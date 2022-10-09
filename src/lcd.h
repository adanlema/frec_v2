#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

void lcd_init(void);
void lcd_escribir(unsigned char *Data, unsigned char fila, unsigned char col);

#endif
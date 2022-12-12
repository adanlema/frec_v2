#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

void lcd_escribir(unsigned char *Data, unsigned char fila, unsigned char col);
void lcd_inicio(void);
void lcd_init(void);
#endif
#ifndef LCD_H
#define LCD_H

#include <stdbool.h>
#include <stdint.h>

void lcd_clear(void);
void lcd_escribir(const char *Data, unsigned char fila, unsigned char col);
void lcd_inicio(void);
#endif
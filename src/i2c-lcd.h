#ifndef I2C_LCD_H
#define I2C_LCD_H


#include <main.h>
#include <i2c.h>

void lcd_write (uint8_t direccion, uint8_t *datos, int size);
void lcd_send_cmd (char cmd);
void lcd_send_data (char data);
void lcd_clear (void);
void lcd_colocar_cursor(int fila, int columna);
void lcd_init (void);
void lcd_send_string (char *str);


#endif
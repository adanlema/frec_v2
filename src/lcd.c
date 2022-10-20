#include<stdio.h>
#include<stdbool.h>
#include <stm32f1xx.h>
#include "delay.h"
#define INLINE inline __attribute__((always_inline))




INLINE void inicializa__PuertoGPIOB(void){
    RCC->APB2ENR |= (1<<3);}
INLINE void conf_PinDatosRsEn(void){
    GPIOA->CRL = 0x22222222;
    GPIOB->CRH = (GPIOB->CRH & ~(0xFF << 8)) | (0x66 << 8);}
INLINE void comando_RS(bool rs){
    if(rs) GPIOB->BSRR |= (1<<11);
    else   GPIOB->BSRR |= (1<<27);}
INLINE void comando_EN(void){
    GPIOB->BSRR |= (1<<10);
    delay_ms(1);
    GPIOB->BSRR |= (1<<26);}
INLINE void bits_NULL(void){
    GPIOA->BSRR |= (0xFF<<16);}



static void lcd_conf(void){
    inicializa__PuertoGPIOB();
    conf_PinDatosRsEn();}
static void lcd_escribir_byte(unsigned char Data){
    comando_RS(true);
    bits_NULL();
    GPIOA->BSRR |= Data;
    comando_EN();}
static void lcd_comand_escribir(unsigned char Data){
    comando_RS(false);
    bits_NULL();
    GPIOA->BSRR |= Data;
    comando_EN();}




INLINE void reiniciar_lcd(void){
    lcd_comand_escribir(0x30);
    delay_ms(10);
    comando_EN();
    delay_ms(1);
    comando_EN();
    delay_ms(1);}
INLINE void comando_set(void){
    lcd_comand_escribir(0x38);}
INLINE void comando_show(void){
    lcd_comand_escribir(0x08);}
INLINE void comando_clear(void){
    lcd_comand_escribir(0x01);
    delay_ms(3);}
INLINE void comando_confModoEntrada(void){
    lcd_comand_escribir(0x07);}
INLINE void comando_encenderlcd(void){
    lcd_comand_escribir(0x0d);}






void lcd_init(void){
    lcd_conf();
    delay_ms(100);
    reiniciar_lcd();
    comando_set();
    comando_show();
    comando_clear();
    comando_confModoEntrada();
    comando_encenderlcd();}


void lcd_escribir(unsigned char *Data, unsigned char fila, unsigned char col){
    unsigned char cursor = 0x00;
    if(fila)    cursor = 0xc0;
    else        cursor = 0x80;
    cursor += col;
    lcd_comand_escribir(cursor);
    while(*Data != '\0'){
        lcd_escribir_byte(*Data);
        Data++;}
}











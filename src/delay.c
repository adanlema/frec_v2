#include<stdio.h>
#include<stdbool.h>
#include <stm32f1xx.h>
#include "delay.h"
#define INLINE inline __attribute__((always_inline))


INLINE void tim2_enable(void){
    RCC->APB1ENR |= (1<<0);}
INLINE void tim2_delay1us(void){
    TIM2->PSC = 8-1;
    TIM2->ARR = 0xffff;    }
INLINE void tim2_enableCounter(void){
    TIM2->CR1 |= (1<<0);}

static void tim2_conf(void){
    tim2_enable();
    tim2_delay1us();
    tim2_enableCounter();
    while(!(TIM2->SR & (1<<0)));}




void tim2_init(){
    tim2_conf();}



void delay_us(uint16_t us){
    TIM2->CNT = 0;
    while (TIM2->CNT < us);}

void delay_ms(uint16_t ms){
    for (uint16_t i=0;i<ms;i++){
        delay_us(1000);
        }
}
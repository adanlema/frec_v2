#include<stdio.h>
#include<stdbool.h>
#include <stm32f1xx.h>
#include "stm32f10x.h"
#include "delay.h"
#define INLINE inline __attribute__((always_inline))


INLINE void tim6_enable(void){
    RCC->APB1ENR |= (1<<4);}
INLINE void tim6_delay1us(void){
    TIM6->PSC = 8-1;
    TIM6->ARR = 0xffff;    }
INLINE void tim6_enableCounter(void){
    TIM6->CR1 |= (1<<0);}

static void tim6_conf(void){
    tim6_enable();
    tim6_delay1us();
    tim6_enableCounter();
    while(!(TIM6->SR & (1<<0)));}




void tim6_init(){
    tim6_conf();}



void delay_us(uint16_t us){
    TIM6->CNT = 0;
    while (TIM6->CNT < us);}

void delay_ms(uint16_t ms){
    for (uint16_t i=0;i<ms;i++){
        delay_us(1000);
        }
}
#include<stdio.h>
#include<stdbool.h>
#include <stm32f1xx.h>
#include "delay.h"
#define INLINE inline __attribute__((always_inline))

void tim2_init(){
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 8-1;
    TIM2->ARR = 0xffff;
    TIM2->CR1 |= TIM_CR1_CEN;
    while(!(TIM2->SR & (1<<0)));
    }

void delay_us(uint16_t us){
    TIM2->CNT = 0;
    while (TIM2->CNT < us);}

void delay_ms(uint16_t ms){
    for (uint16_t i=0;i<ms;i++){
        delay_us(1000);
        }
}
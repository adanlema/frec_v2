#include <stm32f1xx.h>
#include<main.h>

void Reloj_init(void)
{
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)) continue;
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_HSE;
    while ((RCC->CFGR & RCC_CFGR_SWS_Msk) == RCC_CFGR_SWS_HSE )continue;
    SystemCoreClockUpdate();
}

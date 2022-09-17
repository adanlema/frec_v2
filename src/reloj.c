#include <stm32f1xx.h>
#include<main.h> 

#define INLINE inline __attribute__((always_inline))

enum {  CR_HSE_ON           = (1UL<<16),
        CR_HSE_READY        = (1UL<<17),
        CFGR_SW_HSE         = (1UL<<0),
        CFGR_SW_MASK        = (3UL<<0),
        CFGR_SW_ACTIVO_MASK = (3UL<<2),
        CFGR_SW_ACTIVO_HSE  = (1UL<<2)};

INLINE void esperaHseListo(void){
    while (!(RCC->CR & CR_HSE_READY)) continue;}
INLINE void activaHse(void){
    RCC->CR |= CR_HSE_ON;}
INLINE void seleccionaHse(void){
    RCC->CFGR = (RCC->CFGR & ~CFGR_SW_MASK) | CFGR_SW_HSE;}
INLINE void esperaImpacteSeleccionHse(void){
    while ((RCC->CFGR & CFGR_SW_ACTIVO_MASK) == CFGR_SW_ACTIVO_HSE)continue;}
INLINE void actualizaVariable_SystemCoreClock(void){
    SystemCoreClockUpdate();}

void Reloj_init(void)
{
    activaHse();
    esperaHseListo();
    seleccionaHse();
    esperaImpacteSeleccionHse();
    actualizaVariable_SystemCoreClock();
}

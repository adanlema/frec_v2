#include <reloj.h>
#define INLINE inline __attribute__((always_inline))

enum {  CR_HSE_ON           = (1UL<<16),
        CR_HSE_READY        = (1UL<<17),
        CFGR_SW_HSE         = (1UL<<0),
        CFGR_SW_MASK        = (3UL<<0),
        CFGR_SW_ACTIVO_MASK = (3UL<<2),
        CFGR_SW_ACTIVO_HSE  = (1UL<<2)};

INLINE void activaHse(void){
    RCC->CR |= CR_HSE_ON;}
INLINE void esperaHseListo(void){
    while (!(RCC->CR & CR_HSE_READY)) continue;}
INLINE void seleccionaHse(void){
    RCC->CFGR = (RCC->CFGR & ~CFGR_SW_MASK) | CFGR_SW_HSE;}
INLINE void esperaImpacteSeleccionHse(void){
    while (!((RCC->CFGR & CFGR_SW_ACTIVO_MASK) == CFGR_SW_ACTIVO_HSE))continue;}
INLINE void confHPRE(void){
    RCC->CFGR &= ~(1<<7);}
INLINE void confPPRE1(void){
    RCC->CFGR &= ~(1<<10);}
INLINE void confPPRE2(void){
    RCC->CFGR &= ~(1<<13);}
INLINE void actualizaVariable_SystemCoreClock(void){
    SystemCoreClockUpdate();}

void reloj_init(void)
{
    activaHse();
    esperaHseListo();
    seleccionaHse();
    esperaImpacteSeleccionHse();
    confHPRE();
    confPPRE1();
    confPPRE2();
    actualizaVariable_SystemCoreClock();
}

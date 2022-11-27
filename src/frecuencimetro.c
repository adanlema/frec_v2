#include "frecuencimetro.h"
#include <stm32f1xx.h>

#define INLINE inline __attribute__((always_inline))

INLINE void inicializa__Puerto(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH = (GPIOA->CRH & ~(GPIO_CRH_CNF8_Msk | GPIO_CRH_MODE8_Msk)) | (GPIO_CRH_CNF8_0);
    GPIOA->CRH = (GPIOA->CRH & ~(0b1111 << 4)) | (0b0100 << 4);}

INLINE void limpiar_banderaAct(void){
    TIM1->SR &= ~TIM_SR_UIF;}


INLINE void habilitarTIM1(void){
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;}
INLINE void confDivisorUno(void){
    TIM1->CR1 &= ~TIM_CR1_CKD_Msk;}
INLINE void habilitarbufferARR(void){
    TIM1->CR1 |= TIM_CR1_ARPE;}
INLINE void confModoCounterUp(void){
    TIM1->CR1 &= ~TIM_CR1_CMS_Msk;
    TIM1->CR1 &= ~TIM_CR1_DIR;}
INLINE void habilitarContador(void){
    TIM1->CR1 |= TIM_CR1_CEN;}
INLINE void conexionCHANNELTIM1(void){
    TIM1->CR2 &= ~TIM_CR2_TI1S;}
INLINE void confParametros(void){
    TIM1->CNT = 0;
    TIM1->PSC = 20-1;
    TIM1->ARR = 0xffff;
    TIM1->RCR = 0;}
INLINE void conf_Registros(void){
    habilitarTIM1();
    confDivisorUno();
    habilitarbufferARR();
    confModoCounterUp();
    habilitarContador();
    conexionCHANNELTIM1();
    confParametros();}


INLINE void confTIM1comoIC(void){
    TIM1->CCMR1 = (TIM1->CCMR1 & ~TIM_CCMR1_CC1S_Msk) | TIM_CCMR1_CC1S_0;
    TIM1->CCMR1 = (TIM1->CCMR1 & ~(0b11<<8)) | (1<<8);}
INLINE void confFlancoAsc(void){
    TIM1->CCER &= ~TIM_CCER_CC1P;
    TIM1->CCER &= ~(1<<5);}
INLINE void habilitarICTIM1(void){
    TIM1->CCER |= TIM_CCER_CC1E;
    TIM1->CCER |=(1<<4);}    
INLINE void conf_IC(void){
    confTIM1comoIC();
    confFlancoAsc();
    habilitarICTIM1();}

INLINE void habilitarInterrupcionIC(void){
    TIM1->DIER |= TIM_DIER_CC1IE;
    TIM1->DIER |= (1<<2);}


INLINE void inicializa__Timer(void)
{
    limpiar_banderaAct();
    conf_Registros();
    conf_IC();
    habilitarInterrupcionIC();
    while(!(TIM1->SR & TIM_SR_UIF)) continue; // Espera actualización
    limpiar_banderaAct();}

void frecuencimetro_init(void)
{
    inicializa__Puerto();
    inicializa__Timer();
}


static volatile struct EstadoFrecuencimetro{
    int32_t anterior1;
    int32_t anterior2;
    bool anterior_valido1;
    bool anterior_valido2;
    bool lectura_valida1;
    bool lectura_valida2;
    int32_t diferencia;
    int32_t frecuencia1;
    int32_t frecuencia2;
} estado = {0,0,false,false,false,false,0,0,0};

void TIM1_CC_IRQHandler(void)
{
    //debemos configurar nuestro programa, donde debemos leer de TIMX_CCR1.
    if (TIM1->SR & (1<<1)){
        if (estado.anterior_valido1 == false){
            estado.anterior1 = TIM1->CCR1;
            estado.anterior_valido1 = true;
            }
        else {
            uint32_t actual = TIM1->CCR1;
            estado.diferencia = actual - estado.anterior1;
            estado.frecuencia1 = SystemCoreClock / estado.diferencia;
            estado.anterior1 = actual;
            estado.lectura_valida1 = true;
            }
        
        if (estado.anterior_valido2 == false){
            estado.anterior2 = TIM1->CCR2;
            estado.anterior_valido2 = true;
            }
        else {
            uint32_t actual2 = TIM1->CCR1;
            estado.diferencia = actual2 - estado.anterior2;
            estado.frecuencia2 = SystemCoreClock / estado.diferencia;
            estado.anterior2 = actual2;
            estado.lectura_valida2 = true;
            }
        
        TIM1->SR = (TIM1->SR & ~(1<<1));

    }
    
}

Lectura frecuencimetro_get_frecuencia(void)
{
    return (Lectura){.valida = estado.lectura_valida1,.valor = estado.frecuencia1, .valida2 = estado.lectura_valida2, .valor2=estado.frecuencia2};
}

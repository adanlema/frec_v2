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
    TIM1->CCMR1 = (TIM1->CCMR1 & ~(0b11<<0)) | (1<<0);              // Input Capture CHANNEL1
    TIM1->CCMR1 = (TIM1->CCMR1 & ~(0b11<<8)) | (1<<8);}             // Input Capture CHANNEL2
INLINE void confFlancoAsc(void){
    TIM1->CCER &= ~(1<<1);                                          // Rising Edge CHANNEL1
    TIM1->CCER &= ~(1<<5);}                                         // Rising Edge CHANNEL2
INLINE void habilitarICTIM1(void){
    TIM1->CCER |=(1<<0);                                            // CHANNEL 1
    TIM1->CCER |=(1<<4);}                                           // CHANNEL 2
INLINE void conf_IC(void){
    confTIM1comoIC();
    confFlancoAsc();
    habilitarICTIM1();}

INLINE void habilitarInterrupcionIC(void){
    TIM1->DIER |= (1<<1);
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


typedef struct{
    int32_t anterior;
    bool anterior_valido;
    bool lectura_valida;
    int32_t diferencia;
    int32_t frecuencia;
} estado_Canal;

estado_Canal CH1 = {0,false,false,0,0};
estado_Canal CH2 = {0,false,false,0,0};

void TIM1_CC_IRQHandler(void)
{
    //debemos configurar nuestro programa, donde debemos leer de TIMX_CCR1.
    if (TIM1->SR & (1<<1)){
        if (CH1.anterior_valido == false){
            CH1.anterior = TIM1->CCR1;
            CH1.anterior_valido = true;}
        else {
            uint32_t actual1 = TIM1->CCR1;
            CH1.diferencia = actual1 - CH1.anterior;
            CH1.frecuencia = 400000 / CH1.diferencia;
            CH1.anterior = actual1;
            CH1.lectura_valida = true; } 
        TIM1->SR = (TIM1->SR & ~(1<<1));}
    
    if (TIM1->SR & (1<<2)){
        if (CH2.anterior_valido == false){
            CH2.anterior = TIM1->CCR2;
            CH2.anterior_valido = true;}
        else {
            uint32_t actual2 = TIM1->CCR2;
            CH2.diferencia = actual2 - CH2.anterior;
            CH2.frecuencia = 400000 / CH2.diferencia;
            CH2.anterior = actual2;
            CH2.lectura_valida = true;}
        TIM1->SR = (TIM1->SR & ~(1<<2));}
}

Lectura frecuencimetro_get_frecuencia1(void){
    return (Lectura){.valida = CH1.lectura_valida,.valor = CH1.frecuencia};}

Lectura frecuencimetro_get_frecuencia2(void){
    return (Lectura){.valida = CH2.lectura_valida,.valor = CH2.frecuencia};}

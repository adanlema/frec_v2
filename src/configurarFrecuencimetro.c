#include <stm32f1xx.h>
#include <main.h>

static void inicializaPuerto(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    GPIOA->CRH = (GPIOA->CRH & ~(GPIO_CRH_CNF8_Msk | GPIO_CRH_MODE8_Msk)) | (GPIO_CRH_CNF8_0);
}

static void inicializaTimer(void)
{
    //=============HABILITACION================================================
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; //habilitamos el timer
    //TIM1_CKD por defecto esta en t_DTS = T_CKInit
    TIM1->CR1 |= TIM_CR1_ARPE;          //habilitamos el auto-reload
    //TIM1_CR1_CMS por defecto esta en Edge-Aligned Mode
    //TIM1_CR1_DIR por defecto esta en contador "upcounter"
    TIM1->CR1 |= TIM_CR1_CEN;           //habilitamos el contador
    //TIM1_CR2_TI1S por defecto conecta el canal1 del timer con las entradas.
    //=============CONFIGURAR_MODULO_IC========================================
    TIM1->SR |= TIM_SR_CC1IF;           // habilitamos la deteccion de flancos
    TIM1->CCMR1 |= TIM_CCMR1_CC1S_0;    // configuramos como IC el TIM1
    TIM1->CCER |= TIM_CCER_CC1E;        // habilitamos el IC del TIM1
    //=============PARAMETROS==================================================
    TIM1->CNT = 0;
    TIM1->PSC = 0;
    TIM1->ARR = 0;
    TIM1->RCR = 0;
    //=============HABILITACION_INTERRUPCION===================================
    EXTI->IMR = TIM1_CC_IRQn;           // habilitamos la interrupcion IC
}

void Frecuencimetro_init(void)
{
    inicializaPuerto();
    inicializaTimer();
}
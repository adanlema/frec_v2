#include <main.h>
#include <stdbool.h>

int32_t primer_valor = 0;
int32_t segundo_valor = 0;
int32_t diferencia = 0;
int32_t frecuencia = 0;
bool primera_captura = false;

void TIM1_CC_IRQHandler(void)
{
    //debemos configurar nuestro programa, donde debemos leer de TIMX_CCR1.
    if (primera_captura == false){
        primer_valor = TIM1->CCR1;
        primera_captura = true;
    }
    else {
        segundo_valor = TIM1->CCR1;
        primera_captura = false;
    }
    diferencia = primer_valor - segundo_valor;
    frecuencia = SystemCoreClock / diferencia;
}

int main(void){
    Reloj_init();
    Timer_init();
    while(1){
        /* Debemos mostrar la frecuencia en el monitor/LCD16x2*/
        
    }
    return 0;
}
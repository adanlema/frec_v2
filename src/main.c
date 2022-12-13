#include <main.h>
#include <stdio.h>

int main(void){
    
    reloj_init();
    tim2_init();
    
    /* //Rutina para encender el LED_C13
    RCC->APB2ENR |= (1<<4);
    GPIOC->CRH = (GPIOC->CRH & ~(0xF << 20)) | (2<<20);
    while(1){
            GPIOC->BSRR |= (1<<13);
            delay_ms(3000);
            GPIOC->BRR |= (1<<13);
            delay_ms(1000);} */
    
    lcd_inicio();
    lcd_escribir("Frecuencimetro",0,0);
    lcd_escribir("Lema, Adan J.A.",1,0);
    delay_ms(5000);
    lcd_clear();

    frecuencimetro_init();
    usart_config();
    lcd_escribir("Frec1: ",0,0);
    lcd_escribir("Frec2: ",1,0);

    char F1[10]; 
    char F2[10];
    //  protothreads                        <- maquina estado finita, sencilla y aplicable       
    while(1){
        const Lectura frec_CH1 = frecuencimetro_get_frecuencia1();
        const Lectura frec_CH2 = frecuencimetro_get_frecuencia2();
        const PuertoU U1 = uart_transmitir(); 
        if (frec_CH1.valida){
            snprintf(F1,8,"%lu\n",frec_CH1.valor);
            lcd_escribir(F1,0,7);
        }else{
            lcd_escribir("N/A",0,7);}

        if(frec_CH2.valida){
            snprintf(F2,8,"%lu\n",frec_CH2.valor);
            lcd_escribir(F2,1,7);
        }else{
            lcd_escribir("N/A",1,7);}

        if (U1.estado){
            if(U1.palabra == "F1") usart_sendstring(F1);
            else usart_sendstring(F2);}}
    return 0;
}
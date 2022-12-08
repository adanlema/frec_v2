#include <main.h>
#include <stdio.h>

int main(void){

    reloj_init();
    frecuencimetro_init();
    usart_config();
    tim2_init();
    lcd_init();
    lcd_escribir("Frec1: ",0,0);
    lcd_escribir("Frec2: ",1,0);

    char F1[10]; char F2[10];
    //  protothreads                        <- maquina estado finita, sencilla y aplicable       
    while(1){
        const Lectura frec_CH1 = frecuencimetro_get_frecuencia1();
        const Lectura frec_CH2 = frecuencimetro_get_frecuencia2();
        if (frec_CH1.valida){
            snprintf(F1,8,"%s\n",frec_CH1.valor);
            lcd_escribir(F1,0,7);
        }else{
            lcd_escribir("N/A",0,7);}

        if(frec_CH2.valida){
            snprintf(F2,8,"%s\n",frec_CH2.valor);
            lcd_escribir(F2,1,7);
        }else{
            lcd_escribir("N/A",1,7);}
    }
    return 0;
}
#include <main.h>
#include <stdio.h>

int main(void){

    reloj_init();
    frecuencimetro_init();
    tim6_init();
    lcd_init();
    lcd_escribir("Frec1: ",0,0);
    lcd_escribir("Frec2: ",1,0);


    while(1){
        const Lectura frecuencia = frecuencimetro_get_frecuencia();
        if (frecuencia.valida){
            lcd_escribir(frecuencia.valor,0,7);
        }else{
            lcd_escribir("N/A",0,7);
        }
        if(frecuencia.valida2){
            lcd_escribir(frecuencia.valor2,1,7);
        }else{
            lcd_escribir("N/A",1,7);
        }
    }
    return 0;
}
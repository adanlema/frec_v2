#include <main.h>
#include <stdio.h>

int main(void){

    Reloj__init();
    frecuencimetro_init();
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
    }
    return 0;
}
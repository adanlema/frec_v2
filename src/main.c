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
    char *dir1, *dir2;
    dir1 = &F1; dir2 = &F2;

    while(1){
        const Lectura frecuencia = frecuencimetro_get_frecuencia();
        if (frecuencia.valida){
            sprintf(F1,string,frecuencia.valor);
            *dir1 = F1;
            lcd_escribir(F1,0,7);
        }else{
            lcd_escribir("N/A",0,7);
        }
        if(frecuencia.valida2){
            sprintf(F2,string,frecuencia.valor2);
            *dir2 = F2;
            lcd_escribir(F2,1,7);
        }else{
            lcd_escribir("N/A",1,7);
        }
    }
    return 0;
}
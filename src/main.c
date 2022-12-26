#include <main.h>
#include <stdio.h>


int main(void){
    Interprete interp;
    reloj_init();
    tim2_init();
    Interprete_init(&interp);
    
    lcd_inicio();
    lcd_escribir("Frecuencimetro",0,0);
    lcd_escribir("Lema, Adan J.A.",1,0);
    delay_ms(5000);
    lcd_clear();

    frecuencimetro_init();
    usart3_config();
    lcd_escribir("FQ1:",0,0); lcd_escribir("[Hz]",0,12);
    lcd_escribir("FQ2:",1,0); lcd_escribir("[Hz]",1,12);

    char F1[10]; char D1[3];
    char F2[10]; char D2[3];
    usart3_sendstring("F1 | F2 para leer canales\r\n");
    delay_ms(1000);
    while(1){
        delay_ms(200);
        Interprete_paso(&interp);
        const char * n_a = "  N/A   ";
        const Lectura frec_CH1 = frecuencimetro_get_frecuencia1();
        const Lectura frec_CH2 = frecuencimetro_get_frecuencia2();

        if (frec_CH1.valida){
            snprintf(F1,sizeof(F1),"%5lu",frec_CH1.valor);
            snprintf(D1,sizeof(D1),".%-lu",frec_CH1.decimal);
            lcd_escribir(F1,0,4); lcd_escribir(D1,0,9);
        }else{
            lcd_escribir(n_a,0,4);
            }
        if(frec_CH2.valida){
            snprintf(F2,sizeof(F2),"%5lu",frec_CH2.valor);
            snprintf(D2,sizeof(D2),".%-lu",frec_CH2.decimal);
            lcd_escribir(F2,1,4); lcd_escribir(D2,1,9);
        }else{
            lcd_escribir(n_a,1,4);
            }

        if (interp.estado == E_T){
            switch (interp.canal){
            break;case 1:
                usart3_sendstring(F1);
            break;case 2:
                usart3_sendstring(F2);
            break;default:
                (void)0;
            }
            usart3_sendstring("\r\n");
            interp.transmitido=true;
        }
        
        // uint8_t recibir = usart3_getchar();
        // if(recibir == 'F'){
        //     usart3_sendstring(F1);
        //     delay_ms(1000);
        //}
    }
    return 0;
}
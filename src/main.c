#include <main.h>
#include <stdio.h>


typedef struct Interprete {
    enum {E_INICIO,E_F,E_T} estado;
    int canal;
    bool transmitido;
}Interprete;


void Interprete_paso(Interprete *self)
{
    Opt_uint8_t rx = usart3_opt_getchar();
    switch (self->estado)
    {
        break;case E_INICIO:
            if (rx.valido && rx.valor == 'F') {
                self->estado = E_F;
            }
        break;case E_F:
            if (rx.valido){
                switch (rx.valor){
                    break;case '1':
                        self->estado = E_T;
                        self->canal = 1;
                        self->transmitido = false;
                    break;case '2':
                        self->estado = E_T;
                        self->canal = 2;
                        self->transmitido = false;
                    break;default:
                        self->estado = E_INICIO;
                }
            }
        break;case E_T:
            if (self->transmitido){
                self->estado = E_INICIO;
            }
        break;default:
            self->estado = E_INICIO;
    }
}
void Interprete_init(Interprete *self){
    self->estado = E_INICIO;
    self->canal = 0;
    self->transmitido = false;
}

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
    //usart_config();
    usart3_config();
    lcd_escribir("Frec1: ",0,0);
    lcd_escribir("Frec2: ",1,0);

    char F1[10]; 
    char F2[10];     
    usart3_sendstring("F1 | F2 para leer canales\r\n");
    delay_ms(1000);
    while(1){
        // usart3_sendstring("Hola");
        // delay_ms(1000);
        Interprete_paso(&interp);
        const char * n_a = "N/A      ";
        const Lectura frec_CH1 = frecuencimetro_get_frecuencia1();
        const Lectura frec_CH2 = frecuencimetro_get_frecuencia2();
        if (frec_CH1.valida){
            snprintf(F1,sizeof(F1),"%-9lu",frec_CH1.valor);
            lcd_escribir(F1,0,7);
        }else{
            lcd_escribir(n_a,0,7);}

        if(frec_CH2.valida){
            snprintf(F2,sizeof(F2),"%-9lu",frec_CH2.valor);
            lcd_escribir(F2,1,7);
        }else{
            lcd_escribir(n_a,1,7);}

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
        /*
        uint8_t recibir = usart3_getchar();
        if(recibir == 'F'){
            usart3_sendstring(F1);
            delay_ms(1000);
        }*/
    }
    return 0;
}
#include <main.h>
#include <stdio.h>


typedef struct Interprete {
    enum {E_INICIO,E_F,E_T} estado;
    int canal;
    bool transmitido;
}Interprete;


void Interprete_paso(Interprete *self)
{
    Opt_uint8_t rx = usart_opt_getchar();
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
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    GPIOB->CRL = (GPIOB->CRL & ~(0xf << (1*4))) | (0x2 << (1*4));
    GPIOB->BSRR |= (1<<1);
    usart_config();
    GPIOB->BRR  |= (1<<1); 
    lcd_escribir("Frec1: ",0,0);
    lcd_escribir("Frec2: ",1,0);

    char F1[10]; 
    char F2[10];
    //  protothreads                        <- maquina estado finita, sencilla y aplicable       
    usart_sendstring("F1 | F2 para leer canales\r\n");
    while(1){
        Interprete_paso(&interp);
        const char * n_a = "N/A      ";
        const Lectura frec_CH1 = frecuencimetro_get_frecuencia1();
        const Lectura frec_CH2 = frecuencimetro_get_frecuencia2();
        //const PuertoU U1 = uart_transmitir(); 
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

        // if (interp.estado == E_T){
        //     switch (interp.canal){
        //     break;case 1:
        //         usart_sendstring(F1);
        //     break;case 2:
        //         usart_sendstring(F2);
        //     break;default:
        //         (void)0;
        //     }
        //     usart_sendstring("\r\n");
        //     interp.transmitido=true;
        // }
    }
    return 0;
}
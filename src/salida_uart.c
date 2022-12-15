/* Procedure:
1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
2. Program the M bit in USART_CR1 to define the word length.
3. Program the number of stop bits in USART_CR2.
4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
place. Configure the DMA register as explained in multibuffer communication.
5. Select the desired baud rate using the USART_BRR register. <------ NO
6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
for each data to be transmitted in case of single buffer.
8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
that the transmission of the last frame is complete. This is required for instance when
the USART is disabled or enters the Halt mode to avoid corrupting the last transmission. */

#include <stm32f1xx.h>
#include <stdbool.h>
#include "salida_uart.h"
#define INLINE inline __attribute__((always_inline))

INLINE void usart_confPIN(void){
    RCC->APB2ENR |= (1<<14) | (1<<3) | (1<<0);
    AFIO->MAPR |= (1<<2);                                       // PB6 ^ PB7 para la USART1
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<(4*6))) | (0b1010<<(4*6));      // PB6 TX - push-pull
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<(4*7))) | (0b0100<<(4*7));    // PB7 RX - entrada flotante
}
INLINE void usart_enable(void){
    USART1->CR1 |= (1<<13);
}
INLINE void usart_wordlength(void){
    USART1->CR1 &= ~(1<<12);
}
INLINE void usart_1bitstop(void){
    USART1->CR2 = (USART1->CR2 & ~(0b11<<12));
}
INLINE void usart_baudrate(void){
    SystemCoreClockUpdate();
    USART1->BRR = (SystemCoreClock*16)/(115200*16);//(1<<0) | (0x34<<4);
}
INLINE void usart_TEandRX(void){
    USART1->CR1 |= (1<<3);                                      // Habilitar Transmision
    USART1->CR1 |= (1<<2);                                      // Habilitar Recepcion
}

INLINE bool usart_tx_empty(void){
    return USART1->SR & USART_SR_TXE;
}

void usart_config(void){
    usart_confPIN();
    usart_enable();
    usart_wordlength();                                         // 1 start, 8 data, n stop
    usart_1bitstop();
    usart_baudrate();
    usart_TEandRX();
    
    while(!usart_tx_empty());
}

INLINE void usart_sendchar(char data){
    while(!usart_tx_empty());
    USART1->DR = ((USART1->DR & ~(0xff)) | data);
}

void usart_sendstring(const char *palabra){
    //while (*palabra) usart_sendchar(*palabra++)
    while(*palabra != '\0'){
        usart_sendchar(*palabra);
        palabra++;
    }
}

uint8_t usart_getchar(void){
    uint8_t datousart;
    while(!(USART1->SR & (1<<5))) continue;                     // Espera que la recepcion termine
    datousart = USART1->DR;
    return datousart;
}

// si valido==true entonces valor es el valor uint8_t, si valido==false entonces no hay valor

Opt_uint8_t usart_opt_getchar(void){
    if (!(USART1->SR & USART_SR_RXNE)){
        return (Opt_uint8_t){.valido = false}; // No hay nada 
    }
    // if (GPIOB->ODR & (1<<1)) GPIOB->BRR |= (1<<1);
    // else GPIOB->BSRR |= (1<<1);

    return (Opt_uint8_t){.valido = true,
                         .valor = (uint8_t)USART1->DR};
}


//void USART1_IRQHandler (void){}

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
#include "salida_uart3.h"
#define INLINE inline __attribute__((always_inline))


INLINE bool usart3_tx_empty(void){
    return USART3->SR & USART_SR_TXE;
}

void usart3_config(void){
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    GPIOB->CRH = (GPIOB->CRH & ~(0xf<<8)) | (0b1010<<(4*(10-8)));
    GPIOB->CRH = (GPIOB->CRH & ~(0xf<<12)) | (0b0100<<(4*(11-8)));
    
    USART3->CR1  = USART_CR1_UE;
    USART3->CR1 |= ~USART_CR1_M; 
    USART3->BRR  = (6<<0) | (4<<4); //(6<<0)|(4<<4); (7<<0) | (24<<4)
    USART3->CR1 |= (USART_CR1_RE | USART_CR1_TE);
}

void usart3_sendchar(char data){
    //while(!usart3_tx_empty());
    USART3->DR = data;
    while(!(USART3->SR & USART_SR_TC));
}

void usart3_sendstring(const char *palabra){
    while(*palabra != '\0'){
        usart3_sendchar(*palabra);
        palabra++;
    }
}

uint8_t usart3_getchar(void){
    uint8_t datousart3;
    while(!(USART3->SR & USART_SR_RXNE)) continue;                     // Espera que la recepcion termine
    datousart3 = USART3->DR;
    return datousart3;
}

/*
INLINE void usart3_confPIN(void){
    RCC->APB2ENR |= (1<<14) | (1<<3) | (1<<0);
    AFIO->MAPR |= (1<<2);                                       // PB6 ^ PB7 para la USART3
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<(4*6))) | (0b1010<<(4*6));      // PB6 TX - push-pull
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<(4*7))) | (0b0100<<(4*7));    // PB7 RX - entrada flotante
}
INLINE void usart3_enable(void){
    USART3->CR1 |= (1<<13);
}
INLINE void usart3_wordlength(void){
    USART3->CR1 &= ~(1<<12);
}
INLINE void usart3_1bitstop(void){
    USART3->CR2 = (USART3->CR2 & ~(0b11<<12));
}
INLINE void usart3_baudrate(void){
    SystemCoreClockUpdate();
    USART3->BRR = (5<<0)|(4<<4);
    //USART3->BRR = (SystemCoreClock*16)/(115200*16);//(1<<0) | (0x34<<4);
}
INLINE void usart3_TEandRX(void){
    USART3->CR1 |= (1<<3);                                      // Habilitar Transmision
    USART3->CR1 |= (1<<2);                                      // Habilitar Recepcion
}





INLINE void usart3_sendchar(char data){
    while(!usart3_tx_empty());
    USART3->DR = ((USART3->DR & ~(0xff)) | data);
}

void usart3_sendstring(const char *palabra){
    while(*palabra != '\0'){
        usart3_sendchar(*palabra);
        palabra++;
    }
}

uint8_t usart3_getchar(void){
    uint8_t datousart3;
    while(!(USART3->SR & (1<<5))) continue;                     // Espera que la recepcion termine
    datousart3 = USART3->DR;
    return datousart3;
}

// si valido==true entonces valor es el valor uint8_t, si valido==false entonces no hay valor

Opt_uint8_t usart3_opt_getchar(void){
    if (!(USART3->SR & USART_SR_RXNE)){
        return (Opt_uint8_t){.valido = false}; // No hay nada 
    }
    return (Opt_uint8_t){.valido = true,
                         .valor = (uint8_t)USART3->DR};
}

*/
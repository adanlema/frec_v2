/*
Procedure:
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
the USART is disabled or enters the Halt mode to avoid corrupting the last transmission.
*/

#include <stm32f1xx.h>
#include <stdbool.h>
#define INLINE inline __attribute__((always_inline))

INLINE void usart_enable(void){
    USART1->CR1 |= (1<<13);}
INLINE void usart_wordlength(void){
    USART1->CR1 &= ~(1<<12);}
INLINE void usart_1bitstop(void){
    USART1->CR2 = (USART1->CR2 & ~(3<<12)) | (2<<12);}
INLINE void usart_baudrate(void){
    USART1->BRR &= ~(0xf<<0);}
INLINE void usart_TE(bool valor){
    if(valor) USART1->CR1 |= (1<<3);
    else      USART1->CR1 &=  ~(1<<3);}
INLINE void usart_writebyte(unsigned char data){
    while(!(USART1->CR1 & (1<<7))) continue;
    USART1->DR = ((USART1->DR & ~(0xff)) | data);
    while(!(USART1->CR1 & (1<<6))) continue;
}


void usart_config(void){
    usart_enable();
    usart_wordlength();
    usart_1bitstop();
    usart_baudrate();}

void usart_write(unsigned char *palabra){
    usart_TE(true);
    while(*palabra != '\0'){
        usart_writebyte(*palabra);
        palabra++;}
    usart_TE(false);}
















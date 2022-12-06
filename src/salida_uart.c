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
#define INLINE inline __attribute__((always_inline))

INLINE void usart_confPIN(void){
    AFIO->MAPR |= (1<<2);                                       // PB6 ^ PB7
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<24)) | (0b1010<<24);      // PB6 TX - push-pull
    GPIOB->CRL = (GPIOB->CRL & ~(0xf<<28)) | (0b0100<<28);}     // PB7 RX - entrada flotante
INLINE void usart_enable(void){
    USART1->CR1 |= (1<<13);}
INLINE void usart_wordlength(void){
    USART1->CR1 &= ~(1<<12);}
INLINE void usart_1bitstop(void){
    USART1->CR2 = (USART1->CR2 & ~(3<<12)) | (2<<12);}
INLINE void usart_baudrate(void){
    USART1->BRR = (7<<0) | (24<<4);}
INLINE void usart_TEandRX(bool valor){
    USART1->CR1 |= (1<<3);                                      // Habilitar Transmision
    USART1->CR1 |= (1<<2);}                                     // Habilitar Recepcion




void usart_config(void){
    usart_confPIN();
    usart_enable();
    usart_wordlength();                                         // 1 start, 8 data, n stop
    usart_1bitstop();
    usart_baudrate();}

INLINE void usart_sendchar(unsigned char data){
    USART1->DR = ((USART1->DR & ~(0xff)) | data);
    while(!(USART1->CR1 & (1<<6))) continue;}                   // Espera que la transmision termine

void usart_sendstring(unsigned char *palabra){
    //while (*palabra) usart_sendchar(*palabra++)
    while(*palabra != '\0'){
        usart_sendchar(*palabra);
        palabra++;}}

utin8_t usart_getchar(void){
    uint8_t datousart;
    while(!(USART1->SR & (1<<5))) continue;                     // Espera que la recepcion termine
    datousart = USART1->DR;
    return datousart;
}
















#include <i2c.h>
#define INLINE inline __attribute__((always_inline))


INLINE void habilitar_I2CGPIOB(void){
    RCC->APB1ENR |= (1<<21);
    RCC->APB2ENR |= (1<<3);}        
INLINE void confGPIOB_modopines(void){
    GPIOB->CRH = ((GPIOB->CRH & ~(3<<0))|(2<<0));
    GPIOB->CRH = ((GPIOB->CRH & ~(3<<4))|(2<<4));}
INLINE void confGPIOB_confpines(void){
    GPIOB->CRH |= (3<<2);
    GPIOB->CRH |= (3<<6);}
INLINE void confAFIO_AltFunc(void){
    AFIO->MAPR |= (1<<1);}
INLINE void confI2C_Resetear(void){
    I2C1->CR1 |= (1<<15);
    I2C1->CR1 &= ~(1<<15);}
INLINE void confI2C_FREQ(void){
    I2C1->CR2 = ((I2C1->CR2&~(0b111111)) | (8<<0));}
INLINE void confI2C_RegControlReloj(void){
    I2C1->CCR = ((I2C1->CCR&~(0b111111111111)) | 0x28);}
INLINE void confI2C_RiseTime(void){
    I2C1->TRISE = ((I2C1->TRISE&~(0b111111<<0)) | 8);}
INLINE void confI2C_habilitarPeriferico(void){
    I2C1->CR1 |= (1<<0);}
INLINE void I2C_habilitarReconocimiento(void){
    I2C1->CR1 |= (1<<10);}
INLINE void I2C_start(void){
    I2C1->CR1 |= (1<<8);}
INLINE void I2C_esperarTransmisionRegistrodeDatosvacio(void){
    while(!(I2C1->SR1 & (1<<7))) continue;}
INLINE void I2C_tranferenciaFinalizada(void){
    while(!(I2C1->SR1 & (1<<2))) continue;}
INLINE void I2C_esperarenvioDireccion(void){
    while(!(I2C1->SR1 & (1<<1))) continue;}
INLINE void I2C_borrarReconocimiento(void){
    I2C1->CR1 &= ~(1<<10);}
INLINE void I2C_esperarRecepcionRegistrodeDatosvacio(void){
    while(!(I2C1->SR1 & (1<<6))) continue;}
static void I2C_SubirDatos(uint8_t data){
    I2C1->DR = ((I2C1->DR&~0b11111111)|data);}



static void conf__GPIOB(void)
{   
    habilitar_I2CGPIOB();
    confGPIOB_modopines();
    confGPIOB_confpines();
    confAFIO_AltFunc();}
static void conf__I2C(void){
    confI2C_Resetear();
    confI2C_FREQ();
    confI2C_RegControlReloj();
    confI2C_RiseTime();
    confI2C_habilitarPeriferico();}
void I2C__init(void){
    conf__GPIOB();
    conf__I2C();}


void I2C__start(void){
    I2C_habilitarReconocimiento();
    I2C_start();}


void I2C__write(uint8_t data){
    I2C_esperarTransmisionRegistrodeDatosvacio();
    I2C_SubirDatos(data);
    I2C_tranferenciaFinalizada();}


void I2C__direccion(uint8_t direccion){
    I2C_SubirDatos(direccion);
    I2C_esperarenvioDireccion();
    uint8_t temp = I2C1->SR1 | I2C1->SR2;}


void I2C__stop(void){
    I2C1->CR1 |= (1<<9);}


void I2C__writemulti(uint8_t *data, uint8_t size){
    I2C_esperarTransmisionRegistrodeDatosvacio();
    while(size){
        I2C_esperarTransmisionRegistrodeDatosvacio();
        I2C_SubirDatos((uint32_t)*data++);
        size--;}
    I2C_tranferenciaFinalizada();}


void I2C__read(uint8_t direccion, uint8_t *buffer, uint8_t size){
    int8_t restante = size;

    if(size==1){
        I2C_SubirDatos(direccion);
        I2C_esperarenvioDireccion();

        I2C_borrarReconocimiento();
        uint8_t temp = I2C1->SR1 | I2C1->SR2;
        I2C__stop();
        buffer[size-restante] = I2C1->DR;}

    else{
        I2C_SubirDatos(direccion);
        I2C_esperarenvioDireccion();
        uint8_t temp = I2C1->SR1 | I2C1->SR2;
        while(restante>2){
            I2C_esperarRecepcionRegistrodeDatosvacio();
            buffer[size-restante] = I2C1->DR;
            I2C_habilitarReconocimiento();
            restante--;}
        I2C_esperarRecepcionRegistrodeDatosvacio();
        buffer[size-restante] = I2C1->DR;
        I2C_borrarReconocimiento();
        I2C__stop();
        restante--;
        
        I2C_esperarRecepcionRegistrodeDatosvacio();
        buffer[size-restante] = I2C1->DR;}
}
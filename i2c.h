#ifndef _I2C_H_
#define _I2C_H_

#include "common.h"

// Натройка I2C http://we.easyelectronics.ru/STM32/stm32l-interfeys-i2c-kratenko.html
// http://easyelectronics.ru/interface-bus-iic-i2c.html

#define _I2C_CLK            SystemCoreClock // частота тактирования модуля


void i2c_init(void);
void i2c_writeByte(uint8_t hwAddress, uint8_t regAddress, uint8_t data);
void i2c_writeWord(uint8_t hwAddress, uint8_t regAddress, uint16_t data);
void i2c_writeDWord(uint8_t hwAddress, uint8_t regAddress, uint32_t data);
uint8_t i2c_readByte(uint8_t hwAddress, uint8_t regAddress);
uint16_t i2c_readWord(uint8_t hwAddress, uint8_t regAddress);
uint32_t i2c_readDWord(uint8_t hwAddress, uint8_t regAddress);


#endif // _I2C_H_

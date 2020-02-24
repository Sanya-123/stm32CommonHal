#include "soft_spi.h"

#ifndef SPI_GPIO_CLK
#define SPI_GPIO_CLK                    B, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0
#endif
#ifndef SPI_GPIO_DATA
#define SPI_GPIO_DATA                   B, 1, MODE_OUTPUT_PUSH_PULL, SPEED_50MHZ, 0
#endif
#ifndef SPI_GPIO_IN_DA
#define SPI_GPIO_IN_DA                  B, 1, MODE_INPUT_PULL_DOWN, SPEED_50MHZ, 0
#endif


void initSoftSpi()
{
    PIN_CONFIGURATION(SPI_GPIO_CLK);
    PIN_CONFIGURATION(SPI_GPIO_DATA);
    PIN_CONFIGURATION(SPI_GPIO_IN_DA);
}


static void clkDo(uint32_t *data)//тактирование
{
    PIN_ON(SPI_GPIO_CLK);
    //wait...
    __NOP();
    *data |= (PIN_ISSET(SPI_GPIO_IN_DA) != 0);
    PIN_OFF(SPI_GPIO_CLK);
}

static uint32_t sendData(uint32_t data, uint8_t dataBitSize)
{
    uint32_t inData = 0;
    for(int i = dataBitSize - 1; i >= 0; i--)//нагичаю отправку
    {
        inData = inData << 1;
        if((data >> i) & 1)//по одному байту
        {
            PIN_ON(SPI_GPIO_DATA);
        }
        else
        {
            PIN_OFF(SPI_GPIO_DATA);
        }
        clkDo(&inData);//2xИнвентирую состояния тактирования
    }

    return inData;
}

uint8_t softSpiWrite(uint8_t data)
{
    return (uint8_t)softSpiWriteSomeBit(data, 8);
}

uint16_t softSpiWriteDouble(uint16_t data)
{
    return (uint16_t)softSpiWriteSomeBit(data, 16);
}

uint32_t softSpiWriteWorld(uint32_t data)
{
    return softSpiWriteSomeBit(data, 32);
}

uint32_t softSpiWriteSomeBit(uint32_t data, uint8_t dataSizeBit)
{
    return sendData(data, dataSizeBit);
}

void softSpiWriteData(uint8_t *dataTransmit, uint8_t *dataRecive, uint32_t size)
{
    for(uint32_t i = 0; i < size; i++)
        dataRecive[i] = softSpiWrite(dataTransmit[i]);
}

#include "uart.h"

void initUART_PLL_IN()
{
    RCC->APB2ENR |= RCC_APB2ENR_USART6EN;	// Разрешаем тактирование UART
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    NVIC_EnableIRQ(USART6_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
    //для вызова функции операционки через прерывания
    //должны быть >configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY
    NVIC_SetPriority(USART6_IRQn, 6);
    NVIC_SetPriority(USART2_IRQn, 5);
}

void configureUart(USART_TypeDef *uartC, uint32_t systemFreq, uint32_t baudrate)
{
    // Обычная конструкция вида USART1->BRR = systemFreq/baudrate; даёт ошибку округления,
    // чтобы этого избежать, прибавим 0,5 для получения ближайшего целого.
    // в целочисленной арифметике этому соответcвует следующая конструкция:
        uartC->BRR = (2*systemFreq/baudrate + 1)/2;

        uartC->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;	// включаем UART, разрешаем работу приёмника и передатчика
    //interupt for UART

}

void writeUartData(USART_TypeDef * uartC, uint8_t data)
{
        while((uartC->SR & USART_SR_TXE) == RESET){};	// Дожидаемся освобождения буфера передатчика
        uartC->DR = data;
}

uint8_t readUartData(USART_TypeDef * uartC)
{
        while((uartC->SR & USART_SR_RXNE) == RESET) {}; // Дожидаемся, пока буфер приёмника чем-то заполнится
        return uartC->DR;
}


void watyTransmitDone(USART_TypeDef * uartC)
{
    while((uartC->SR & USART_SR_TC) == RESET) {};
}

void setRxInterupt(USART_TypeDef * uartC, bool en)
{
    if(en)
        uartC->CR1 |= USART_CR1_RXNEIE;//interupt RX en
    else
         uartC->CR1 &= ~USART_CR1_RXNEIE;//interupt RX dis
}

void setTxInterupt(USART_TypeDef * uartC, bool en)
{
    if(en)
        uartC->CR1 |= USART_CR1_TXEIE;//interupt TX en
    else
         uartC->CR1 &= ~USART_CR1_TXEIE;//interupt TX dis
}

void uartPutString(USART_TypeDef * uartC, char *s)
{
    while (*s != 0)
    {
        writeUartData(uartC, *s++);
    }
}

void uartPutInt(USART_TypeDef * uartC, int32_t data)
{
    unsigned char temp[10], count = 0;
    if (data < 0)
    {
        data = -data;
        writeUartData(uartC, '-');
    }
    if (data)
    {
        while (data)
        {
            temp[count++] = data % 10 + '0';
            data /= 10;
        }
        while (count)
        {
            writeUartData(uartC, temp[--count]);
        }
    }
    else
    {
        writeUartData(uartC, '0');
    }
}


/*H**********************************************************************
*
*Description:
*   In this file we want to create a UART communication.
*   In first we must turn on RX and TX pin of GPIO PORT B in alternate 
*   function mode:
*   
*   - First we have to enable GPIOB clock as for led (maybe should test if 
*   it's already set ?) and set PB6 & PB7 as RX and TX.
*   - After that we should set the AFRL register to set the right alternate function
*   for PB6 & PB7
*
*   - After this we have to enable the USART1 clock, the USART1 is connected to the
*   controller via APB2 BUS.
*
*   - Then we can set the USART Clock to PCLK
*
*   - Set the baud rate to 115200
*
*   - Set oversampling to 16
*
*   - Enable USART1 clock
*
*Info:
*   PORT B > PB6 > AF7 = USART1_TX
*   PORT B > PB7 > AF7 = USART1_RX
*
*   GPIOX_MODER: Alternate function mode = 10
*
*   AFSEL > 0111 = AF7
*
*   RCC_APB2ENR: USART1 = bit 14.
*
*H*/

/*
Transmission using DMA
DMA mode can be enabled for transmission by setting DMAT bit in the USART_CR3
register. Data is loaded from a SRAM area configured using the DMA peripheral (refer to
Section 11: Direct memory access controller (DMA) on page 337) to the USART_TDR
register whenever the TXE bit is set. To map a DMA channel for USART transmission, use
the following procedure (x denotes the channel number):
1. Write the USART_TDR register address in the DMA control register to configure it as
the destination of the transfer. The data is moved to this address from memory after
each TXE event.
2. Write the memory address in the DMA control register to configure it as the source of
the transfer. The data is loaded into the USART_TDR register from this memory area
after each TXE event.
3. Configure the total number of bytes to be transferred to the DMA control register.
4. Configure the channel priority in the DMA register
5. Configure DMA interrupt generation after half/ full transfer as required by the
application.
6. Clear the TC flag in the USART_ISR register by setting the TCCF bit in the
USART_ICR register.
7. Activate the channel in the DMA register.
When the number of data transfers programmed in the DMA Controller is reached, the DMA
controller generates an interrupt on the DMA channel interrupt vector.
In transmission mode, once the DMA has written all the data to be transmitted (the TCIF flag
is set in the DMA_ISR register), the TC flag can be monitored to make sure that the USART 
communication is complete. This is required to avoid corrupting the last transmission before
disabling the USART or entering Stop mode. Software must wait until TC=1. The TC flag
remains cleared during all data transfers and it is set by hardware at the end of transmission
of the last frame.
*/

#include <stddef.h>
#include "stm32l475xx.h"
#include "uart.h"

void uart_init()
{
    // reset serial port
    RCC->APB2RSTR = (RCC->APB2RSTR & ~RCC_APB2RSTR_USART1RST_Msk) | (0 << RCC_APB2RSTR_USART1RST_Pos);

    // enable GPIOB clock;
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

    // turning PB6 into 'Alternate function' mode
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE6_Msk) | (0b10 << GPIO_MODER_MODE6_Pos);

    // turning PB7 into 'Alternate function' mode
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE7_Msk) | (0b10 << GPIO_MODER_MODE7_Pos);

    // set PB6 Alternate function as USART1_TX
    GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFSEL6_Msk) | (7 << GPIO_AFRL_AFSEL6_Pos);

    // set PB7 Alternate function as USART1_RX
    GPIOB->AFR[0] = (GPIOB->AFR[0] & ~GPIO_AFRL_AFSEL7_Msk) | (7 << GPIO_AFRL_AFSEL7_Pos);

    // enable USART1 clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    // set USART1 clock as PCLK (Really not sure of this line have to test further) (Maybe not need because reset set to 0)
    RCC->CCIPR = (RCC->CCIPR & ~RCC_CCIPR_USART1SEL_Msk) | (0b00 << RCC_CCIPR_USART1SEL_Pos);

    // set USART1 BRR
    USART1->BRR = (USART1->BRR &~USART_BRR_DIV_FRACTION_Msk) | (694 << USART_BRR_DIV_FRACTION_Pos);

    // turning baud rate to 115200 (Not sure of this too, have to test)
    USART1->BRR = (USART1->BRR & ~USART_BRR_DIV_FRACTION_Msk) | (694 << USART_BRR_DIV_FRACTION_Pos);

    // set oversampling to 16
    // Don't need to set it by default when OVER8 it in USART_CR1 = 0 Oversampling by 16.
    // but need to test

    // enable USART1
    RCC->AHB2ENR |= RCC_APB2ENR_USART1EN;

    // enable transmiter and receiver
    USART1->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;

    // activing DMA mode (DMA mode can be enabled for transmission by setting DMAT bit in the USART_CR3 register)
    USART1->CR3 |= USART_CR3_DMAT;
}

void uart_putchar(uint8_t c)
{
    /*
    Character Transmission Procedure (CTP)
        1. Program the M bits in USART_CR1 to define the word length..................................(✅)
        2. Select the desired baud rate using the USART_BRR register..................................(✅)
        3. Program the number of stop bits in USART_CR2...............................................(✅)
        4. Enable the USART by writing the UE bit in USART_CR1 register to 1..........................(✅)
        5. (NOT NEEDED) Select DMA enable (DMAT) in USART_CR3 if multibuffer
            communication is to take place. Configure the DMA register as explained
            in multibuffer communication..............................................................(❌)
        6. Set the TE bit in USART_CR1 to send an idle frame as first transmission....................(✅)
        7. Write the data to send in the USART_TDR register (this clears the TXE bit). Repeat this....(✅)
            for each data to be transmitted in case of single buffer.
        8. After writing the last data into the USART_TDR register, wait until TC=1. This indicates...(❌)
            that the transmission of the last frame is complete. This is required for instance when
            the USART is disabled or enters the Halt mode to avoid corrupting the last
            transmission.
    */

    // CTP 4.
    USART1->CR1 |= USART_CR1_UE;

    // CTP 6.
    USART1->CR1 |= USART_CR1_TE;

    // CTP 7.
    USART1->TDR = (USART1->TDR & ~USART_TDR_TDR_Msk) | (c << USART_TDR_TDR_Pos);

    // CTP 8.
    while (!(USART1->ISR &USART_ISR_TC_Msk));
}

uint8_t uart_getchar()
{
    return 0;
}

void uart_puts(const uint8_t *s)
{
    while (*s != '\0')
    {
        uart_putchar(*s);
        s++;
    }
}

void uart_gets(uint8_t *s, size_t size)
{

}
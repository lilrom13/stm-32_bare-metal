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
    Character Transmission Procedure (CTP)............................................................(✅)
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
        8. After writing the last data into the USART_TDR register, wait until TC=1. This indicates...(✅)
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
    /*
    Character reception procedure (CRP).............................................................(✅)
        1. Program the M bits in USART_CR1 to define the word length................................(✅)
        2. Select the desired baud rate using the baud rate register USART_BRR......................(✅)
        3. Program the number of stop bits in USART_CR2.............................................(✅)
        4. Enable the USART by writing the UE bit in USART_CR1 register to 1........................(✅)
        5. Select DMA enable (DMAR) in USART_CR3 if multibuffer communication is to take............(❌)
            place. Configure the DMA register as explained in multibuffer communication.
        6. Set the RE bit USART_CR1. This enables the receiver which begins searching for a.........(✅)
            start bit.
    */

   // CRP 4.
    USART1->CR1 |= USART_CR1_UE;

    // CRP 6.
    USART1->CR1 |= USART_CR1_RE;

    while (!(USART1->ISR &USART_ISR_RXNE_Msk));

    return USART1->RDR;
}

void uart_puts(const uint8_t *s)
{
    while (*s != 0)
        uart_putchar(*s++);
    uart_putchar('\n');
    uart_putchar('\r');
}

void uart_gets(uint8_t *s, size_t size)
{
    while(size-- > 1) {
        *s = uart_getchar();        
        if ((*s == '\r') || (*s == '\n')) 
            break;
        s++;
    }
    *s = 0;
}
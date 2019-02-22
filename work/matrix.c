#include "stm32l475xx.h"
#include "matrix.h"

#define RST(x)      (x) ? GPIOC->BRR |= GPIO_BSRR_BS3 : (GPIOC->BRR |= GPIO_BSRR_BR3)
#define SB(x)       (x) ? GPIOC->BRR |= GPIO_BSRR_BS5 : (GPIOC->BRR |= GPIO_BSRR_BR5)
#define LAT(x)      (x) ? GPIOC->BRR |= GPIO_BSRR_BS4 : (GPIOC->BRR |= GPIO_BSRR_BR4)
#define SCK(x)      (x) ? GPIOB->BRR |= GPIO_BSRR_BS1 : (GPIOC->BRR |= GPIO_BSRR_BR1)
#define SDA(x)      (x) ? GPIOA->BRR |= GPIO_BSRR_BS4 : (GPIOC->BRR |= GPIO_BSRR_BR4)
#define ROW0(x)     (x) ? GPIOB->BRR |= GPIO_BSRR_BS2 : (GPIOC->BRR |= GPIO_BSRR_BR2)
#define ROW1(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS15: (GPIOC->BRR |= GPIO_BSRR_BR15)
#define ROW2(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS2 : (GPIOC->BRR |= GPIO_BSRR_BR2)
#define ROW3(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS7 : (GPIOC->BRR |= GPIO_BSRR_BR7)
#define ROW4(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS6 : (GPIOC->BRR |= GPIO_BSRR_BR6)
#define ROW5(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS5 : (GPIOC->BRR |= GPIO_BSRR_BR5)
#define ROW6(x)     (x) ? GPIOB->BRR |= GPIO_BSRR_BS0 : (GPIOC->BRR |= GPIO_BSRR_BR0)
#define ROW7(x)     (x) ? GPIOA->BRR |= GPIO_BSRR_BS3 : (GPIOC->BRR |= GPIO_BSRR_BR3)

void matrix_init()
{
    // Init A, B & C clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

    // configure toutes les broches reliées au driver en mode GPIO Output et haute vitesse
    // A
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE15_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE7_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE6_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE5_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE4_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE3_0;
    GPIOA->MODER = GPIOA->MODER & ~GPIO_MODER_MODE2_0;

    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED15_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED7_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED6_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED5_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED4_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED3_1;
    GPIOA->OSPEEDR = GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED2_1;
 
    // B
    GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODE2_0;
    GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODE1_0;
    GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODE0_0;

    GPIOB->OSPEEDR = GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED2_1;
    GPIOB->OSPEEDR = GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED1_1;
    GPIOB->OSPEEDR = GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED0_1;

    // C
    GPIOC->MODER = GPIOC->MODER & ~GPIO_MODER_MODE5_0;
    GPIOC->MODER = GPIOC->MODER & ~GPIO_MODER_MODE4_0;
    GPIOC->MODER = GPIOC->MODER & ~GPIO_MODER_MODE3_0;

    GPIOC->OSPEEDR = GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED5_1;
    GPIOC->OSPEEDR = GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED4_1;
    GPIOC->OSPEEDR = GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED3_1;

    // RST
    GPIOC->BRR |= GPIO_BSRR_BR3;
    // LAT
    GPIOC->BRR |= GPIO_BSRR_BS4;
    // SB
    GPIOC->BRR |= GPIO_BSRR_BS5;
    // SCK
    GPIOB->BRR |= GPIO_BSRR_BR1;
    // SDA
    GPIOA->BRR |= GPIO_BSRR_BR4;
    // C0
    GPIOB->BRR |= GPIO_BSRR_BR2;
    // C1
    GPIOA->BRR |= GPIO_BSRR_BR15;
    // C2
    GPIOA->BRR |= GPIO_BSRR_BR2;
    // C3
    GPIOA->BRR |= GPIO_BSRR_BR7;
    // C4
    GPIOA->BRR |= GPIO_BSRR_BR6;
    // C5
    GPIOA->BRR |= GPIO_BSRR_BR5;
    // C6
    GPIOB->BRR |= GPIO_BSRR_BR0;
    // C7
    GPIOA->BRR |= GPIO_BSRR_BR3;

    // 100ms
    for (int i = 0; i < 100000; i++)
        asm volatile("nop");

    GPIOC->BRR |= GPIO_BSRR_BS3;
}

void pulse_SCK()
{
    while (1)
    {
        RST(0);
        RST(1);
    }
}

void pulse_LAT()
{
    while (1)
    {
        LAT(0);
        LAT(1);
    }
}

/*
 * This function disable all the rows
 */
void deactivate_rows(void)
{
    ROW0(0);
    ROW1(0);
    ROW2(0);
    ROW3(0);
    ROW4(0);
    ROW5(0);
    ROW6(0);
    ROW7(0);
}

/*
 * This function activate one row
 * 
 * row: the row to activate 
 */
void activate_row(int row)
{
    switch (row)
    {
        case 0:
            ROW0(1);
            break;
        case 1:
            ROW1(1);
            break;
        case 2:
            ROW2(1);
            break;
        case 3:
            ROW3(1);
            break;
        case 4:
            ROW4(1);
            break;
        case 5:
            ROW5(1);
            break;
        case 6:
            ROW6(1);
            break;
        case 7:
            ROW7(1);
            break;
    }
}

void send_byte(uint8_t val, int bank)
{

}

void mat_set_row(int row, const rgb_color *val)
{

}

void init_bank0()
{
   
}
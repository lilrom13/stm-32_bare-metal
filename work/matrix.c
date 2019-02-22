#include "stm32l475xx.h"
#include "matrix.h"
#include <stddef.h>

#define RST(x)    GPIOC->BSRR |= x ? GPIO_BSRR_BS3: GPIO_BSRR_BR3
#define LAT(x)    GPIOC->BSRR |= x ? GPIO_BSRR_BS4: GPIO_BSRR_BR4
#define SB(x)     GPIOC->BSRR |= x ? GPIO_BSRR_BS5: GPIO_BSRR_BR5
#define SCK(x)    GPIOB->BSRR |= x ? GPIO_BSRR_BS1: GPIO_BSRR_BR1
#define SDA(x)    GPIOA->BSRR |= x ? GPIO_BSRR_BS4: GPIO_BSRR_BR4
#define ROW0(x)   GPIOB->BSRR |= x ? GPIO_BSRR_BS2: GPIO_BSRR_BR2
#define ROW1(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS15: GPIO_BSRR_BR15
#define ROW2(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS2: GPIO_BSRR_BR2
#define ROW3(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS7: GPIO_BSRR_BR7
#define ROW4(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS6: GPIO_BSRR_BR6
#define ROW5(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS5: GPIO_BSRR_BR5
#define ROW6(x)   GPIOB->BSRR |= x ? GPIO_BSRR_BS0: GPIO_BSRR_BR0
#define ROW7(x)   GPIOA->BSRR |= x ? GPIO_BSRR_BS3: GPIO_BSRR_BR3

static void pulse_SCK(void);
static void pulse_LAT(void);
static void deactivate_rows(void);
static void activate_row(int);
static void send_byte(uint8_t, int);
static void mat_set_row(int, const rgb_color *);
static void init_bank0();

void matrix_init(){
    // Init A, B & C clock
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

    // configure toutes les broches reliées au driver en mode GPIO Output et haute vitesse
    // A
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE15) | GPIO_MODER_MODE15_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE7) | GPIO_MODER_MODE7_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE6) | GPIO_MODER_MODE6_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE5) | GPIO_MODER_MODE5_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE4) | GPIO_MODER_MODE4_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE3) | GPIO_MODER_MODE3_0;
    GPIOA->MODER = (GPIOA->MODER & ~GPIO_MODER_MODE2) | GPIO_MODER_MODE2_0;

    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED15) | GPIO_OSPEEDR_OSPEED15_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED7) | GPIO_OSPEEDR_OSPEED7_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED6) | GPIO_OSPEEDR_OSPEED6_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED5) | GPIO_OSPEEDR_OSPEED5_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED4) | GPIO_OSPEEDR_OSPEED4_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED3) | GPIO_OSPEEDR_OSPEED3_1;
    GPIOA->OSPEEDR = (GPIOA->OSPEEDR & ~GPIO_OSPEEDR_OSPEED2) | GPIO_OSPEEDR_OSPEED2_1;
 
    // B
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE2) | GPIO_MODER_MODE2_0;
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE1) | GPIO_MODER_MODE1_0;
    GPIOB->MODER = (GPIOB->MODER & ~GPIO_MODER_MODE0) | GPIO_MODER_MODE0_0;

    GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED2) | GPIO_OSPEEDR_OSPEED2_1;
    GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED1) | GPIO_OSPEEDR_OSPEED1_1;
    GPIOB->OSPEEDR = (GPIOB->OSPEEDR & ~GPIO_OSPEEDR_OSPEED0) | GPIO_OSPEEDR_OSPEED0_1;

    // C
    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE5) | GPIO_MODER_MODE5_0;
    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE4) | GPIO_MODER_MODE4_0;
    GPIOC->MODER = (GPIOC->MODER & ~GPIO_MODER_MODE3) | GPIO_MODER_MODE3_0;

    GPIOC->OSPEEDR = (GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED5) | GPIO_OSPEEDR_OSPEED5_1;
    GPIOC->OSPEEDR = (GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED4) | GPIO_OSPEEDR_OSPEED4_1;
    GPIOC->OSPEEDR = (GPIOC->OSPEEDR & ~GPIO_OSPEEDR_OSPEED3) | GPIO_OSPEEDR_OSPEED3_1;

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

    deactivate_rows();

    LAT(1);
    SB(1);
    SCK(0);
    SDA(0);
    RST(0);
    
    for (int i = 0; i < 800000; i++)
        asm volatile("nop");
    
    RST(1);

    init_bank0();
}

static void pulse_SCK()
{
  SCK(0);
  SCK(1);
  SCK(0);
}

static void pulse_LAT()
{
  LAT(1);
  LAT(0);
  LAT(1);
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
    SB(bank);
    uint8_t MSB;

    for(int i=0; i<8; i++){
        MSB = (val & (1<< (7 - i))) ? 1 : 0;
        SDA(MSB);
        pulse_SCK();
    }
}

void mat_set_row(int row, const rgb_color *val)
{
    for(int i = 7; i >= 0; i--){
        send_byte(val->b, 1);
        send_byte(val->g, 1);
        send_byte(val->r, 1);
        val++;
    }
    deactivate_rows();
    for (int i = 0; i < 100; i++)
        asm volatile("nop");    
    pulse_LAT();
    activate_row(row);
}

void init_bank0()
{
    for(int i = 0; i < 18; i++)
        send_byte(0xFF, 0);
    pulse_LAT();
}

void test_pixels()
{
  rgb_color matrix[8][8];

  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
    {
        matrix[i][j].r = (i%3)    ?0:255-(j*32);
        matrix[i][j].g = ((i+1)%3)?0:255-(j*32);
        matrix[i][j].b = ((i+2)%3)?0:255-(j*32);
     }

  for(;;)
  {
      for (int i=0; i<8; i++)
        mat_set_row(i, matrix[i]);
  }
}

void display_image_loop(rgb_color *image)
{
    while (1)
    {
        for (int i = 0; i < 8; i++)
            mat_set_row(i, image + i * 8);
    }
}
#include <stdint.h>

#include "led.h"
/*H**********************************************************************
*
*Description:
*       We want to turn on led #2 of the UM2153 Discovery kit.
*       This led (LD2) is connected to PB14 (GPIO Port B pin 14).
*       Inside the STM32L475VG GPIO B is connected to the
*       ARM cortex-M4 via AHB2 Bus.
*
*       Before being used GPIO B port's, its clock must be enable.
*       For this the bit 1 of the register RCC_AHB2ENR must
*       be set to 1.
*
*       The RCC_AHB2ENR address is 0x4002 1000 + 0x4C of offset
*
*       Once done we can configure the right pin as general purpose
*       output mode (01). As mention above LD2 is mapped to pin 14
*       inside GPIOB.
*
*       GPIOB_MODER = [0x48000400 - 0x48480007FF]
*
*       So we have to set bit 29 to 1 & bit 28 to 0 for the GPIOB_MODER register to 1
*
*       Turn on procedure :
        To turn on the led we have to put the pin PB14 in hight state, for that
*
*H*/

// common address
#define RCC_AHB2ENR_BASE 0x40021000
#define RCC_AHB2ENR_OFFSET 0x4C
#define GPIO_BSRR_OFFSET 0x18

// GPIO ports address
#define GPIOB_MODER_BASE 0x48000400
#define GPIOC_MODER_BASE 0x48000800

#define RCC_AHB2ENR (*(volatile uint32_t *)(RCC_AHB2ENR_BASE+RCC_AHB2ENR_OFFSET))

// GPIO B address
#define GPIOB_MODER (*(volatile uint32_t *)(GPIOB_MODER_BASE))
#define GPIOB_BSRR (*(volatile uint32_t *) (GPIOB_MODER_BASE+GPIO_BSRR_OFFSET))

// GPIO C address
#define GPIOC_MODER (*(volatile uint32_t *) (GPIOC_MODER_BASE))
#define GPIOC_BSRR (*(volatile uint32_t *) (GPIOC_MODER_BASE+GPIO_BSRR_OFFSET))

void led_init()
{
  // enable clock for GPIO B & C
  RCC_AHB2ENR |= (1 << 1) | (1 << 2);

  // turning GPIO B MODE 14 into 'General purpose output mode'
  GPIOB_MODER = (GPIOB_MODER & ~(3 << 28)) | (1 << 28);

  // turning GPIO C MODE 9 into 'General purpose output mode'
  GPIOC_MODER = (GPIOC_MODER & ~(3 << 18)) | (1 << 18);
}

void led_g_on()
{
  GPIOB_BSRR |= (1 << 14);
}

void led_g_off()
{
  GPIOB_BSRR |= (1 << 30);
}

void led_state(state st)
{
  switch (st)
  {
    case LED_OFF:
      GPIOC_MODER = (GPIOC_MODER & ~(0 << 18)) | (1 << 18);
      break;
    case LED_YELLOW:
      GPIOC_BSRR |= (1 << 9);
      break;
    case LED_BLUE:
      GPIOC_BSRR |= (1 << 25);
      break;
  }
}

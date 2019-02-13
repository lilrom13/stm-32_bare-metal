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
*        To turn on the led we have to put the pin PB14 in hight state, for that
*
*H*/

#include <stdint.h>

#include "stm32l475xx.h"
#include "led.h"

void led_init()
{
  // enable clock for GPIO B & C
  RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN;

  // turning GPIO B MODE 14 into 'General purpose output mode'
  GPIOB->MODER = GPIOB->MODER & ~GPIO_MODER_MODE14_1;

  // turning GPIO C MODE 9 into 'General purpose output mode'
  GPIOC->MODER = GPIOB->MODER & ~GPIO_MODER_MODE9_1;
}

void led_g_on()
{
  GPIOB->BSRR |= GPIO_BSRR_BS14;
}

void led_g_off()
{
  GPIOB->BSRR |= GPIO_BSRR_BR14;
}

void led_state(state st)
{
  switch (st)
  {
    case LED_OFF:
      GPIOC->MODER = (GPIOC->MODER & ~(0 << 18)) | (1 << 18);
      break;
    case LED_YELLOW:
      GPIOC->BSRR |= GPIO_BSRR_BS9;
      break;
    case LED_BLUE:
      GPIOC->BSRR |= GPIO_BSRR_BR9;
      break;
  }
}

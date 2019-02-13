#include "led.h"

void delay(int loop)
{
  for (int i = 0; i < loop; i++)
    asm volatile("nop");
}
int fibo(int n)
{
  if (n < 2)
    return n;
  else
    return fibo(n-1) + fibo(n-2);
}

int main(int ac, char **av)
{
  led_init();

  led_g_on();
  
  while (1)
  {
    led_state(LED_OFF);
    delay(100000);
    led_state(LED_BLUE);
    delay(100000);
    led_state(LED_YELLOW);
    delay(100000);
  }

  return fibo(8);
}

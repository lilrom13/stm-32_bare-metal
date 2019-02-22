#include "led.h"
#include "clocks.h"
#include "uart.h"
#include "matrix.h"

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
  clocks_init();
  uart_init();
  matrix_init();
  test_pixels();

  // led_init();

  uint8_t str = 0;
  // uint8_t *s = 0;

  // uart_gets(s, 4);

  while (1)
  {
    uart_gets(&str, 4);
    uart_puts(&str);
  }
  // while (1)
  // {
  //   led_g_on();
  //   delay(100000*10);
  //   led_state(LED_OFF);
  //   delay(100000*10);
  //   led_state(LED_BLUE);
  //   delay(100000*10);
  //   led_state(LED_YELLOW);
  //   led_g_off();
  //   delay(100000*10);
  // }

  return fibo(8);
}

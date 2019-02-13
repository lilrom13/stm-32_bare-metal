#ifndef LED_H
#define LED_H

typedef enum state_e { LED_OFF, LED_YELLOW, LED_BLUE } state;

void led_init(void);
void led_g_on(void);
void led_g_off(void);
void led_state(state);

#endif

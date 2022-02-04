#ifndef BOARD_LED_H /* include guards */
#define BOARD_LED_H

#include "Arduino.h"

#define BOARD_LED_PIN 13

#define BOARD_LED_OFF 0
#define BOARD_LED_ON 1


void board_led_init();
void board_led_set(unsigned int val);

#endif /* BOARD_LED_H */

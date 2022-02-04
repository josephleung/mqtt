#include "board_led.h"

void board_led_init()
{
    pinMode(BOARD_LED_PIN, OUTPUT);
}

void board_led_set(unsigned int val)
{
    digitalWrite(BOARD_LED_PIN, val);
}

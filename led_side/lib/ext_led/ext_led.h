#ifndef EXT_LED_H /* include guards */
#define EXT_LED_H

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>
#include "../../include/global_config.h"

#define LED_PIN 21
#define LED_COUNT 5
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)

void ext_led_init();
void set_ext_led(unsigned int red, unsigned int green, unsigned int blue, unsigned int white);

#endif /* EXT_LED_H */

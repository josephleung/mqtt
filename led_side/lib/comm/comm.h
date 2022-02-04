#ifndef COMM_H /* include guards */
#define COMM_H

#include "Arduino.h"

#include <Adafruit_NeoPixel.h>
#define LED_PIN 21
#define LED_COUNT 5
#define BRIGHTNESS 100 // Set BRIGHTNESS to about 1/5 (max = 255)





void comm_init(void);
void MQTT_connect();
void subUpdate(void);

#endif /* COMM_H */
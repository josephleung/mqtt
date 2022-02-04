#ifndef MQTT_H /* include guards */
#define MQTT_H

#include <Arduino.h>
#include "Adafruit_MQTT_Client.h"
#include "c_wifi.h"
#include "../../include/global_config.h"

/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 8883

// Adafruit IO Account Configuration
// (to obtain these values, visit https://io.adafruit.com and click on Active Key)
#define AIO_USERNAME "josephleung"
#define AIO_KEY "02aed570870d4d30a14b62105e7fd6ea"

#define MQTT_SUCCESS 0
#define MQTT_FAIL 1
#define MQTT_UPDATE 2
#define MQTT_NO_UPDATE 3

int mqtt_init(void);
int mqtt_update(unsigned int *p_red, unsigned int *p_blue, unsigned int *p_green, unsigned int *p_white);

#endif /* MQTT_H */
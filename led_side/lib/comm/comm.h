#ifndef COMM_H /* include guards */
#define COMM_H

#include "Arduino.h"

#include <Adafruit_NeoPixel.h>
#define LED_PIN 21
#define LED_COUNT 5
#define BRIGHTNESS 100 // Set BRIGHTNESS to about 1/5 (max = 255)

#include "WiFiClientSecure.h"
//#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/

#define WLAN_SSID "Shen1"
#define WLAN_PASS "haveaniceday"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER "io.adafruit.com"

// Using port 8883 for MQTTS
#define AIO_SERVERPORT 8883

// Adafruit IO Account Configuration
// (to obtain these values, visit https://io.adafruit.com and click on Active Key)
#define AIO_USERNAME "josephleung"
#define AIO_KEY "02aed570870d4d30a14b62105e7fd6ea"

void comm_init(void);
void MQTT_connect();
void subUpdate(void);

#endif /* COMM_H */
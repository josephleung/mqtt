#ifndef C_OTA_H /* include guards */
#define C_OTA_H

#include "c_wifi.h"
#include "Arduino.h"
#include <ESPmDNS.h>
//#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#define HOSTNAME "esp32_led_side"

void ota_init();
void ota_run();

#endif /* C_OTA_H */

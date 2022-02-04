#ifndef C_WIFI_H /* include guards */
#define C_WIFI_H

#include <Arduino.h>
#include "WiFiClientSecure.h"

#define WLAN_SSID "Shen1"
#define WLAN_PASS "haveaniceday"

extern WiFiClientSecure client;

void c_wifi_init(void);

#endif /* C_WIFI_H */
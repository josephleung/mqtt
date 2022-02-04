#include <Arduino.h>

#include "c_wifi.h"
#include "mqtt.h"
#include "ext_led.h"

void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.println(F("LED side MQTT"));

  c_wifi_init();
  mqtt_init();
  ext_led_init();

}

void loop()
{
  unsigned int red, green, blue, white;
  unsigned int mqtt_result;

  mqtt_result = mqtt_update(&red, &green, &blue, &white);
  if (mqtt_result == MQTT_UPDATE)
  {
    set_ext_led(red, green, blue, white);
  }

}

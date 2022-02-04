#include <Arduino.h>

#include "board_led.h"
#include "ext_led.h"
#include "c_wifi.h"
#include "mqtt.h"
#include "c_ota.h"

void setup()
{
  unsigned int status;

  Serial.begin(115200);
  delay(10);

  Serial.println(F("LED side MQTT"));

  board_led_init();
  ext_led_init();
  c_wifi_init();
  status = mqtt_init();
  if (status == MQTT_SUCCESS)
  {
    board_led_set(BOARD_LED_ON);
  }

  ota_init();
}

void loop()
{
  unsigned int red, green, blue, white;
  unsigned int mqtt_result;

//TODO: only update the updated color, do not touch non-updated ones
  mqtt_result = mqtt_update(&red, &green, &blue, &white);
  if (mqtt_result == MQTT_UPDATE)
  {
    set_ext_led(red, green, blue, white);
  }
  else if (mqtt_result == MQTT_FAIL)
  {
    board_led_set(BOARD_LED_OFF);
  }

  ota_run();
}

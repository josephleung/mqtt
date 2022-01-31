#include <Arduino.h>

#include "comm.h"

void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.println(F("LED side MQTT"));
  comm_init();
  MQTT_connect();
}

void loop()
{
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  subUpdate();
}

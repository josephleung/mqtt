#include "mqtt.h"

extern WiFiClientSecure client;


// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test1");

Adafruit_MQTT_Subscribe sub_led_switch = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led_switch");
Adafruit_MQTT_Subscribe sub_led_color = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led_color");

int mqtt_init(void)
{
    int8_t ret = 0;
    uint8_t retries = 3;

    mqtt.subscribe(&sub_led_switch);
    mqtt.subscribe(&sub_led_color);

    // Stop if already connected.
    if (mqtt.connected())
    {
        return MQTT_FAIL;
    }

    Serial.print("Connecting to MQTT... ");
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
        DBG_PRINT("%s", mqtt.connectErrorString(ret));
        DBG_PRINT("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
        retries--;
        if (retries == 0)
        {
            ret = MQTT_FAIL;
        }
    }

    DBG_PRINT("%s", "MQTT Connected!");
    return MQTT_SUCCESS;
}

int mqtt_update(unsigned int *p_red, unsigned int *p_blue, unsigned int *p_green, unsigned int *p_white)
{
    unsigned int ret = MQTT_NO_UPDATE;
    unsigned int LEN_OF_COLOR_CMD = 7;
    char command[LEN_OF_COLOR_CMD];
    unsigned int red = 0, green = 0, blue = 0, white = 0;

    Adafruit_MQTT_Subscribe *subscription;

    unsigned int LED_switch = 0;

    subscription = mqtt.readSubscription(5000);

    if (subscription == &sub_led_switch)
    {
        PRINT(("Got sub_led_switch: "));
        PRINT((char *)sub_led_switch.lastread);
        memcpy(command, (char *)sub_led_switch.lastread, sizeof(LED_switch));
        // command.trim();
        PRINT("switch command: %s\n", command);
        LED_switch = command[0] - '0';
        if (LED_switch == 1)
        {
            white = 255;
        }
        if (LED_switch == 0)
        {
            white = 0;
        }

        ret = MQTT_UPDATE;
    }
    else if (subscription == &sub_led_color)
    {
        PRINT("Got sub_led_color: ");
        PRINT((char *)sub_led_color.lastread);
        memcpy(command, (char *)sub_led_color.lastread, sizeof(command));
        PRINT("color command: %s\n", command);

        int length = LEN_OF_COLOR_CMD;
        int i;
        char out[LEN_OF_COLOR_CMD];
        for (i = 0; i < length; i++)
        {
            char here = command[i];
            // printf("here: %c, 0x%x  ", (char)here, here);

            if ((here >= '0') && (here <= '9'))
                // printf("here is int: %d ", here - '0');
                out[i] = here - '0';
            else if ((here >= 'A') && (here <= 'F'))
                out[i] = here - 'A' + 9;
            else if ((here >= 'a') && (here <= 'f'))
                out[i] = here - 'a' + 10;

            // printf("out[%d]: int: %d, 0x%x \n", i, out[i], out[i]);
        }

        red = (out[1] << 4) | out[2];
        green = (out[3] << 4) | out[4];
        blue = (out[5] << 4) | out[6];

        // printf("red: 0x%x\n", red);
        // printf("green: 0x%x\n", green);
        // printf("blue: 0x%x\n", blue);
        ret = MQTT_UPDATE;
    }

    // ping the server to keep the mqtt connection alive
    // NOT required if you are publishing once every KEEPALIVE seconds
    if (!mqtt.ping())
    {
        mqtt.disconnect();
        ret = MQTT_FAIL;
    }

    *p_red = red;
    *p_green = green;
    *p_blue = blue;
    *p_white = white;
    
    return ret;
}
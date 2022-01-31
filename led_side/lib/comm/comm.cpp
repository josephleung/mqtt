#include "comm.h"


// Create an WiFiClient class to connect to the MQTT server.
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// io.adafruit.com root CA
const char *adafruitio_root_ca =
    "-----BEGIN CERTIFICATE-----\n"
    "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n"
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n"
    "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n"
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
    "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n"
    "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n"
    "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n"
    "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n"
    "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n"
    "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n"
    "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n"
    "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n"
    "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n"
    "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n"
    "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n"
    "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n"
    "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n"
    "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n"
    "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n"
    "-----END CERTIFICATE-----\n";

/****************************** Feeds ***************************************/

// Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/test1");

Adafruit_MQTT_Subscribe sub_led_switch = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led_switch");
Adafruit_MQTT_Subscribe sub_led_color = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led_color");

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

uint32_t x = 0;
unsigned int LED_switch = 0;
// String command;
#define LEN_OF_COLOR_CMD 7
char command[LEN_OF_COLOR_CMD];

unsigned int red, green, blue;
unsigned long rgb;


void comm_init(void)
{
    // Connect to WiFi access point.
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(WLAN_SSID);

    delay(1000);

    WiFi.begin(WLAN_SSID, WLAN_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Set Adafruit IO's root CA
    client.setCACert(adafruitio_root_ca);

    mqtt.subscribe(&sub_led_switch);
    mqtt.subscribe(&sub_led_color);

    strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();  // Turn OFF all pixels ASAP
    strip.setBrightness(BRIGHTNESS);
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
    int8_t ret;

    // Stop if already connected.
    if (mqtt.connected())
    {
        return;
    }

    Serial.print("Connecting to MQTT... ");

    uint8_t retries = 3;
    while ((ret = mqtt.connect()) != 0)
    { // connect will return 0 for connected
        Serial.println(mqtt.connectErrorString(ret));
        Serial.println("Retrying MQTT connection in 5 seconds...");
        mqtt.disconnect();
        delay(5000); // wait 5 seconds
        retries--;
        if (retries == 0)
        {
            // basically die and wait for WDT to reset me
            while (1)
                ;
        }
    }
    Serial.println("MQTT Connected!");
}



void colorWipe(uint32_t color, int wait)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {                                // For each pixel in strip...
    strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
    strip.show();                  //  Update strip to match
    delay(wait);                   //  Pause for a moment
  }
}

void subUpdate(void)
{
    Adafruit_MQTT_Subscribe *subscription;
    subscription = mqtt.readSubscription(5000);

    if (subscription == &sub_led_switch)
    {
        Serial.print(F("Got sub_led_switch: "));
        Serial.println((char *)sub_led_switch.lastread);
        memcpy(command, (char *)sub_led_switch.lastread, sizeof(LED_switch));
        // command.trim();
        Serial.printf("switch command: %s\n", command);
        LED_switch = command[0] - '0';
    }
    else if (subscription == &sub_led_color)
    {
        Serial.print(F("Got sub_led_color: "));
        Serial.println((char *)sub_led_color.lastread);
        memcpy(command, (char *)sub_led_color.lastread, sizeof(command));
        Serial.printf("color command: %s\n", command);

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
    }

    if (LED_switch == 1)
        colorWipe(strip.Color(red, green, blue, BRIGHTNESS), 0); // True white (not RGB white)
    if (LED_switch == 0)
        colorWipe(strip.Color(red, green, blue, 0), 0); // True white (not RGB white)

    // ping the server to keep the mqtt connection alive
    // NOT required if you are publishing once every KEEPALIVE seconds
    if (!mqtt.ping())
    {
        mqtt.disconnect();
    }
}
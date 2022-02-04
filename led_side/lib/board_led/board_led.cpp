#include "ext_led.h"

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

static void colorWipe(uint32_t color, int wait)
{
    for (int i = 0; i < strip.numPixels(); i++)
    {                                  // For each pixel in strip...
        strip.setPixelColor(i, color); //  Set pixel's color (in RAM)
        strip.show();                  //  Update strip to match
        delay(wait);                   //  Pause for a moment
    }
}

void init_ext_led()
{
    strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.show();  // Turn OFF all pixels ASAP
    strip.setBrightness(BRIGHTNESS);
}

void set_ext_led(unsigned int red, unsigned int green, unsigned int blue, unsigned int white)
{
    colorWipe(strip.Color(red, green, blue, white), 0);
    Serial.printf("red: %d, green: %d, blue: %d, white: %d", red, green, blue, white);
}
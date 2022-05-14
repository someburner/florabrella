#include <Arduino.h>
#include "CPlayExpress.h"
#include "NeoPixel.h"

Adafruit_CPlay_NeoPixel strip;
uint8_t brightness = 32;

static void clearPixels(void)
{
    strip.clear();
    strip.show();
}

// set the color of a neopixel on the board
// p: the pixel to set. Pixel 0 is above the pad labeled 'GND' right next to the
// USB connector, while pixel 9 is above the pad labeled '3.3V' on the other
// side of the USB connector.
// c: a 24bit color value to set the pixel to
static void setPixelColor(uint8_t p, uint32_t c)
{
    strip.setPixelColor(p, c);
    strip.show();
}

static void setPixelColor(uint8_t p, uint8_t r, uint8_t g, uint8_t b)
{
    strip.setPixelColor(p, r, g, b);
    strip.show();
}

// read the onboard lightsensor
// returns value between 0 and 1023 read from the light sensor
// NOTE: 1000 Lux will roughly read as 2 Volts (or about 680 as a raw analog
// reading). A reading of about 300 is common for most indoor light levels.
// Note that outdoor daylight is 10,000 Lux or even higher, so this sensor is
// best suited for indoor light levels!
static uint16_t lightSensor(void)
{
    return analogRead(CPLAY_LIGHTSENSOR);
}

void setup(void)
{
    Serial.begin(115200);
    Serial.println("hello");

    // strip setup
    strip = Adafruit_CPlay_NeoPixel();
    strip.updateType(NEO_GRB + NEO_KHZ800);
    strip.updateLength(19*8);
    strip.setPin(CPLAY_NEOPIXELPIN);

    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(brightness);
}

static void testPixels(void)
{
    Serial.println("testPixels");

    setPixelColor(0, 255,   0,   0);
    setPixelColor(1, 128, 128,   0);
    setPixelColor(2,   0, 255,   0);
    setPixelColor(3,   0, 128, 128);
    setPixelColor(4,   0,   0, 255);

    setPixelColor(5, 0xFF0000);
    setPixelColor(6, 0x808000);
    setPixelColor(7, 0x00FF00);
    setPixelColor(8, 0x008080);
    setPixelColor(9, 0x0000FF);
}

// Configuration to tune the color sensing logic: Amount of time (in millis) to
// wait between changing the pixel color and reading the light sensor.
#define LIGHT_SETTLE_MS 100

static void senseColor(uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // Save the current pixel brightness so it can later be restored.  Then bump
    // the brightness to max to make sure the LED is as bright as possible for
    // the color readings.
    uint8_t old_brightness = strip.getBrightness();
    strip.setBrightness(255);
    // Set pixel 1 (next to the light sensor) to full red, green, blue
    // color and grab a light sensor reading.  Make sure to wait a bit
    // after changing pixel colors to let the light sensor change
    // resistance!
    setPixelColor(1, 255, 0, 0); // Red
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_red = lightSensor();
    setPixelColor(1, 0, 255, 0); // Green
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_green = lightSensor();
    setPixelColor(1, 0, 0, 255); // Blue
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_blue = lightSensor();
    // Turn off the pixel and restore brightness, we're done with readings.
    setPixelColor(1, 0);
    strip.setBrightness(old_brightness);
    // Now scale down each of the raw readings to be within
    // 0 to 255.  Remember each sensor reading is from the ADC
    // which has 10 bits of resolution (0 to 1023), so dividing
    // by 4 will change the range from 0-1023 to 0-255.  Also
    // use the min function to clamp the value to 255 at most (just
    // to prevent overflow from 255.xx to 0).
    red = min(255, raw_red / 4);
    green = min(255, raw_green / 4);
    blue = min(255, raw_blue / 4);
}

// perform a color sense
static void testLightSense(void)
{
    // turn off all the pixels to make sure they don't interfere with reading
    clearPixels();
    // Now take a color reading
    uint8_t red, green, blue;
    senseColor(red, green, blue);
    Serial.print("Color: red=");
    Serial.print(red, DEC);
    Serial.print(" green=");
    Serial.print(green, DEC);
    Serial.print(" blue=");
    Serial.println(blue, DEC);
}


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

void loop(void)
{
    rainbow(20);
    // delay(1000);
    // Serial.println(millis());
    // clearPixels();
    // delay(500);

    // testPixels();
    // testLightSense();


    // delay(1000);
}

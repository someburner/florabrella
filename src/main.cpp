#include <Arduino.h>
#include "CPlayExpress.h"
#include <Adafruit_NeoPixel_ZeroDMA.h>
// #include <Adafruit_NeoPixel.h>
#include <FastLED.h>
#include "LIS3DH.h"

#define DEFAULT_BRIGHTNESS_ONBOARD 32
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
// Adafruit_NeoPixel strip(CPLAY_NUMPIXELS, A2, NEO_GRB);

#define NUM_COLUMNS 8
#define NUM_ROWS    19
#define NUM_LEDS    19*8
CRGB leds[NUM_LEDS];

int matrix[NUM_ROWS][NUM_COLUMNS] = {
    {0,19,38,57,76,95,114,133},
    {1,20,39,58,77,96,115,134},
    {2,21,40,59,78,97,116,135},
    {3,22,41,60,79,98,117,136},
    {4,23,42,61,80,99,118,137},
    {5,24,43,62,81,100,119,138},
    {6,25,44,63,82,101,120,139},
    {7,26,45,64,83,102,121,140},
    {8,27,46,65,84,103,122,141},
    {9,28,47,66,85,104,123,142},
    {10,29,48,67,86,105,124,143},
    {11,30,49,68,87,106,125,144},
    {12,31,50,69,88,107,126,145},
    {13,32,51,70,89,108,127,146},
    {14,33,52,71,90,109,128,147},
    {15,34,53,72,91,110,129,148},
    {16,35,54,73,92,111,130,149},
    {17,36,55,74,93,112,131,150},
    {18,37,56,75,94,113,132,151}
};

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int currentPaletteIndex = 0;
int currentAnimationIndex = 0;

// Gradient palette "bhw4_063_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_063.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 76 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_063_gp ) {
      0,   8,  3,  1,
     20,  50, 17,  1,
     35,  19, 13,  5,
     48, 242,115,  9,
     61, 252,184, 17,
     76, 252,114,  9,
     89,  75, 24,  7,
     99, 252,195, 14,
    117,  75, 24,  7,
    130, 210, 77,  6,
    140, 103, 33,  3,
    153,  10,  9,  9,
    168, 252,213, 21,
    186,  18,  6,  1,
    196,  50, 17,  1,
    209,   6,  4,  2,
    224,  91, 87, 72,
    242,  17,  9,  3,
    255,   4,  1, 12}
;

extern CRGBPalette16 bhw4_063_p = bhw4_063_gp;

// Gradient palette "bhw2_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_50_gp ) {
      0,   8,  2, 23,
     84,  47,  7,102,
    138,  16, 46,147,
    173,   2,127,203,
    255,   1,  7, 11
};

extern CRGBPalette16 bhw2_50_p = bhw2_50_gp;

// Gradient palette "bhw4_062_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_062.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_062_gp ) {
       0,   4,  1, 12,
      15,  10,  2, 25,
      35,  28, 16,138,
      63, 210,108,205,
     107,  47, 18, 74,
     137, 229,244,255,
     153, 165,118,228,
     178,  83, 53,174,
     209,   8,  2, 42,
     242,   1,  1, 12,
     255,   1,  1, 12
};

extern CRGBPalette16 bhw4_062_p = bhw4_062_gp;

LIS3DH accel;

void accel_isr()
{
    Serial.println("isr!");
}

void setup(void)
{
    delay(2000);

    Serial.begin(115200);
    Serial.println("hello");

    Serial.print("LIS3DH init: ");
    accel = LIS3DH(&Wire1); // i2c on wire1
    if(accel.begin(CPLAY_LIS3DH_ADDRESS)) {
        Serial.println("success");
    } else {
        Serial.println("fail");
    }

    // void intConf(uint8_t moveType, uint8_t threshold, uint8_t timeDur, bool polarity);
    accel.intConf(1, 13, 10, 0); // active high
    // attachInterrupt(CPLAY_LIS3DH_INTERRUPT, accel_isr, RISING );

    currentPalette = RainbowStripeColors_p;
    currentBlending = NOBLEND;

    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);
    strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
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

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
            }
        strip.show();
        delay(wait);
    }
}

static void testAccel(void)
{
    accel.read();
    Serial.print(accel.x_g);
    Serial.print(',');
    Serial.print(accel.y_g);
    Serial.print(',');
    Serial.println(accel.z_g);
}

void topToBottomScrollAnimation4( uint8_t colorIndex) {
    currentPalette = bhw2_50_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        int i = matrix[row_index][column_index];
        strip.setPixelColor(i, strip.Color(leds[i].r, leds[i].g, leds[i].b));
      }
      strip.show();
      colorIndex += 3;
    }
}

void topToBottomScrollAnimation5( uint8_t colorIndex) {
    currentPalette = bhw4_062_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        int i = matrix[row_index][column_index];
        strip.setPixelColor(i, strip.Color(leds[i].r, leds[i].g, leds[i].b));
      }
      strip.show();
      colorIndex += 3;
    }
}

void pulsatingColorsAnimation () {
  currentPalette = PartyColors_p;
  static uint8_t brightnessDirection = 0;  // Incrementing
  static uint8_t brightness = 100;
  static uint8_t stepSize = 1;
  static uint8_t pulsatingColorIndex = 0;

  for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
    for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
      leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, pulsatingColorIndex, brightness, currentBlending);
    }
  }

  if (brightness >= 250 && brightnessDirection == 0) {
    brightnessDirection = 1;
  } else if (brightness <= 25 && brightnessDirection == 1) {
    brightnessDirection = 0;
    pulsatingColorIndex = pulsatingColorIndex + 20;
  }
  brightness = brightnessDirection == 0 ? brightness + stepSize : brightness - stepSize;
}

void loop(void)
{
    static uint8_t startIndex = 0;

    // Motion speed
    startIndex = startIndex - 1;

    // topToBottomScrollAnimation4(startIndex);
    topToBottomScrollAnimation5(startIndex);
    // pulsatingColorsAnimation();

    // for(int i = 0; i < NUM_LEDS; i++ ) {
        // strip.setPixelColor(i, leds[i].r, leds[i].g, leds[i].b);
        // strip.setPixelColor(i, strip.Color(leds[i].r, leds[i].g, leds[i].b));
        // delay(1);
    // }

    // strip.show();
    // delay(2);
    // FastLED.show();
    // FastLED.delay(1000 / UPDATES_PER_SECOND);
}

#if 0

// TODO: implement light sense with FastLED

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
#endif

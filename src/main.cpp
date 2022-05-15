#include <Arduino.h>
#include "CPlayExpress.h"
// #include <Adafruit_NeoPixel_ZeroDMA.h>
#include <Adafruit_NeoPixel.h>
#include "LIS3DH.h"

#define DEFAULT_BRIGHTNESS_ONBOARD 32
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
Adafruit_NeoPixel strip(CPLAY_NUMPIXELS, A2, NEO_GRB);

LIS3DH accel;

void accel_isr()
{
    Serial.println("isr!");
}

void setup(void)
{
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

    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);
    strip.show();
}

#define NUM_LEDS 19


// ***************************************
// ** FastLed/NeoPixel Common Functions **
// ***************************************

// Apply LED color changes
void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

// Set a LED color (not yet visible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
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

void BouncingColoredBalls(int BallCount, byte colors[][3], boolean continuous)
{
    float Gravity = -9.81;
    int StartHeight = 1;

    float Height[BallCount];
    float ImpactVelocityStart = sqrt( -2 * Gravity * StartHeight );
    float ImpactVelocity[BallCount];
    float TimeSinceLastBounce[BallCount];
    int   Position[BallCount];
    long  ClockTimeSinceLastBounce[BallCount];
    float Dampening[BallCount];
    boolean ballBouncing[BallCount];
    boolean ballsStillBouncing = true;

    for (int i = 0 ; i < BallCount ; i++) {
    ClockTimeSinceLastBounce[i] = millis();
    Height[i] = StartHeight;
    Position[i] = 0;
    ImpactVelocity[i] = ImpactVelocityStart;
    TimeSinceLastBounce[i] = 0;
    Dampening[i] = 0.90 - float(i)/pow(BallCount,2);
    ballBouncing[i]=true;
    }

    while (ballsStillBouncing) {
    for (int i = 0 ; i < BallCount ; i++) {
    TimeSinceLastBounce[i] =  millis() - ClockTimeSinceLastBounce[i];
    Height[i] = 0.5 * Gravity * pow( TimeSinceLastBounce[i]/1000 , 2.0 ) + ImpactVelocity[i] * TimeSinceLastBounce[i]/1000;

    if ( Height[i] < 0 ) {
    Height[i] = 0;
    ImpactVelocity[i] = Dampening[i] * ImpactVelocity[i];
    ClockTimeSinceLastBounce[i] = millis();

    if ( ImpactVelocity[i] < 0.01 ) {
    if (continuous) {
    ImpactVelocity[i] = ImpactVelocityStart;
    } else {
    ballBouncing[i]=false;
    }
    }
    }
    Position[i] = round( Height[i] * (NUM_LEDS - 1) / StartHeight);
    }

    ballsStillBouncing = false; // assume no balls bouncing
    for (int i = 0 ; i < BallCount ; i++) {
    setPixel(Position[i],colors[i][0],colors[i][1],colors[i][2]);
    if ( ballBouncing[i] ) {
    ballsStillBouncing = true;
    }
    }

    showStrip();
    setAll(0,0,0);
    }
}

void colorWipe(byte red, byte green, byte blue, int SpeedDelay) {
  for(uint16_t i=0; i<NUM_LEDS; i++) {
      setPixel(i, red, green, blue);
      showStrip();
      delay(SpeedDelay);
  }
}

void loop(void)
{
    // rainbow(20);
    // multiple colored balls
    // byte colors[3][3] = { {0xff, 0x00, 0x00},
    //                       {0xff, 0xff, 0xff},
    //                       {0x00, 0x00, 0xff} };
    // BouncingColoredBalls(3, colors, false);
    // colorWipe(0x00,0xff,0x00, 50);
    // colorWipe(0x00,0x00,0x00, 50);
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

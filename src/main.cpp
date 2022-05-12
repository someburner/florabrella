#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"
#include "LIS3DH.h"

#define DATA_PIN_ONBOARD    CPLAY_NEOPIXELPIN
#define LED_TYPE_ONBOARD    WS2812B
#define COLOR_ORDER_ONBOARD RGB
#define NUM_LEDS_ONBOARD    CPLAY_NUMPIXELS
#define DEFAULT_BRIGHTNESS_ONBOARD 32
CRGB leds_onboard[NUM_LEDS_ONBOARD];

#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns


LIS3DH accel;

void setup(void)
{
    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE_ONBOARD,DATA_PIN_ONBOARD,COLOR_ORDER_ONBOARD>(leds_onboard, NUM_LEDS_ONBOARD).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);

    Serial.print("LIS3DH init: ");
    accel = LIS3DH(&Wire1); // i2c on wire1
    if(accel.begin(CPLAY_LIS3DH_ADDRESS)) {
        Serial.println("success");
    } else {
        Serial.println("fail");
    }
}

static void testPixels(void)
{
    // Serial.println("testPixels");
    fill_rainbow(leds_onboard, NUM_LEDS_ONBOARD, gHue, 7);

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS( 50 ) { gHue++; } // slowly cycle the "base color" through the rainbow
    // EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
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


void loop(void)
{
    // testPixels();
    testAccel();
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

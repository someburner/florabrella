#include <Arduino.h>
#include "CPlayExpress.h"
#include <Adafruit_NeoPixel_ZeroDMA.h>
#include "LIS3DH.h"

#define DEFAULT_BRIGHTNESS_ONBOARD 32
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);

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

void rain()
{
    // Create an array of 20 raindrops
    const int count = 19;
    int pos[count];
    // Set each rain drop at the starting gate.
    // Signify by a position of -1
    for(int i=0; i < count; i++) {
        pos[i]=-1;
    }
    // Main loop. Keep looping until we've done
    // enough "frames."
    boolean done=false;
    int counter = 0;
    while(!done) {
        // Start by turning all LEDs off:
        for(int i=0; i<strip.numPixels(); i++)
            strip.setPixelColor(i, 0);

        // Loop for each rain drop
        for(int i=0; i < count; i++) {
            // If the drop is out of the starting gate,
            // turn on the LED for it.
            if( pos[i] >= 0 ) {
                strip.setPixelColor(pos[i], strip.Color(0,0,127));
                // Move the drop down one row
                pos[i] -= 7;
                // If we've fallen off the strip, but us back at the starting gate.
                if( pos[i] < 0 )
                pos[i]=-1;
            }
            // If this drop is at the starting gate, randomly
            // see if we should start it falling.
            if(pos[i] == -1 && random(40) == 0 && counter < 380) {
                // Pick one of the 6 starting spots to begin falling
                pos[i] = 143-random(6);
            }
            strip.show();
            delay(2);
        }
    }
}

void loop(void)
{
    // rainbow(20);
    rain();
    delay(20);
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

#include <Arduino.h>
#include "CPlayExpress.h"
#include "LEDconfig.h"
#include "FastLED.h"
#include "gradients.h"
#include "LUT.h"

// Effects
#include "Bloom.h"
#include "DropDownFade.h"
#include "EdgeRotate.h"
#include "EdgeLoop.h"
#include "PingPong.h"
#include "GradientTest.h"

// #define USE_NEOPIXEL_DMA

#ifdef USE_NEOPIXEL_DMA
#include <Adafruit_NeoPixel_ZeroDMA.h>
#endif

CRGB leds[NUM_LEDS];

#ifdef USE_NEOPIXEL_DMA
Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
#endif

void setup(void)
{
    delay(500);

    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);

#ifdef USE_NEOPIXEL_DMA
    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS);
#endif
}

bool isRunning = false;

// cool
void run_bloom(void)
{
    isRunning = true;
    Bloom bloom = Bloom();
    while(isRunning) bloom.run();
}

// nice but needs improvement
void run_dropdownfade(void)
{
    isRunning = true;
    DropDownFade ddf = DropDownFade();
    while(isRunning) ddf.run();
}

// a bit hard on the eyes but demos it
void run_edgerotate(void)
{
    isRunning = true;
    EdgeRotate er = EdgeRotate();
    while(isRunning) er.run();
}

// kinda cool but needs variation
void run_edgeloop(void)
{
    isRunning = true;
    EdgeLoop el = EdgeLoop();
    while(isRunning) el.run();
}

// also kinda cool but also needs variation
void run_pingpong(void)
{
    isRunning = true;
    PingPong pp = PingPong();
    while(isRunning) pp.run();
}

// tests - some good ones in there
void run_gradienttest(void)
{
    isRunning = true;
    GradientTest gt = GradientTest();
    while(isRunning) gt.run();
}

void loop(void)
{
    // run_dropdownfade();
    // run_pingpong();
    // run_edgeloop();
    // run_edgerotate();
    run_gradienttest();
    // run_bloom();
}

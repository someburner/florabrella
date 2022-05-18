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

// #define USE_NEOPIXEL_DMA

#ifdef USE_NEOPIXEL_DMA
#include <Adafruit_NeoPixel_ZeroDMA.h>
#endif

CRGB leds[NUM_LEDS];
uint8_t colorIndex[NUM_LEDS];

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

    // fill colorIndex array with random numbers
    for(int i = 0; i < NUM_LEDS; i++) {
        colorIndex[i] = random8();
    }
}

/// Move a single led
void color_chase(uint32_t color, uint8_t wait)
{
  FastLED.clear();
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  for(int led_number = 0; led_number < NUM_LEDS; led_number++)
  {
    leds[led_number] = color;
    FastLED.show();
    delay(wait);
    leds[led_number] = CRGB::Black; //Will clear the current pixel on the next show()
  }
  return;
}

uint8_t paletteIndex = 0;

void gradient_test()
{
    uint8_t beatA = beatsin8(120, 0, 255);
    uint8_t beatB = beatsin8(80, 0, 255);
    fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, brownGreen, 128, LINEARBLEND);
    FastLED.show();
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

void loop(void)
{
    // run_dropdownfade();
    // run_pingpong();
    // run_edgeloop();
    // run_edgerotate();
    // gradient_test();
    // run_bloom();

    // color_chase(CRGB::White, 200);



    // for ( int row_index = 0; row_index < BRANCH_LEN; row_index++) {  // 0...18
    //   for( int column_index = 0; column_index < BRANCHES; column_index++) { // 0...7
    //     leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    //   }
    //   colorIndex += 6;
    // }



#if 0
    // fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, myPal, 255, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
#endif

#if 0
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
#endif

#if 0
    // create a sin wave with period of 2 seconds (30bmp) to change brightness
    // limit lowest brightness
    uint8_t sinBeat = beatsin8(30, DEFAULT_BRIGHTNESS, 255, 0, 0);

    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(greenblue, colorIndex[i], sinBeat);
    }

    EVERY_N_MILLISECONDS(5) {
        for(int i = 0; i < NUM_LEDS; i++) {
            colorIndex[i]++;
        }
    }
    FastLED.show();
#endif

#if 0
    uint8_t beatA = beatsin8(30, 0, 255);
    uint8_t beatB = beatsin8(20, 0, 255);
    // fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, orangePink, 255, LINEARBLEND);
    fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
    FastLED.show();
#endif

}

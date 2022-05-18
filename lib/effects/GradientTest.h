#ifndef GRADIENTTEST_EFFECT_H
#define GRADIENTTEST_EFFECT_H

// ping-pongs colors across edges and fades

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class GradientTest {
  public:
    GradientTest(){
        // fill colorIndex array with random numbers
        for(int i = 0; i < NUM_LEDS; i++) {
            colorIndex[i] = random8();
        }
    };
    void run();

  private:
    void beats2();
    void changingPalette();
    void bmp1();
    void bmp2();
    void bmp_rainbow();
    uint8_t test = 0;
    const uint8_t maxTests = 5;
    uint8_t paletteIndex = 0;
    uint8_t colorIndex[NUM_LEDS];
};

// brown green, kinda meh but shows 2 waves, could be cool with some changes,
// better colors maybe
void GradientTest::beats2(void)
{
    uint8_t beatA = beatsin8(120, 0, 255);
    uint8_t beatB = beatsin8(80, 0, 255);
    fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, brownGreen, 128, LINEARBLEND);
    FastLED.show();
}

// this one is kinda cool
void GradientTest::changingPalette(void)
{
    // fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, myPal, 255, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
}

// kinda cool actually, needs more variation
void GradientTest::bmp1(void)
{
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
}

// sorta nice glowing effect
void GradientTest::bmp2(void)
{
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
}

// nice rainbow effect, needs variation
void GradientTest::bmp_rainbow(void)
{
    uint8_t beatA = beatsin8(30, 0, 255);
    uint8_t beatB = beatsin8(20, 0, 255);
    // fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, orangePink, 255, LINEARBLEND);
    fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
    FastLED.show();
}

void GradientTest::run(void)
{
    EVERY_N_SECONDS(5) {
        test = (test + 1) % maxTests;
    }

    switch(test) {
        case 0: beats2(); break;
        case 1: changingPalette(); break;
        case 2: bmp1(); break;
        case 3: bmp2(); break;
        case 4: bmp_rainbow(); break;
    }
}

#endif

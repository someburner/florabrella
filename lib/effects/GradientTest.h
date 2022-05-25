#ifndef GRADIENTTEST_EFFECT_H
#define GRADIENTTEST_EFFECT_H

// ping-pongs colors across edges and fades

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class GradientTest {
  public:
    GradientTest(){
        initColors();
    };
    GradientTest(int anim){
        initColors();
        if(_anim >= 0 && _anim <= 12) {
            _anim = anim;
            _allAnims = false;
        }
    };
    void initColors(void) {
        // fill colorIndex array with random numbers
        for(int i = 0; i < NUM_LEDS; i++) {
            colorIndex[i] = random8();
        }
    };
    void run();

  private:
    void bmp0();
    void bmp1();
    void bmp2();
    void bmp3();
    void bmp4();
    void beats2();
    void changingPalette0();
    void changingPalette1();
    void changingPalette2();
    void changingPalette3();
    void changingPalette4();
    void changingPalette5();
    void changingPalette6();
    void bmp_rainbow();
    uint8_t _anim = 0;
    bool _allAnims = true;
    const uint8_t maxAnims = 12;
    uint8_t hue = 0;
    uint8_t paletteIndex = 0;
    uint8_t colorIndex[NUM_LEDS];

    uint8_t grad_index = 0;
    CRGBPalette16 gradients[3] = {
        heatmap,
        semirainbow,
        blueyellowblue
    };
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
void GradientTest::changingPalette0(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, heatmap, 128, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
    // EVERY_N_SECONDS(30) {
    //     grad_index = (grad_index + 1) % ARRAY_SIZE(gradients);
    //     paletteIndex = 0;
    // }
}

void GradientTest::changingPalette1(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, blueyellowblue, 128, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
}

void GradientTest::changingPalette2(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, blueyellowblue, 128, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
}

void GradientTest::changingPalette3(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, semirainbow, 128, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(8) {
        paletteIndex++;
    }
}

void GradientTest::changingPalette4(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, greenbluepurple, 64, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(15) {
        paletteIndex++;
    }
}

void GradientTest::changingPalette5(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, bluered, 128, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
}

void GradientTest::changingPalette6(void)
{
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, whitefade, 64, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(20) {
        paletteIndex++;
    }
}

// kinda cool actually, needs more variation
void GradientTest::bmp0(void)
{
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CHSV(hue, 255, 255);
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
    EVERY_N_MILLISECONDS(200) {
        hue += 1;
    }
}

// kinda cool actually, needs more variation
void GradientTest::bmp1(void)
{
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS/2-1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, NUM_LEDS/2, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CHSV(hue, 255, 255);
    leds[sinBeat2] = CHSV(hue, 255, 255);
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
    EVERY_N_MILLISECONDS(200) {
        hue += 1;
    }
}

void GradientTest::bmp2(void)
{
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS/4-1, 0, 0);
    uint8_t sinBeat2 = beatsin8(30, NUM_LEDS/4, 2*(NUM_LEDS/4)-1, 0, 0);
    uint8_t sinBeat3 = beatsin8(30, 2*(NUM_LEDS/4), 3*(NUM_LEDS/4)-1, 0, 0);
    uint8_t sinBeat4 = beatsin8(30, 3*(NUM_LEDS/4), NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CHSV(hue, 255, 255);
    leds[sinBeat2] = CHSV(hue, 255, 255);
    leds[sinBeat3] = CHSV(hue, 255, 255);
    leds[sinBeat4] = CHSV(hue, 255, 255);
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
    EVERY_N_MILLISECONDS(200) {
        hue += 1;
    }
}

void GradientTest::bmp3(void)
{
    const int bmp = 120;
    // create a sin wave at bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(bmp, 0, NUM_LEDS/8-1, 0, 0);
    uint8_t sinBeat2 = beatsin8(bmp, 1*(NUM_LEDS/8), 2*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat3 = beatsin8(bmp, 2*(NUM_LEDS/8), 3*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat4 = beatsin8(bmp, 3*(NUM_LEDS/8), 4*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat5 = beatsin8(bmp, 4*(NUM_LEDS/8), 5*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat6 = beatsin8(bmp, 5*(NUM_LEDS/8), 6*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat7 = beatsin8(bmp, 6*(NUM_LEDS/8), 7*(NUM_LEDS/8)-1, 0, 0);
    uint8_t sinBeat8 = beatsin8(bmp, 7*(NUM_LEDS/8), NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CHSV(hue, 255, 196);
    leds[sinBeat2] = CHSV(hue+32, 255, 196);
    leds[sinBeat3] = CHSV(hue+64, 255, 196);
    leds[sinBeat4] = CHSV(hue+96, 255, 196);
    leds[sinBeat5] = CHSV(hue+128, 255, 196);
    leds[sinBeat6] = CHSV(hue+160, 255, 196);
    leds[sinBeat7] = CHSV(hue+196, 255, 196);
    leds[sinBeat8] = CHSV(hue+224, 255, 196);
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
    EVERY_N_MILLISECONDS(20) {
        hue += 1;
    }
}

// sorta nice glowing effect
void GradientTest::bmp4(void)
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
    if(_allAnims) {
        EVERY_N_SECONDS(5) {
            _anim = (_anim + 1) % maxAnims;
        }
    }

    switch(_anim) {
        case 0: bmp0(); break;
        case 1: bmp1(); break;
        case 2: bmp2(); break;
        case 3: bmp3(); break;
        case 4: bmp4(); break;
        case 5: beats2(); break;
        case 6: changingPalette0(); break;
        case 7: changingPalette1(); break;
        case 8: changingPalette2(); break;
        case 9: changingPalette3(); break;
        case 10: changingPalette4(); break;
        case 11: changingPalette5(); break;
        case 12: changingPalette6(); break;
        // case 8: bmp_rainbow(); break;
    }
}

#endif

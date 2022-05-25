#ifndef TOPBOTTOM_EFFECT_H
#define TOPBOTTOM_EFFECT_H

// various topbottom animations from kiwiholmberg repo
// https://github.com/kiwiholmberg/led-umbrella-core

#include "Arduino.h"
#include "Debug.h"
extern CRGB leds[NUM_LEDS];

class TopBottom {
  public:
    TopBottom(){};
    TopBottom(int anim){
        if(_anim >= 0 && _anim <= 10) {
            _anim = anim;
            _allAnims = false;
        }
    };
    void run();

  private:
    void anim0();
    void anim1();
    void anim2();
    void anim3();
    void anim4();
    void anim5();
    void anim6();
    void anim7();
    void anim8();
    void anim9();
    void anim10();

    bool _allAnims = true;
    bool _slower = false;
    uint8_t _anim = 0;
    const uint8_t _maxAnims = 9;

    CRGBPalette16 currentPalette = RainbowStripeColors_p;
    TBlendType    currentBlending = NOBLEND;
    uint8_t       startIndex = 0; // Motion speed
};

// topToBottomScrollAnimation
void TopBottom::anim0(void)
{
    currentPalette = RainbowStripeColors_p;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// circularScrollAnimation
void TopBottom::anim1(void)
{
    currentPalette = PartyColors_p;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;

    for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
        for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// topToBottomScrollAnimation2
void TopBottom::anim2(void)
{
    currentPalette = PartyColors_p;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 6;
    }
    FastLED.show();
}

// topToBottomScrollAnimation3
void TopBottom::anim3(void)
{
    currentPalette = bhw4_063_p;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// topToBottomScrollAnimation4
void TopBottom::anim4(void)
{
    currentPalette = bhw2_50_p;
    uint8_t brightness = 255;
    uint8_t colorIndex = startIndex;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// topToBottomScrollAnimation5
void TopBottom::anim5(void)
{
    currentPalette = bhw4_062_p;
    uint8_t brightness = 255;
    uint8_t colorIndex = startIndex;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// circularScrollAnimation
void TopBottom::anim6(void)
{
    currentPalette = PartyColors_p;
    uint8_t brightness = 128;
    uint8_t colorIndex = startIndex;

    for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
        for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

// runningClusterAnimation
void TopBottom::anim7(void)
{
    currentPalette = RainbowColors_p;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;

    static uint8_t currentClusterCol = 0;
    static uint8_t currentClusterRow = 0;
    static uint8_t skipCount = 0;

    for(int branch = 0; branch < BRANCHES; branch++) {
        for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {
            if (branch == currentClusterCol && brlen == currentClusterRow) {
                // Add colors to 3 pixels in a row. The leading and trailing pixel should be faded.
                // Main col
                leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
                // Next col
                if(branch >= BRANCHES) { // On last column
                    leds[matrix[brlen][0]] = ColorFromPalette(currentPalette, colorIndex, brightness / 2, currentBlending);
                } else {
                    leds[matrix[brlen][branch + 1]] = ColorFromPalette(currentPalette, colorIndex, brightness / 2, currentBlending);
                }
                // Prev col
                if(branch == 0) { // On first column
                    leds[matrix[brlen][BRANCHES - 2]] = ColorFromPalette(currentPalette, colorIndex, brightness / 2, currentBlending);
                } else {
                    leds[matrix[brlen][branch - 1]] = ColorFromPalette(currentPalette, colorIndex, brightness / 2, currentBlending);
                }
            } else {
                leds[matrix[brlen][branch]] = CRGB::Black;
            }
        }
    }

    // Regulate traversing speed
    if(skipCount < 5) {
        skipCount = skipCount + 1;
        return;
    } else {
        skipCount = 0;
    }

    if(currentClusterRow >= BRANCH_LEN - 1) {
        currentClusterRow = 0;
    } else if (currentClusterCol >= BRANCHES - 1) {
        // On last column, switch to next row.
        currentClusterRow = currentClusterRow + 1;
    }

    if(currentClusterCol >= BRANCHES - 1) {
        // Last column
        currentClusterCol = 0;
    } else {
        currentClusterCol = currentClusterCol + 1;
    }
    FastLED.show();
}

// pulsatingColorsAnimation
void TopBottom::anim8(void)
{
    currentPalette = PartyColors_p;
    static uint8_t brightnessDirection = 0;  // Incrementing
    static uint8_t brightness = 100;
    static uint8_t stepSize = 1;
    static uint8_t pulsatingColorIndex = 0;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) { // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, pulsatingColorIndex, brightness, currentBlending);
        }
    }

    if(brightness >= 250 && brightnessDirection == 0) {
        brightnessDirection = 1;
    } else if (brightness <= 25 && brightnessDirection == 1) {
        brightnessDirection = 0;
        pulsatingColorIndex = pulsatingColorIndex + 20;
    }
    brightness = brightnessDirection == 0 ? brightness + stepSize : brightness - stepSize;
    FastLED.show();
}

void TopBottom::anim9(void)
{
    // currentPalette = bhw4_063_p;
    CRGBPalette32 currentPalette = ofaurora;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;
    _slower = true;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}

void TopBottom::anim10(void)
{
    // currentPalette = bhw4_063_p;
    CRGBPalette32 currentPalette = songoflight;
    uint8_t brightness = 96;
    uint8_t colorIndex = startIndex;
    _slower = true;

    for(int brlen = 0; brlen < BRANCH_LEN; brlen++) {  // 0...18
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[brlen][branch]] = ColorFromPalette(currentPalette, colorIndex, brightness, currentBlending);
        }
        colorIndex += 3;
    }
    FastLED.show();
}


void TopBottom::run(void)
{
    if(_allAnims) {
        EVERY_N_SECONDS(5) {
            _anim = (_anim + 1) % _maxAnims;
            DEBUG("anim: "); DEBUGln(_anim);
        }
    }

    if(_slower) {
        EVERY_N_MILLISECONDS(8) {
            // Motion speed
            startIndex = startIndex - 1;
        }
    } else {
        startIndex = startIndex - 1;
    }

    switch(_anim) {
        case 0: anim0(); break;
        case 1: anim1(); break;
        case 2: anim2(); break;
        case 3: anim3(); break;
        case 4: anim4(); break;
        case 5: anim5(); break;
        case 6: anim6(); break;
        case 7: anim7(); break; // cluster
        case 8: anim8(); break;
        case 9: anim9(); break;
        case 10: anim10(); break;
    }
}

#endif

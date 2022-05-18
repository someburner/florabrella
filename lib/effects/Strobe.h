#ifndef STROBE_EFFECT_H
#define STROBE_EFFECT_H

// multiple meteor rain drops

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class Strobe {
  public:
    Strobe(CRGB defaultColor = CRGB::White, uint8_t count = 10, uint8_t flashdelay = 50) {
        _defaultColor = defaultColor;
        _strobeCount = count;
        _flashdelay = flashdelay;
    };
    void run();

  private:
    CRGB _defaultColor = CRGB::White;
    uint8_t _flashdelay = 50;
    uint8_t _strobeCount = 10;
    uint8_t startIndex = 0; // hue change
};

void Strobe::run(void)
{
    // void Strobe(byte red, byte green, byte blue, int StrobeCount, int FlashDelay, int EndPause)
    // Strobe(0xff, 0xff, 0xff, 10, 50, 1000);

    // copy current led array to show between flashes
    CRGB prevleds[NUM_LEDS];
    memcpy(prevleds, leds, sizeof(prevleds));
    for(int i = 0; i < _strobeCount; i++) {
        fill_solid(leds, NUM_LEDS, _defaultColor);
        FastLED.show();
        delay(_flashdelay); // flashdelay
        // fill_solid(leds, NUM_LEDS, CRGB::Black);
        memcpy(leds, prevleds, sizeof(prevleds));
        FastLED.show();
        delay(_flashdelay);
    }
    // delay(EndPause);

    // hue change
    startIndex = startIndex - 1;
}

#endif

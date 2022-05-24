#ifndef DROPDOWNFADE_EFFECT_H
#define DROPDOWNFADE_EFFECT_H

// Pixel dropping down on all branches and slowly fading

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class DropDownFade {
  public:
    DropDownFade(){};
    void run();

  private:
    int pxi = 0;
    uint8_t hue = 0;
};

void DropDownFade::run(void)
{
    EVERY_N_MILLISECONDS(400) {
        pxi++;
        if(pxi == 19) {
            pxi = 0;
            hue += 32;
        }
    }
    for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
        leds[matrix[pxi][branch]] = CHSV(hue, 255, 128);
    }
    EVERY_N_MILLISECONDS(20) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

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
};

void DropDownFade::run(void)
{
    EVERY_N_MILLISECONDS(500) {
        pxi++;
        if(pxi == 19) {
            pxi = 0;
        }
    }
    for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
        leds[matrix[pxi][branch]] = CRGB::Green;
    }
    EVERY_N_MILLISECONDS(5) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

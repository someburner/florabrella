#ifndef EDGELOOP_EFFECT_H
#define EDGELOOP_EFFECT_H

// loops a changing hue around edges

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class EdgeLoop {
  public:
    EdgeLoop(){};
    void run();

  private:
    uint8_t len_ix = 0;
    uint8_t hue = 0; // rotating "base color"
};

void EdgeLoop::run(void)
{
    EVERY_N_MILLISECONDS(20) {
        len_ix++;
        if(len_ix == 2*19 - 1) len_ix = 0;
        // Serial.println(len_ix);
    }
#if 0 // static colors
    leds[edge1[len_ix]] = CRGB::Green;
    leds[edge2[len_ix]] = CRGB::Blue;
    leds[edge3[len_ix]] = CRGB::Red;
    leds[edge4[len_ix]] = CRGB::Purple;
#endif

#if 1 // rainbow comet
    leds[edge1[len_ix]] = CHSV( hue+0, 255, 192);
    leds[edge2[len_ix]] = CHSV( hue+64, 255, 192);
    leds[edge3[len_ix]] = CHSV( hue+128, 255, 192);
    leds[edge4[len_ix]] = CHSV( hue+192, 255, 192);
#endif

    EVERY_N_MILLISECONDS(5) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
        hue++;
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

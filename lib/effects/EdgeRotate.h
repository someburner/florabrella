#ifndef EDGEROTATE_EFFECT_H
#define EDGEROTATE_EFFECT_H

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class EdgeRotate {
  public:
    EdgeRotate(){};
    void run();

  private:
    uint8_t edgeN = 0;
    uint8_t prev_edgeN = 7;
    uint8_t hue = 0;
};

void EdgeRotate::run(void)
{
    EVERY_N_MILLISECONDS(100) {
        edgeN = (edgeN + 1) % 8;
        for(int i = 0; i < BRANCH_LEN; i++) {
            leds[matrix[i][prev_edgeN]] = CHSV( hue+0, 255, 192);
            leds[matrix[i][edgeN]] = CHSV( hue+32, 255, 192);
        }
        prev_edgeN = edgeN;
        hue += 16;
    }

    EVERY_N_MILLISECONDS(10) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }
    delayMicroseconds(100);
    FastLED.show();
}

#endif

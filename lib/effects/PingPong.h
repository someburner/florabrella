#ifndef PINGPONG_EFFECT_H
#define PINGPONG_EFFECT_H

// ping-pongs colors across edges and fades

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class PingPong {
  public:
    PingPong(){};
    void run();

  private:
    int pxi = 0;
    bool direction = true;
    uint8_t prev_edgeN = 7;
};

void PingPong::run(void)
{
    EVERY_N_MILLISECONDS(25) {
        if(direction) {
            pxi++;
            if(pxi == 2*19 - 1) direction = false;
        } else {
            pxi--;
            if(pxi == 0) direction = true;
        }
        // Serial.println(pxi);
    }
    leds[edge1[pxi]] = CRGB::Green;
    leds[edge2[pxi]] = CRGB::Blue;
    leds[edge3[pxi]] = CRGB::Red;
    leds[edge4[pxi]] = CRGB::Purple;

    EVERY_N_MILLISECONDS(10) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

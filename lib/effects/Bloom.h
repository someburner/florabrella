#ifndef BLOOM_EFFECT_H
#define BLOOM_EFFECT_H

// Rainbow hue that fades in and out going up branches

#include "Arduino.h"
#include "mycolorutils.h"
extern CRGB leds[NUM_LEDS];

class Bloom {
  public:
    Bloom(){};
    Bloom(uint8_t hue) {
        _hue = hue;
        _staticHue = true;
    };
    void run();

  private:
    uint8_t _staticHue = false;
    uint8_t _hue = 0;
    int step = 1;
    int bstep = 64;
    bool bstepDir = true;
};

void Bloom::run(void)
{
    // branch len == ring count
    for(int i = 0; i < BRANCH_LEN; i++) {
        CRGB rgb;
        if(_staticHue) {
            rgb = hue_helper(_hue, BRANCH_LEN*2, 0, bstep);
        } else {
            rgb = hue_helper(i, BRANCH_LEN*2, step, bstep);
        }
        // fill_ring
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[i][branch]] = rgb;
        }
    }

    EVERY_N_MILLISECONDS(50) {
        step += 3;
        if(step >= 255) {
            step = 0;
        }
    }

    EVERY_N_MILLISECONDS(20) {
        if(bstepDir) {
            bstep += 3;
        } else {
            bstep -= 3;
        }
        if(bstep >= 224) {
            bstepDir = false;
        }
        if(bstep < 64) {
            bstepDir = true;
        }
    }

#ifdef USE_NEOPIXEL_DMA
    for(int i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, leds[i].r,  leds[i].g, leds[i].b);
    }
    strip.show();
#else
    delayMicroseconds(10);
    FastLED.show();
#endif
}

#endif

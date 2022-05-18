#ifndef METEORRAIN_EFFECT_H
#define METEORRAIN_EFFECT_H

// multiple meteor rain drops

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

class Meteor {
  public:
    Meteor(){};
    void SetBranch(uint8_t branch) {
        _indexShift = branch*19;
        nextMeteor = millis() + random16(0, 2048);
    };
    void draw(CRGB color, uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay, int SpeedDelay);
    void poll(uint32_t now);
    bool running = true;

  private:
    uint32_t nextMeteor = 0;
    uint8_t _indexShift = 0;
    int rainIndex = 0;
};

void Meteor::draw(CRGB color, uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay, int SpeedDelay)
{
    // fade brightness all LEDs one step
    for(int j = 0; j < BRANCH_LEN; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
            leds[j+_indexShift].fadeToBlackBy(meteorTrailDecay);
        }
    }
    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
        if((rainIndex-j < BRANCH_LEN) && (rainIndex-j >= 0)) {
            leds[rainIndex-j+_indexShift] = color;
        }
    }
    rainIndex++;
    if(rainIndex > 3*BRANCH_LEN) {
        running = false;
        nextMeteor = millis() + random16(0, 3096);
        for(int i = 0; i < 19; i++ ) {
            leds[i+_indexShift] = CRGB::Black;
        }
    }
}

void Meteor::poll(uint32_t now)
{
    if(running) {
        draw(CRGB::White, 10, 64, true, 30);
    } else if (now > nextMeteor) {
        running = true;
        rainIndex = 0;
    }
}

class MeteorRain {
  public:
    MeteorRain() {
        for(uint8_t i = 0; i < BRANCHES; i++) {
            meteors[i].SetBranch(i);
        }
    };
    void run();

  private:
    // Meteor m0 = Meteor(0);
    Meteor meteors[8];
};

void MeteorRain::run(void)
{
    uint32_t now = millis();
    EVERY_N_MILLISECONDS(30) {
        // m0.poll(now);
        for(uint8_t i = 0; i < BRANCHES; i++) {
            meteors[i].poll(now);
        }
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

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
    void SetDefaultColor(CRGB color) {
        _defaultColor = color;
    }
    void SetChangeHue(bool changeHue) {
        _changeHue = changeHue;
    }
    void draw(CRGB color, uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay);
    void poll(uint32_t now, uint8_t index);
    bool running = true;

  private:
    CRGB _defaultColor = CRGB::White;
    bool _changeHue = false;
    uint32_t nextMeteor = 0;
    uint8_t _indexShift = 0;
    int rainIndex = 0;
};

void Meteor::draw(CRGB color, uint8_t meteorSize, uint8_t meteorTrailDecay, bool meteorRandomDecay)
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
        nextMeteor = millis() + random16(0, 2048);
        for(int i = 0; i < 19; i++ ) {
            leds[i+_indexShift] = CRGB::Black;
        }
    }
}

void Meteor::poll(uint32_t now, uint8_t index)
{
    if(running) {
        if(_changeHue) {
            uint8_t colorIndex = index;
            CRGB c = CHSV(colorIndex, 255, 192);
            draw(c, 10, 64, true);
        } else {
            draw(_defaultColor, 10, 64, true);
        }
    } else if (now > nextMeteor) {
        running = true;
        rainIndex = 0;
    }
}

class MeteorRain {
  public:
    MeteorRain(CRGB defaultColor = CRGB::White, bool changeHue = false) {
        _changeHue = changeHue;
        _defaultColor = defaultColor;
        initBranches();
    };
    void initBranches(void) {
        for(uint8_t i = 0; i < BRANCHES; i++) {
            meteors[i].SetBranch(i);
            meteors[i].SetDefaultColor(_defaultColor);
            meteors[i].SetChangeHue(_changeHue);
        }
    }
    void run();

  private:
    // Meteor m0 = Meteor(0);
    Meteor meteors[8];
    bool _changeHue = false;
    CRGB _defaultColor = CRGB::White;
    uint8_t startIndex = 0; // hue change
};

void MeteorRain::run(void)
{
    uint32_t now = millis();
    EVERY_N_MILLISECONDS(30) {
        // m0.poll(now);
        for(uint8_t i = 0; i < BRANCHES; i++) {
            meteors[i].poll(now, startIndex);
        }
    }

    // hue change
    startIndex = startIndex - 1;

    delayMicroseconds(100);
    FastLED.show();
}

#endif

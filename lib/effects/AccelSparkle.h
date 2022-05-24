#ifndef ACCEL_SPARKLE_EFFECT_H
#define ACCEL_SPARKLE_EFFECT_H

// introduce varying hue based on accel ISRs, slowly fade

#include "Arduino.h"
extern CRGB leds[NUM_LEDS];

extern LIS3DH accel;
volatile bool gAccelIsr = false;

static void accel_isr()
{
    gAccelIsr = true;
}

class AccelSparkle {
  public:
    AccelSparkle(CRGB defaultColor = CRGB::White) {
        _defaultColor = defaultColor;
        if(accel.isInitialized()) {
            // NOTE: higher threshold == easier detect?
            // accel.intConf(1, 13, 10, 0); // active high
            accel.intConf(1, 48, 10, 0); // active high
            attachInterrupt(CPLAY_LIS3DH_INTERRUPT, accel_isr, RISING);
        }
    };
    ~AccelSparkle() {
        if(accel.isInitialized())
            detachInterrupt(CPLAY_LIS3DH_INTERRUPT);
    };
    void run();

  private:
    CRGB _defaultColor = CRGB::White;
    uint8_t       thishue = 50;
};

void AccelSparkle::run(void)
{
    EVERY_N_MILLISECONDS(25) {
        fadeToBlackBy(leds, NUM_LEDS, 3);
    }
    // fill_solid(leds, NUM_LEDS, _defaultColor);

    cli();
    if(gAccelIsr) {
        gAccelIsr = false;
        sei();
        uint8_t hueDiff = 16;

        for(int i = 0; i < NUM_LEDS / 5; i++) {
            int pos = random16(NUM_LEDS);
            uint8_t   sat = random8(192, 255);
            uint8_t   bri = random8(192, 255);
            // leds[pos] += CHSV(thishue + (random8(hueDiff)/4), sat, bri);
            leds[pos] = CHSV(thishue + (random8(hueDiff)/4), sat, bri);
        }
        thishue += hueDiff;
    } else {
        sei();
    }

    delayMicroseconds(100);
    FastLED.show();
}

#endif

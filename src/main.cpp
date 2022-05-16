#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"

#define DATA_PIN    A2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    19*2
#define DEFAULT_BRIGHTNESS 32
CRGB leds[NUM_LEDS];
uint8_t colorIndex[NUM_LEDS];

DEFINE_GRADIENT_PALETTE(heatmap_gp) {
    0,     0,   0,   0, // black
    128, 255,   0,   0, // red
    200, 255, 255,   0, // bright yellow
    255, 255, 255, 255  // full white
};
CRGBPalette16 myPal = heatmap_gp;

DEFINE_GRADIENT_PALETTE(greenblue_gp) {
      0,   0, 255, 245,
     46,   0,  21, 255,
    179,  12, 250,   0,
    255,   0, 255, 245
};
CRGBPalette16 greenblue = greenblue_gp;

DEFINE_GRADIENT_PALETTE(browngreen_gp) {
      0,   6, 255,   0, // green
     71,   0, 255, 153, // bluegreen
    122, 200, 200, 200, // gray
    181, 110,  61,   6, // brown
    255,   6, 255,   0  // green
};
CRGBPalette16 orangePink = browngreen_gp;

// ***************************************
// ** FastLed/NeoPixel Common Functions **
// ***************************************

// Apply LED color changes
void showStrip() {
    // FastLED
    FastLED.show();
}

// Set a LED color (not yet visible)
void setPixel(int Pixel, byte red, byte green, byte blue) {
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
}

// Set all LEDs to a given color and apply it (visible)
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

void setup(void)
{
    delay(2000);

    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);

    // fill colorIndex array with random numbers
    for(int i = 0; i < NUM_LEDS; i++) {
        colorIndex[i] = random8();
    }
}

/// Move a single led
void color_chase(uint32_t color, uint8_t wait)
{
  FastLED.clear();
  FastLED.setBrightness(DEFAULT_BRIGHTNESS);

  for(int led_number = 0; led_number < NUM_LEDS; led_number++)
  {
    leds[led_number] = color;
    FastLED.show();
    delay(wait);
    leds[led_number] = CRGB::Black; //Will clear the current pixel on the next show()
  }
  return;
}

uint8_t paletteIndex = 0;

#define BRANCHES      8
#define BRANCH_PIXELS 19

// CRGB leds[NUM_LEDS];

void meteorRainMulti(int i, byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
    // fade brightness all LEDs one step
    for(int j=0; j<BRANCH_PIXELS; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
            leds[j].fadeToBlackBy( meteorTrailDecay );
        }
    }

    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
        if( ( i-j <BRANCH_PIXELS) && (i-j>=0) ) {
            setPixel(i-j, red, green, blue);
        }
    }
}

void meteorRainMulti2(int i, byte red, byte green, byte blue, byte meteorSize, byte meteorTrailDecay, boolean meteorRandomDecay, int SpeedDelay) {
    // fade brightness all LEDs one step
    for(int j=0; j<BRANCH_PIXELS; j++) {
        if( (!meteorRandomDecay) || (random(10)>5) ) {
            leds[j+19].fadeToBlackBy( meteorTrailDecay );
        }
    }

    // draw meteor
    for(int j = 0; j < meteorSize; j++) {
        if( ( i-j <BRANCH_PIXELS) && (i-j>=0) ) {
            setPixel(i-j+19, red, green, blue);
        }
    }
}

void loop(void)
{
    // color_chase(CRGB::White, 200);

    static int ix0 = 0;
    static int ix1 = 0;
    static int nextUpdate = 30;
    static int nextUpdate2 = 3700;
    uint32_t now = millis();
    static bool running = false;
    static bool running2 = false;

    if(!running && now > nextUpdate) {
        running = true;
    }

    EVERY_N_MILLISECONDS(30) {
        if(running) {
            meteorRainMulti(ix0, 0xff,0xff,0xff,10, 64, true, 30);
            ix0++;
            if(ix0 > 3*BRANCH_PIXELS) {
                ix0 = 0;
                running = false;
                nextUpdate = now + 3000;
            }
        }
    }

    if(!running2 && now > nextUpdate2) {
        running2 = true;
    }

    EVERY_N_MILLISECONDS(30) {
        if(running2) {
            meteorRainMulti2(ix1, 0xff,0xff,0xff,10, 64, true, 30);
            ix1++;
            if(ix1 > 3*BRANCH_PIXELS) {
                ix1 = 0;
                running2 = false;
                nextUpdate2 = now + 1720;
            }
        }
    }

    FastLED.show();

#if 0
    // fill_palette(leds, NUM_LEDS, paletteIndex, 255 / NUM_LEDS, myPal, 255, LINEARBLEND);
    fill_palette(leds, NUM_LEDS, paletteIndex, 2, myPal, 255, LINEARBLEND);
    FastLED.show();
    EVERY_N_MILLISECONDS(10) {
        paletteIndex++;
    }
#endif

#if 0
    // create a sin wave at 30bmp for all LEDS, no time base or phase shift
    uint8_t sinBeat = beatsin8(30, 0, NUM_LEDS-1, 0, 0);
    leds[sinBeat] = CRGB::Blue;
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
#endif

#if 0
    // create a sin wave with period of 2 seconds (30bmp) to change brightness
    // limit lowest brightness
    uint8_t sinBeat = beatsin8(30, DEFAULT_BRIGHTNESS, 255, 0, 0);

    for(int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(greenblue, colorIndex[i], sinBeat);
    }

    EVERY_N_MILLISECONDS(5) {
        for(int i = 0; i < NUM_LEDS; i++) {
            colorIndex[i]++;
        }
    }
    FastLED.show();
#endif

#if 0
    uint8_t beatA = beatsin8(30, 0, 255);
    uint8_t beatB = beatsin8(20, 0, 255);
    // fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, orangePink, 255, LINEARBLEND);
    fill_rainbow(leds, NUM_LEDS, (beatA+beatB)/2, 8);
    FastLED.show();
#endif

}

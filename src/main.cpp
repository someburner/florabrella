#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"
#include "gradients.h"
#include "LUT.h"

// #define USE_NEOPIXEL_DMA

#ifdef USE_NEOPIXEL_DMA
#include <Adafruit_NeoPixel_ZeroDMA.h>
#endif

#define DATA_PIN    A2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    19*8
#define DEFAULT_BRIGHTNESS 64
CRGB leds[NUM_LEDS];
uint8_t colorIndex[NUM_LEDS];

#ifdef USE_NEOPIXEL_DMA
Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
#endif

void setup(void)
{
    delay(500);

    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);

#ifdef USE_NEOPIXEL_DMA
    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS);
#endif

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

void drop_down_fade(void)
{
    static int pxi = 0;
    EVERY_N_MILLISECONDS(500) {
        pxi++;
        if(pxi == 19) {
            pxi = 0;
        }
    }
    for( int column_index = 0; column_index < BRANCHES; column_index++) { // 0...7
        leds[matrix[pxi][column_index]] = CRGB::Green;
    }
    EVERY_N_MILLISECONDS(5) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }

    delayMicroseconds(100);
    FastLED.show();
}

void ping_pong(void)
{
    static int pxi = 0;
    static bool direction = true;
    EVERY_N_MILLISECONDS(50) {
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

void edge_loop(void)
{
    static int pxi = 0;
    static uint8_t hue = 0; // rotating "base color"
    EVERY_N_MILLISECONDS(20) {
        pxi++;
        if(pxi == 2*19 - 1) pxi = 0;
        // Serial.println(pxi);
    }
#if 0 // static colors
    leds[edge1[pxi]] = CRGB::Green;
    leds[edge2[pxi]] = CRGB::Blue;
    leds[edge3[pxi]] = CRGB::Red;
    leds[edge4[pxi]] = CRGB::Purple;
#endif

#if 1 // rainbow comet
    leds[edge1[pxi]] = CHSV( hue+0, 255, 192);
    leds[edge2[pxi]] = CHSV( hue+64, 255, 192);
    leds[edge3[pxi]] = CHSV( hue+128, 255, 192);
    leds[edge4[pxi]] = CHSV( hue+192, 255, 192);
#endif

    EVERY_N_MILLISECONDS(5) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
        hue++;
    }

    delayMicroseconds(100);
    FastLED.show();
}

void edge_rotate(void)
{
    CRGB::Green;
    static uint8_t edge_num = 0;
    static uint8_t prev_edge = 7;
    static uint8_t hue = 0; // rotating "base color"

    EVERY_N_MILLISECONDS(100) {
        edge_num = (edge_num + 1) % 8;
        for(int i = 0; i < BRANCH_LEN; i++) {
            leds[matrix[i][prev_edge]] = CHSV( hue+0, 255, 192);
            leds[matrix[i][edge_num]] = CHSV( hue+32, 255, 192);
        }
        prev_edge = edge_num;
        hue += 16;
    }

    EVERY_N_MILLISECONDS(10) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }
    delayMicroseconds(100);
    FastLED.show();
}

void gradient_test()
{
    uint8_t beatA = beatsin8(120, 0, 255);
    uint8_t beatB = beatsin8(80, 0, 255);
    fill_palette(leds, NUM_LEDS, (beatA+beatB)/2, 10, brownGreen, 128, LINEARBLEND);
    FastLED.show();
}

CRGB hue_helper(int pos, int length, int step, int bstep)
{
    CHSV hsv( ((pos*255/length) + step) % 255, 255, bstep );
    CRGB rgb;
    hsv2rgb_rainbow(hsv, rgb);  //convert HSV to RGB
    return rgb;
}

void bloom_test(void)
{
    static int step = 1;
    static int bstep = 64;
    // branch len == ring count
    for(int i = 0; i < BRANCH_LEN; i++) {
        CRGB rgb = hue_helper(i, BRANCH_LEN*2, step, bstep);
        // fill_ring
        for(int branch = 0; branch < BRANCHES; branch++) { // 0...7
            leds[matrix[i][branch]] = rgb;
        }
    }

    static bool bstepDir = true;
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

void loop(void)
{
    // drop_down_fade();
    // ping_pong();
    // edge_loop();
    // edge_rotate();
    // gradient_test();
    bloom_test();


    // color_chase(CRGB::White, 200);



    // for ( int row_index = 0; row_index < BRANCH_LEN; row_index++) {  // 0...18
    //   for( int column_index = 0; column_index < BRANCHES; column_index++) { // 0...7
    //     leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    //   }
    //   colorIndex += 6;
    // }



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

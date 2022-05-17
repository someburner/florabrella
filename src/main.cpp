#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"

#define DATA_PIN    A2
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS    19*8
#define DEFAULT_BRIGHTNESS 64
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

#define NUM_COLUMNS 8
#define NUM_ROWS    19

int matrix[NUM_ROWS][NUM_COLUMNS] = {
    {0,19,38,57,76,95,114,133},
    {1,20,39,58,77,96,115,134},
    {2,21,40,59,78,97,116,135},
    {3,22,41,60,79,98,117,136},
    {4,23,42,61,80,99,118,137},
    {5,24,43,62,81,100,119,138},
    {6,25,44,63,82,101,120,139},
    {7,26,45,64,83,102,121,140},
    {8,27,46,65,84,103,122,141},
    {9,28,47,66,85,104,123,142},
    {10,29,48,67,86,105,124,143},
    {11,30,49,68,87,106,125,144},
    {12,31,50,69,88,107,126,145},
    {13,32,51,70,89,108,127,146},
    {14,33,52,71,90,109,128,147},
    {15,34,53,72,91,110,129,148},
    {16,35,54,73,92,111,130,149},
    {17,36,55,74,93,112,131,150},
    {18,37,56,75,94,113,132,151}
};

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


void drop_down_fade(void)
{
    static int pxi = 0;
    EVERY_N_MILLISECONDS(500) {
        pxi++;
        if(pxi == 19) {
            pxi = 0;
        }
    }
    for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[pxi][column_index]] = CRGB::Green;
    }
    EVERY_N_MILLISECONDS(5) {
        fadeToBlackBy(leds, NUM_LEDS, 10);
    }

    delayMicroseconds(100);
    FastLED.show();
}

int edge1[2*NUM_ROWS] = {
    18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
    76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94
};

int edge2[2*NUM_ROWS] = {
    37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19,
    95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113
};

int edge3[2*NUM_ROWS] = {
    56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38,
    114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132
};

int edge4[2*NUM_ROWS] = {
    75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57,
    133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151
};

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

void loop(void)
{
    // drop_down_fade();
    ping_pong();



    // color_chase(CRGB::White, 200);



    // for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...18
    //   for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
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

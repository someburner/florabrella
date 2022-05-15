#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"

// All code from https://github.com/kiwiholmberg/led-umbrella-core

// LED related
#define LED_PIN     A2
#define NUM_COLUMNS 8
#define NUM_ROWS    19
#define NUM_LEDS    NUM_COLUMNS * NUM_ROWS
#define BRIGHTNESS  32
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
#define NUMBER_OF_ANIMATIONS 8

CRGB leds[NUM_LEDS];

// Led numbers arranged in a matrix. Notice first column has reversed order, and all
// other columns increment downwards.
#if 0
int matrix[NUM_ROWS][NUM_COLUMNS] = {
    {17,18,36,54,72,90,108,126},
    {16,19,37,55,73,91,109,127},
    {15,20,38,56,74,92,110,128},
    {14,21,39,57,75,93,111,129},
    {13,22,40,58,76,94,112,130},
    {12,23,41,59,77,95,113,131},
    {11,24,42,60,78,96,114,132},
    {10,25,43,61,79,97,115,133},
    {9,26,44,62,80,98,116,134},
    {8,27,45,63,81,99,117,135},
    {7,28,46,64,82,100,118,136},
    {6,29,47,65,83,101,119,137},
    {5,30,48,66,84,102,120,138},
    {4,31,49,67,85,103,121,139},
    {3,32,50,68,86,104,122,140},
    {2,33,51,69,87,105,123,141},
    {1,34,52,70,88,106,124,142},
    {0,35,53,71,89,107,125,143}
};
#endif

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

#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int currentPaletteIndex = 0;
int currentAnimationIndex = 0;

// Gradient palette "bhw4_063_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_063.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 76 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_063_gp ) {
      0,   8,  3,  1,
     20,  50, 17,  1,
     35,  19, 13,  5,
     48, 242,115,  9,
     61, 252,184, 17,
     76, 252,114,  9,
     89,  75, 24,  7,
     99, 252,195, 14,
    117,  75, 24,  7,
    130, 210, 77,  6,
    140, 103, 33,  3,
    153,  10,  9,  9,
    168, 252,213, 21,
    186,  18,  6,  1,
    196,  50, 17,  1,
    209,   6,  4,  2,
    224,  91, 87, 72,
    242,  17,  9,  3,
    255,   4,  1, 12}
;

extern CRGBPalette16 bhw4_063_p = bhw4_063_gp;

// Gradient palette "bhw2_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw2_50_gp ) {
      0,   8,  2, 23,
     84,  47,  7,102,
    138,  16, 46,147,
    173,   2,127,203,
    255,   1,  7, 11
};

extern CRGBPalette16 bhw2_50_p = bhw2_50_gp;

// Gradient palette "bhw4_062_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_062.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.

DEFINE_GRADIENT_PALETTE( bhw4_062_gp ) {
       0,   4,  1, 12,
      15,  10,  2, 25,
      35,  28, 16,138,
      63, 210,108,205,
     107,  47, 18, 74,
     137, 229,244,255,
     153, 165,118,228,
     178,  83, 53,174,
     209,   8,  2, 42,
     242,   1,  1, 12,
     255,   1,  1, 12
};

extern CRGBPalette16 bhw4_062_p = bhw4_062_gp;

void setup(void)
{
    delay( 3000 ); // power-up safety delay

    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    currentPalette = RainbowStripeColors_p;
    currentBlending = NOBLEND;
}

void topToBottomScrollAnimation( uint8_t colorIndex);
void topToBottomScrollAnimation2( uint8_t colorIndex);
void topToBottomScrollAnimation3( uint8_t colorIndex);
void topToBottomScrollAnimation4( uint8_t colorIndex);
void topToBottomScrollAnimation5( uint8_t colorIndex);
void circularScrollAnimation( uint8_t colorIndex);
void runningClusterAnimation (uint8_t colorIndex);
void pulsatingColorsAnimation();

void loop(void)
{
    static uint8_t startIndex = 0;

    // Motion speed
    startIndex = startIndex - 1;

    switch(currentAnimationIndex) {
      case 0:
        topToBottomScrollAnimation5(startIndex);
        break;
      case 1:
        circularScrollAnimation(startIndex);
        break;
      case 2:
        topToBottomScrollAnimation2(startIndex);
        break;
      case 3:
        pulsatingColorsAnimation();
        break;
      case 4:
        topToBottomScrollAnimation(startIndex);
        break;
      case 5:
        topToBottomScrollAnimation3(startIndex);
        break;
      case 6:
        topToBottomScrollAnimation4(startIndex);
        break;
      case 7:
        runningClusterAnimation(startIndex);
        break;
    }

    FastLED.show();
    // FastLED.delay(1000 / UPDATES_PER_SECOND);

    EVERY_N_SECONDS(5) {
        currentAnimationIndex++;
        if(currentAnimationIndex > 7) currentAnimationIndex = 0;
        Serial.print("currentAnimationIndex = ");
        Serial.println(currentAnimationIndex);
    }
}

void topToBottomScrollAnimation( uint8_t colorIndex) {
    currentPalette = RainbowStripeColors_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}

void topToBottomScrollAnimation2( uint8_t colorIndex) {
    currentPalette = PartyColors_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 6;
    }
}

void topToBottomScrollAnimation3( uint8_t colorIndex) {
    currentPalette = bhw4_063_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}

void topToBottomScrollAnimation4( uint8_t colorIndex) {
    currentPalette = bhw2_50_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}

void topToBottomScrollAnimation5( uint8_t colorIndex) {
    currentPalette = bhw4_062_p;
    uint8_t brightness = 255;

    for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
      for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}

void circularScrollAnimation( uint8_t colorIndex) {
    currentPalette = PartyColors_p;
    uint8_t brightness = 255;

    for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
      for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
      }
      colorIndex += 3;
    }
}

void runningClusterAnimation (uint8_t colorIndex) {
  uint8_t brightness = 255;
  static uint8_t currentClusterCol = 0;
  static uint8_t currentClusterRow = 0;
  currentPalette = RainbowColors_p;
  static uint8_t skipCount = 0;

  for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) {
    for ( int row_index = 0; row_index < (NUM_ROWS - 8); row_index++) {

      if (column_index == currentClusterCol && row_index == currentClusterRow) {
        // Add colors to 3 pixels in a row. The leading and trailing pixel should be faded.

        // Main col
        leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        // Next col
        if (column_index >= NUM_COLUMNS) {  // On last column
          leds[matrix[row_index][0]] = ColorFromPalette( currentPalette, colorIndex, brightness / 2, currentBlending);
        } else {
          leds[matrix[row_index][column_index + 1]] = ColorFromPalette( currentPalette, colorIndex, brightness / 2, currentBlending);
        }
        // Prev col
        if (column_index == 0) { // On first column
          leds[matrix[row_index][NUM_COLUMNS - 2]] = ColorFromPalette( currentPalette, colorIndex, brightness / 2, currentBlending);
        } else {
          leds[matrix[row_index][column_index - 1]] = ColorFromPalette( currentPalette, colorIndex, brightness / 2, currentBlending);
        }
      } else {
        leds[matrix[row_index][column_index]] = CRGB::Black;
      }
    }
  }

  // Regulate traversing speed
  if (skipCount < 5) {
    skipCount = skipCount + 1;
    return;
  } else {
    skipCount = 0;
  }

  if (currentClusterRow >= (NUM_ROWS - 8) - 1) {
    currentClusterRow = 0;
  } else if (currentClusterCol >= NUM_COLUMNS - 1) {
    // On last column, switch to next row.
    currentClusterRow = currentClusterRow + 1;
  }

  if (currentClusterCol >= NUM_COLUMNS - 1) {
    // Last column
    currentClusterCol = 0;
  } else {
    currentClusterCol = currentClusterCol + 1;
  }
}

void pulsatingColorsAnimation () {
  currentPalette = PartyColors_p;
  static uint8_t brightnessDirection = 0;  // Incrementing
  static uint8_t brightness = 100;
  static uint8_t stepSize = 1;
  static uint8_t pulsatingColorIndex = 0;

  for ( int row_index = 0; row_index < NUM_ROWS; row_index++) {  // 0...17
    for( int column_index = 0; column_index < NUM_COLUMNS; column_index++) { // 0...7
      leds[matrix[row_index][column_index]] = ColorFromPalette( currentPalette, pulsatingColorIndex, brightness, currentBlending);
    }
  }

  if (brightness >= 250 && brightnessDirection == 0) {
    brightnessDirection = 1;
  } else if (brightness <= 25 && brightnessDirection == 1) {
    brightnessDirection = 0;
    pulsatingColorIndex = pulsatingColorIndex + 20;
  }
  brightness = brightnessDirection == 0 ? brightness + stepSize : brightness - stepSize;
}

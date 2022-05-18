#ifndef GRADIENTS_H
#define GRADIENTS_H

#include "FastLED.h"

// HOWTO:
// youtube link: https://www.youtube.com/watch?v=Ukq0tH2Tnkc
// github: https://github.com/s-marley/FastLED-basics/tree/main/3.%20Palettes

// RESOURCES:
// premade gradients here: http://soliton.vm.bytemark.co.uk/pub/cpt-city/
// generated with this tool: http://fastled.io/tools/paletteknife/
// custom gradient builds: https://cssgradient.io/

DEFINE_GRADIENT_PALETTE(heatmap_gp) {
    0,     0,   0,   0, // black
    128, 255,   0,   0, // red
    200, 255, 255,   0, // bright yellow
    255, 255, 255, 255  // full white
};
static CRGBPalette16 myPal = heatmap_gp;

DEFINE_GRADIENT_PALETTE(greenblue_gp) {
      0,   0, 255, 245,
     46,   0,  21, 255,
    179,  12, 250,   0,
    255,   0, 255, 245
};
static CRGBPalette16 greenblue = greenblue_gp;

DEFINE_GRADIENT_PALETTE(browngreen_gp) {
      0,   6, 255,   0, // green
     71,   0, 255, 153, // bluegreen
    122, 200, 200, 200, // gray
    181, 110,  61,   6, // brown
    255,   6, 255,   0  // green
};
static CRGBPalette16 brownGreen = browngreen_gp;

#endif

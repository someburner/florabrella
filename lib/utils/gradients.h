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
static CRGBPalette16 heatmap = heatmap_gp;

// Gradient palette "bhw4_051_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_051.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 36 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw4_051_gp) {
    0,   1,  1,  4,
   28,  16, 24, 77,
   66,  35, 87,160,
  101, 125,187,205,
  127, 255,233, 13,
  145, 125,187,205,
  193,  28, 70,144,
  224,  14, 19, 62,
  255,   1,  1,  4
};
static CRGBPalette16 blueyellowblue = bhw4_051_gp;

// Gradient palette "bhw4_029_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_029.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw4_029_gp) {
    0,  28,  7, 75,
   43,  73, 22, 74,
   79, 177,146,197,
  122,  21, 72,137,
  165,  15,184, 75,
  255, 224,205,  4
};
static CRGBPalette16 semirainbow = bhw4_029_gp;

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

// Gradient palette "bhw4_063_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_063.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
DEFINE_GRADIENT_PALETTE(bhw4_063_gp) {
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
    255,   4,  1, 12
};
static CRGBPalette16 bhw4_063_p = bhw4_063_gp;

// Gradient palette "bhw2_50_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_50.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
DEFINE_GRADIENT_PALETTE(bhw2_50_gp) {
      0,   8,  2, 23,
     84,  47,  7,102,
    138,  16, 46,147,
    173,   2,127,203,
    255,   1,  7, 11
};
static CRGBPalette16 bhw2_50_p = bhw2_50_gp;

// Gradient palette "bhw4_062_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw4/tn/bhw4_062.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 44 bytes of program space.
DEFINE_GRADIENT_PALETTE(bhw4_062_gp) {
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
static CRGBPalette16 bhw4_062_p = bhw4_062_gp;

#endif

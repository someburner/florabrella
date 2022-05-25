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

// Gradient palette "bhw1_05_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_05.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 8 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_05_gp) {
    0,   1,221, 53,
  255,  73,  3,178
};
static CRGBPalette16 greenbluepurple = bhw1_05_gp;

// Gradient palette "bhw1_sunset3_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_sunset3.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_sunset3_gp) {
    0, 227,237, 56,
   33, 186, 67,  1,
   71, 163, 21,  1,
   81, 157, 13,  1,
  188,  39, 21, 18,
  234,  12,  7,  4,
  255,  12,  7,  4
};
static CRGBPalette16 sunset = bhw1_sunset3_gp;

// Gradient palette "bhw1_w00t_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_w00t.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_w00t_gp) {
    0,   3, 13, 43,
  104,  78,141,240,
  188, 255,  0,  0,
  255,  28,  1,  1
};
static CRGBPalette16 bluered = bhw1_w00t_gp;

// Gradient palette "bhw1_bluesteel_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_bluesteel.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_bluesteel_gp) {
    0,   1,  2,  1,
   51,  42, 55, 45,
   89, 144,178,170,
  130, 255,255,255,
  146, 194,215,210,
  175, 144,178,170,
  255,   1,  2,  1
};
static CRGBPalette16 bluesteel = bhw1_bluesteel_gp;

// Gradient palette "bhw1_26_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw1/tn/bhw1_26.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 52 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw1_26_gp) {
    0, 107,  1,205,
   35, 255,255,255,
   73, 107,  1,205,
  107,  10,149,210,
  130, 255,255,255,
  153,  10,149,210,
  170,  27,175,119,
  198,  53,203, 56,
  207, 132,229,135,
  219, 255,255,255,
  231, 132,229,135,
  252,  53,203, 56,
  255,  53,203, 56
};
static CRGBPalette16 purplebluegreen = bhw1_26_gp;

// Gradient palette "bhw2_10_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_10.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 28 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw2_10_gp) {
    0,   0, 12,  0,
   61, 153,239,112,
  127,   0, 12,  0,
  165, 106,239,  2,
  196, 167,229, 71,
  229, 106,239,  2,
  255,   0, 12,  0
};
static CRGBPalette16 greenwipe = bhw2_10_gp;

// Gradient palette "bhw2_greenman_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/bhw/bhw2/tn/bhw2_greenman.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE(bhw2_greenman_gp) {
    0,   1, 22,  1,
  130,   1,168,  2,
  255,   1, 22,  1
};
static CRGBPalette16 greenman = bhw2_greenman_gp;

// Gradient palette "ib58_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/ing/general2/tn/ib58.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 68 bytes of program space.

DEFINE_GRADIENT_PALETTE(ib58_gp) {
    0, 118,133,122,
   10,  59, 74, 63,
   28,  87,103, 91,
   35, 121,136,125,
   38,   1,  1,  1,
   43, 103,118,106,
   81,  71, 86, 74,
   89,  82, 97, 85,
  104, 128,142,132,
  122,  22, 33, 25,
  150, 220,225,221,
  165,  97,112,100,
  186,   1,  1,  1,
  193, 133,147,137,
  204, 188,197,190,
  229, 167,178,170,
  255,  22, 32, 24
};
static CRGBPalette16 whitefade = ib58_gp;

// Gradient palette "lifeforce5_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/3/tn/lifeforce5.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 132 bytes of program space.

DEFINE_GRADIENT_PALETTE( lifeforce5_gp ) {
    0, 255,255,255,
   15, 220, 20, 14,
   28,   1,  1,  1,
   35, 255,255,255,
   43, 220, 20, 14,
   51,   1,  1,  1,
   56, 255,255,255,
   61, 220, 20, 14,
   68,   1,  1,  1,
   79, 255,255,255,
   79, 255,255,255,
   89, 220, 20, 14,
   96,   1,  1,  1,
  101,  20,225, 14,
  107, 255,255,255,
  112, 110,108, 11,
  119,   1,  1,  1,
  127, 255,255,255,
  132, 220, 20, 14,
  142,   1,  1,  1,
  145, 255,255,255,
  153, 255,255,255,
  158, 220, 20, 14,
  173,   1,  1,  1,
  181, 255,255,255,
  191, 255,255,255,
  201, 220, 20, 14,
  216,   1,  1,  1,
  221, 255,255,255,
  229, 255,255,255,
  239, 220, 20, 14,
  249,   1,  1,  1,
  255,   1,  1,  1
};
static CRGBPalette32 lifeforce = lifeforce5_gp;

// Gradient palette "ofaurora_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/ofaurora.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 124 bytes of program space.

DEFINE_GRADIENT_PALETTE(ofaurora_gp) {
    0, 255,241,242,
   10, 237,  1,  9,
   15,  42,  1,  2,
   22,   1,  1,  1,
   35, 237,  1,  9,
   48, 255,241,242,
   58, 239, 57,  1,
   66,   1,  1,  1,
   76, 239, 57,  1,
   84, 255,241,242,
   94, 242,217,  1,
  101,   1,  1,  1,
  109, 242,217,  1,
  119, 255,241,242,
  127,   9,144, 36,
  132,   1,  1,  1,
  140,   9,144, 36,
  147, 255,241,242,
  158,   1,156,186,
  168,   1,  1,  1,
  178,   1,156,186,
  186, 255,241,242,
  198,  16, 13,255,
  206,   1,  1,  1,
  216,  16, 13,255,
  224, 255,241,242,
  234,  78,  1,156,
  237,  14,  1, 30,
  239,   1,  1,  1,
  244,  78,  1,156,
  255, 255,241,242
};
static CRGBPalette32 ofaurora = ofaurora_gp;

// Gradient palette "songoflight_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/pj/2/tn/songoflight.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 96 bytes of program space.

DEFINE_GRADIENT_PALETTE(songoflight_gp) {
    0, 255,241,242,
    7, 255,241,242,
   22, 237,  1,  9,
   27, 247, 66, 78,
   35, 255,241,242,
   48, 255,241,242,
   61, 239, 57,  1,
   73, 255,241,242,
   84, 255,241,242,
   94, 242,217,  1,
  109, 255,241,242,
  117, 255,241,242,
  130,   9,144, 36,
  140, 255,241,242,
  147, 255,241,242,
  160,   1,156,186,
  173, 255,241,242,
  178, 255,241,242,
  198,  16, 13,255,
  211, 255,241,242,
  221, 255,241,242,
  234,  78,  1,156,
  244, 255,241,242,
  255, 255,241,242
};
static CRGBPalette32 songoflight = songoflight_gp;

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

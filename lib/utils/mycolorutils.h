#ifndef MYCOLORUTILS_H
#define MYCOLORUTILS_H

#include "FastLED.h"

static inline CRGB hue_helper(int pos, int length, int step, int bstep)
{
    CHSV hsv( ((pos*255/length) + step) % 255, 255, bstep );
    CRGB rgb;
    hsv2rgb_rainbow(hsv, rgb);  //convert HSV to RGB
    return rgb;
}

#endif

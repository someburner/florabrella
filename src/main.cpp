#include <Arduino.h>
#include "CPlayExpress.h"
#include <Adafruit_NeoPixel_ZeroDMA.h>
// #include <Adafruit_NeoPixel.h>
#include "LIS3DH.h"

#define DEFAULT_BRIGHTNESS_ONBOARD 32
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
// Adafruit_NeoPixel strip(CPLAY_NUMPIXELS, A2, NEO_GRB);

/* Neopixel strip layout */
const int strip_pin = 6;
const int geometry[] = {19, 19, 19, 19, 19, 19, 19, 19};
const int nPixels = 152;

/* Framerate (in milliseconds):
 * advisory as a maximum framerate; i.e. frames will never render FASTER than
 * this rate. Animations may use this for converting anticipated durations into
 * some number of arbitrary animation frames. */
const uint16_t framerate = 20;

/** Global State **/
const int nStrips = sizeof(geometry) / sizeof(geometry[0]);
int stripAddrs[nStrips];

float lum[nPixels]; // Luminance map for pixel strip
int buttonState;    // variable to hold the button state
int lightMode;      // how many times the button has been pressed

uint8_t pColorRGB[3]; // Current color RGB
uint32_t pcolor;      // Current color
byte gammatable[256];


void setColor(uint8_t r, uint8_t g, uint8_t b);
void setColor(uint32_t color);

void setup(void)
{
    delay(3000);

    Serial.begin(115200);
    Serial.println("hello");

    int i, j;

    // Set up the pixel strips
    for(i = j = 0; i < nStrips; i++) {
        stripAddrs[i] = j;
        j += geometry[i];
    }

    // thanks PhilB for this gamma table!
    // it helps convert RGB colors to what humans see
    for(i=0; i<256; i++) {
        float x = i;
        x /= 255;
        x = pow(x, 2.5);
        x *= 255;
        gammatable[i] = x;
        //Serial.println(gammatable[i]);
    }

    setColor(0, 255, 0);

    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);
    strip.show();
}

void lum_adjust(int16_t addr, float amt)
{
  lum[addr] += amt;
  if (lum[addr] > 1.00) {
    lum[addr] = 1.00;
  } else if (lum[addr] < 0.0) {
    lum[addr] = 0.0;
  }
}

/* Given a strip and a local address, get the global address */
int16_t loc2global(int8_t strip, int8_t loc)
{
  return stripAddrs[strip] + loc;
}

/* Given the luminance map, apply the static, color color
 * to all the pixels in the strip. */
void renderFromLuminance(void)
{
    int16_t i;

    for (i = 0; i < strip.numPixels(); i++) {
      uint8_t r, g, b;

      r = lum[i] * pColorRGB[0];
      g = lum[i] * pColorRGB[1];
      b = lum[i] * pColorRGB[2];

      r = gammatable[r];
      g = gammatable[g];
      b = gammatable[b];

      strip.setPixelColor(i, r, g, b);
    }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for(uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void solidColor(uint32_t c)
{
    uint16_t i;
    for(i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t wheel(byte wheelPos)
{
    if(wheelPos < 85) {
        return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    } else if(wheelPos < 170) {
        wheelPos -= 85;
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
    } else {
        wheelPos -= 170;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
    }
}

/** Set the chosen color to the one specified */
void setColor(uint8_t r, uint8_t g, uint8_t b)
{
    pColorRGB[0] = r;
    pColorRGB[1] = g;
    pColorRGB[2] = b;
    pcolor = strip.Color(r, g, b);
    // pixel.setPixelColor(0, pcolor);
    // pixel.show();
}

void setColor(uint32_t color)
{
    uint8_t b = (color & 0xff);
    uint8_t g = (color >> 8) & 0xff;
    uint8_t r = (color >> 16) & 0xff;
    setColor(r, g, b);
}



bool mode_rain(void)
{
    struct raindrop {
        uint8_t strand;
        int16_t pos;
    };

    int i, s;
    const int count = 20;
    static struct raindrop drop[count];
    static bool initialized = false;

    // Set each rain drop at the starting gate.
    // Signify by a position of -1
    if(!initialized) {
        for(i = 0; i < count; i++) {
            drop[i].pos = -1;
        }
        initialized = true;
    }

    // Start with all LEDs off
    solidColor(0);

    // Loop for each raindrop
    for(i = 0; i < count; i++) {
        // Visible raindrops
        if(drop[i].pos >= 0) {
            /* FIXME: Catch drops at the end of their strand, not at the end of the strip */
            strip.setPixelColor(drop[i].pos, pcolor);
            drop[i].pos += 1;
            if(drop[i].pos > strip.numPixels()) {
                drop[i].pos = -1;
            }
        } else {
            // Non-visible raindrops:
            if(random(40) == 0) {
                drop[i].strand = random(nStrips);
                drop[i].pos = stripAddrs[drop[i].strand];
            }
        }
    }

    return false;
}

// Rainbow Cycle Program - Equally distributed
bool mode_rainbowCycle(void)
{
    static uint16_t frame = 0;
    uint16_t i, j;

    for(i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, wheel(((i * 256 / strip.numPixels()) + frame) % 256));
    }

    /* fixme: is this a full cycle for this mode? */
    frame = (frame + 1) % (256 * 5);
    return false;
}

bool mode_blink(void)
{
    static unsigned long t1;
    unsigned long t2;
    static int blink_ms = 250;
    static bool state;

    t2 = millis();
    if((t2 - t1) >= blink_ms) {
        state = !state;
        t1 = t2;
    }

    if(state) {
        solidColor(pcolor);
    } else {
        solidColor(0);
    }

    return false;
}

bool mode_rotate(void)
{
    static unsigned long t1;
    unsigned long t2;
    static int rotate_ms = 250;
    static uint8_t s;
    int i;

    t2 = millis();
    if((t2 - t1) >= rotate_ms) {
        s = (s + 1) % nStrips;
        t1 = t2;
    }

    solidColor(0);
    for(i = 0; i < geometry[s]; i++) {
        strip.setPixelColor(stripAddrs[s] + i, pcolor);
    }
    return false;
}

bool mode_void_rotate(void)
{
    static unsigned long t1;
    unsigned long t2;
    static int rotate_ms = 250;
    static uint8_t s;
    int i;

    t2 = millis();
    if((t2 - t1) >= rotate_ms) {
        s = (s + 1) % nStrips;
        t1 = t2;
    }

    solidColor(pcolor);
    for(i = 0; i < geometry[s]; i++) {
        strip.setPixelColor(stripAddrs[s] + i, 0);
    }

    return false;
}

/**
 * sparkles: creates a number of sparkles that appear and fade out.
 * @nSparkles: maximum number of concurrent sparkes
 * @pSparkle:    Odds that a new sparkle will be created.
 *               If p = 40 and frame rate is 20ms,
 *               40 * 20ms --> 800ms;
 *               A sparkle can be expected every 800ms.
 * @sparkleLife: maximum life of a sparkle, in frames.
 *               If the frame rate is 20, a sparkle life of 100 is 2000ms,
 *               2 seconds.
 */
bool mode_sparkles(void) {
    struct sparkle {
        int8_t strand;
        int8_t strandPos;
        int16_t pos;
        float brightness;
        bool visible;
        unsigned long tick;
    };
    /* Animation parameters */
    static const int nSparkles = 4;
    static const int pSparkle = 40;
    static const int sparkleLife = 100;
    /* state */
    static struct sparkle spark[nSparkles];
    static unsigned long tick;
    int i, j, k;

    /* Start with no color, and no brightness */
    solidColor(0);
    for (i = 0; i < strip.numPixels(); i++) {
      lum[i] = 0.0;
    }

    /* Iterate over the sparkles */
    for (i = 0; i < nSparkles; i++) {
        if (spark[i].visible) {
            int frame = tick - spark[i].tick;
            float decay = ((exp(2 * ((float)frame / (float)sparkleLife)) - 1) / exp(2));
            float ell = spark[i].brightness - decay;

            /* Sparkle is extinguished: */
            if (ell <= 0.0) {
                ell = 0.0;
                spark[i].visible = false;
                continue;
            }

            /* For all pixels on this sparkle's strand, compute brightness; with k=0 as the epicenter: */
            for (j = 0; j < geometry[spark[i].strand]; j++) {
              float fade_lum;
              k = abs(j - spark[i].strandPos);
              fade_lum = ell / exp(k);
              lum_adjust(loc2global(spark[i].strand, j), fade_lum);
            }
        } else if (random(pSparkle) == 0) {
            /* Create a new sparkle: */
            spark[i].visible = true;
            spark[i].tick = tick;
            /* Brightness is somewhere from 0.75 to 1.00 */
            spark[i].brightness = 1.00 - (((float)random(100) / 100.0) * 0.25);
            spark[i].strand = random(nStrips);
            spark[i].strandPos = random(geometry[spark[i].strand]);
            spark[i].pos = stripAddrs[spark[i].strand] + spark[i].strandPos;
        }
    }

    renderFromLuminance();
    tick += 1;
    return false;
}

bool mode_paparazzi(void)
{
    /* time in ms for a light to fade to 1% */
    static uint16_t life = 1000;
    static uint16_t lifeFrames = life / framerate;
    static float decay = pow(100.0, 1.0 / (float)lifeFrames);
    static float decayCoefficient = 1.0 / decay;

    int16_t i;
    int16_t addr = random(strip.numPixels());

    /* Buzzap! */
    lum[addr] = 1.00;

    /* Decay remaining sparks */
    for (i = 0; i < strip.numPixels(); i++) {
        if (lum[i]) {
            lum[i] = lum[i] * decayCoefficient;
            if (lum[i] <= 0.01) {
                lum[i] = 0.00;
            }
        }
    }

    renderFromLuminance();
    return false;
}

void loop(void)
{
    static uint32_t last_switch = 0;
    static uint8_t mode = 0;

    switch(mode) {
        case 0: mode_rain(); break;
        case 1: mode_void_rotate(); break;
        case 2: mode_rotate(); break;
        case 3: mode_blink(); break;
        case 4: mode_rainbowCycle(); break;
        case 5: mode_sparkles(); break;
        // rainbow(20);
    }
    strip.show();

    uint32_t now = millis();
    if(now - last_switch > 10000) {
        last_switch = now;
        mode++;
        if(mode > 5) {
            mode = 0;
        }
        Serial.print("mode: ");
        Serial.println(mode);
    }

    // static uint32_t t1 = 0;
    // uint32_t t2 = millis();
    // uint32_t dx = t2 - t1;
    // t1 = t2;
    // if(dx < framerate) {
    //     // delay(framerate - dx);
    // }
}

#if 0

// TODO: implement light sense with FastLED

// read the onboard lightsensor
// returns value between 0 and 1023 read from the light sensor
// NOTE: 1000 Lux will roughly read as 2 Volts (or about 680 as a raw analog
// reading). A reading of about 300 is common for most indoor light levels.
// Note that outdoor daylight is 10,000 Lux or even higher, so this sensor is
// best suited for indoor light levels!
static uint16_t lightSensor(void)
{
    return analogRead(CPLAY_LIGHTSENSOR);
}

// Configuration to tune the color sensing logic: Amount of time (in millis) to
// wait between changing the pixel color and reading the light sensor.
#define LIGHT_SETTLE_MS 100

static void senseColor(uint8_t &red, uint8_t &green, uint8_t &blue)
{
    // Save the current pixel brightness so it can later be restored.  Then bump
    // the brightness to max to make sure the LED is as bright as possible for
    // the color readings.
    uint8_t old_brightness = strip.getBrightness();
    strip.setBrightness(255);
    // Set pixel 1 (next to the light sensor) to full red, green, blue
    // color and grab a light sensor reading.  Make sure to wait a bit
    // after changing pixel colors to let the light sensor change
    // resistance!
    setPixelColor(1, 255, 0, 0); // Red
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_red = lightSensor();
    setPixelColor(1, 0, 255, 0); // Green
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_green = lightSensor();
    setPixelColor(1, 0, 0, 255); // Blue
    delay(LIGHT_SETTLE_MS);
    uint16_t raw_blue = lightSensor();
    // Turn off the pixel and restore brightness, we're done with readings.
    setPixelColor(1, 0);
    strip.setBrightness(old_brightness);
    // Now scale down each of the raw readings to be within
    // 0 to 255.  Remember each sensor reading is from the ADC
    // which has 10 bits of resolution (0 to 1023), so dividing
    // by 4 will change the range from 0-1023 to 0-255.  Also
    // use the min function to clamp the value to 255 at most (just
    // to prevent overflow from 255.xx to 0).
    red = min(255, raw_red / 4);
    green = min(255, raw_green / 4);
    blue = min(255, raw_blue / 4);
}

// perform a color sense
static void testLightSense(void)
{
    // turn off all the pixels to make sure they don't interfere with reading
    clearPixels();
    // Now take a color reading
    uint8_t red, green, blue;
    senseColor(red, green, blue);
    Serial.print("Color: red=");
    Serial.print(red, DEC);
    Serial.print(" green=");
    Serial.print(green, DEC);
    Serial.print(" blue=");
    Serial.println(blue, DEC);
}
#endif

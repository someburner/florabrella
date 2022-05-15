#include <Arduino.h>
#include "CPlayExpress.h"
// #include <Adafruit_NeoPixel_ZeroDMA.h>
#include <Adafruit_NeoPixel.h>

#include "LIS3DH.h"

#define DEFAULT_BRIGHTNESS_ONBOARD 32
#define FRAMES_PER_SECOND  120
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

// Number of pixel on each stripe.
#define N_PIXEL 19*8
// Number of stripes controlled by one controller.
#define N_STRIPE 1
// Number of drops parallel for the raindrop animation.
#define N_DROP 4

// The controller offset is used for the spiral and lantern animation. The animation uses an offset of illuminated time for
// the different stripes. If multiple controllers are used for one object, an offset between the controllers can be set by
// "CONTROLLER_OFFSET". The offset is added to the stripe number (e.g. if you use 1 controller for 2 stripes, set the offset
// to 0 for the first controller, to 2 for the second controller, to 4 for the third controller and so on).
#define CONTROLLER_OFFSET 0
//
#define N_SPIRAL_PIXEL 5
#define SPIRAL_DISTANCE 10
#define SPIRAL_ANIMATION_LENGTH 20

// Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
Adafruit_NeoPixel led_stripe[] {
  Adafruit_NeoPixel(N_PIXEL, A2, NEO_GRB + NEO_KHZ800),
  // Adafruit_NeoPixel(N_PIXEL, 5, NEO_GRB + NEO_KHZ800),
  // Adafruit_NeoPixel(N_PIXEL, 8, NEO_GRB + NEO_KHZ800),
};



class Color {
  public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

// Declare function here while it is used in the class "Drop".
void set_drop(Color* pixel_buffer, int drop_position, uint8_t drop_length, Color color);

// Inititate the mode while it is needed by the class "Drop".
uint8_t mode = 0;

class Drop {
  public:
    uint8_t position;
    uint8_t length;
    Color color;
    uint16_t delay;
    uint16_t delay_counter;
    Color *pixel_buffer;

    Drop()
      : position(0)
      , length(0)
      , color( {
      0xFF, 0xFF, 0xFF
    })
    , delay(0)
    , delay_counter(0)
    , pixel_buffer(0)
    {}

    Drop(Color *pb) {
      // Hand this drop a pointer for the pixel buffer of the stripe the drop is on.
      pixel_buffer = pb;
      random_init();
    }

    void random_init() {
      delay_counter = random(0, 15);
      delay = random(1, 3);
      length = random(2, 5);
      position = 0;
      if (mode == 0) {
        // If the mode is 0 the raindrops will be in colors.
        uint8_t switch_color = random(0, 7);
        switch (switch_color) {
          case 1:
            color.r = 200;
            color.g = 0;
            color.b = 0;
            break;
          case 2:
            color.r = 200;
            color.g = 200;
            color.b = 0;
            break;
          case 3:
            color.r = 200;
            color.g = 0;
            color.b = 200;
            break;
          case 4:
            color.r = 0;
            color.g = 200;
            color.b = 0;
            break;
          case 5:
            color.r = 0;
            color.g = 200;
            color.b = 200;
            break;
          case 6:
            color.r = 0;
            color.g = 0;
            color.b = 200;
            break;
        }
      }
      else {
        // If the mode is not 0 (mode 1) the raindrops will be white.
        color.r = 200;
        color.g = 200;
        color.b = 200;
      }
    }



    void update_drop() {
      // If the delay counter is on, reduce it by one, otherwise update the drop position.
      if (delay_counter != 0) {
        delay_counter--;
        return;
      }
      position++;
      delay_counter = delay;
      // Initiate the drop again if it not on the LED stripe anymore
      if (position > N_PIXEL + length) {
        random_init();
      }
    }

    void paint() {
      set_drop(pixel_buffer, position, length, color);
    }
};

// Initiate an array for the drops for every stripe.
Drop drop[N_STRIPE][N_DROP];
// Initiate the pixel buffer, in which every pixel for every stripe is stored.
Color pixel_buffer[N_STRIPE][N_PIXEL];

// Initiate the frame counter for the spiral and stroboscope effect.
uint16_t frame_counter = 0;
// Initiate the animation step for the spiral effect.
uint8_t animation_counter = 0;

void setup(void)
{
    delay(2000);

    Serial.begin(115200);
    Serial.println("hello");

    // strip.begin();
    // strip.setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);
    // strip.show();

    for (uint16_t i_stripe = 0; i_stripe < N_STRIPE; i_stripe++) {
        led_stripe[i_stripe].begin();
        led_stripe[i_stripe].setBrightness(DEFAULT_BRIGHTNESS_ONBOARD);
        // Initiate the drops for this stripe.
        for (uint16_t i_drop = 0; i_drop < N_DROP; i_drop++) {
            drop[i_stripe][i_drop] = Drop((Color*)&pixel_buffer[i_stripe]);
        }
    }
}


// ################# LED RAIN ########################### (CHECKED)
void led_rain() {
  for (uint16_t i_stripe = 0; i_stripe < N_STRIPE; i_stripe++) {
    for (uint16_t i_drop = 0; i_drop < N_DROP; i_drop++) {
      drop[i_stripe][i_drop].paint();
      drop[i_stripe][i_drop].update_drop();
    }
  }
}

// ################# SET SPIRAL ###########################
void set_spiral(uint16_t this_stripe, uint16_t this_point_position) {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  if (mode == 2) {
    // In mode 2 the spiral effect will be in rainbow colors.

    // The rainbow color spectrum is obtained in 5 different sectors, each with its own RGB function to determin the
    // right RGB values. Thus the stripe is split into 5 sectors.
    int stripe_sector = this_point_position * 5 / N_PIXEL;
    // Calculate where within one sector the point lies.
    int sector_position = (this_point_position + 1) % (N_PIXEL / 6);

    switch (stripe_sector) {
      case 0:
        red = 255;
        green = sector_position * 255 / N_PIXEL * 5;
        blue = 0;
        break;
      case 1:
        red = 255 - sector_position * 200 / N_PIXEL;
        green = 255;
        blue = 0;
        break;
      case 2:
        red = 0;
        green = 255;
        blue = sector_position * 200 / N_PIXEL * 5;
        break;
      case 3:
        red = 0;
        green = 255 - sector_position * 200 / N_PIXEL;
        blue = 255;
      case 4:
        red = sector_position * 200 / N_PIXEL * 5;
        green = 0;
        blue = 255;
        break;
      //case 5:
      //  red = 255;
      //  green = 0;
      //  blue = 255;
      //  break;
      default:
        red = 0xff;
        green = 0xff;
        blue = 0xff;
    }
  }
  else {
    // If the current mode is not 2, the spiral will be given out in the color red.
    red = 0xff;
    green = 0x00;
    blue = 0x00;
  }
  // Set the spiral animation as a drop with varrying length
  set_drop(pixel_buffer[this_stripe], this_point_position, 3 -
    cos((animation_counter % SPIRAL_ANIMATION_LENGTH) * 2 * PI / SPIRAL_ANIMATION_LENGTH) * 2, {red, green , blue});
}

// ################# LED SPIRAL ###########################
void led_spiral() {
  for (uint16_t i_stripe = 0; i_stripe < N_STRIPE; i_stripe++) {
    // The bouncy effect of the spiral is realized by only showing it 1/4 of the time with an offset between stripes
    // and, if used and set up, different controllers.
    if (!((frame_counter + i_stripe + CONTROLLER_OFFSET) % 4)) {
      // Set up the spiral effect points to occur with an offset defined by "SPIRAL_DISTANCE" on one stripe. Each point
      // will expand to the top and collapse again.
      for (uint16_t i_point = 0; i_point < N_PIXEL / SPIRAL_DISTANCE; i_point++) {
        // The effect of a downwards spiral is realized by adding the frame counter to the start pixel of the effect.
        set_spiral(i_stripe, (frame_counter + i_point * SPIRAL_DISTANCE) % N_PIXEL);
      }
    }
  }
  // Update the animation step.
  animation_counter++;
  // If end of animation is reached, the frame counter is updated.
  if (!(animation_counter % (SPIRAL_ANIMATION_LENGTH - 1))) {
    frame_counter++;
    // Reset the animation counter for a new animation.
    animation_counter = 0;
  }
}

// ################# SET PIXEL BUFFER ########################### (CHECKED)
void set_pixel_buffer(Color* pixel_buffer, uint16_t this_stripe) {
  for (uint16_t i_pixel = 0; i_pixel < N_PIXEL; i_pixel++) {
    // Prevent the pixel ilumination value to exceed the max 255.
    pixel_buffer[i_pixel].r = min(pixel_buffer[i_pixel].r, 255);
    pixel_buffer[i_pixel].g = min(pixel_buffer[i_pixel].g, 255);
    pixel_buffer[i_pixel].b = min(pixel_buffer[i_pixel].b, 255);
    // Set the pixel on this stripe.
    led_stripe[this_stripe].setPixelColor(i_pixel, led_stripe[this_stripe].Color(pixel_buffer[i_pixel].r,
      pixel_buffer[i_pixel].g, pixel_buffer[i_pixel].b));
    // After the pixel has been set the buffer is set back to zero.
    pixel_buffer[i_pixel].r = 0;
    pixel_buffer[i_pixel].g = 0;
    pixel_buffer[i_pixel].b = 0;
  }
}

void loop(void)
{
    led_rain();
    // led_spiral();

    // Update the LED pixel buffer.
    for (uint16_t i_stripe = 0; i_stripe < N_STRIPE; i_stripe++) {
      set_pixel_buffer(pixel_buffer[i_stripe], i_stripe);
    }

    // Write the pixel buffer to the LEDs.
    for (uint16_t i_stripe = 0; i_stripe < N_STRIPE; i_stripe++) {
      led_stripe[i_stripe].show();
    }

    delay(10);
}

// ################# SET DROP ########################### (CHECKED)
void set_drop(Color* pixel_buffer, int drop_position, uint8_t drop_length, Color color) {
  // Here a drop is set. The drop effect is realized by reducing the brightness from the lowest pixel on to the top.
  // Special cases for when the drop exceeds the LED stripe to the top or bottom have to be consindered.

  // First, the visible part on the stripe is defined.
  // The top pixel of the drop that can be seen (If all are seen, this is 0)
  uint8_t dropindex_top;
  // The bottom pixel of the drop that can be seen (If all are seen, this is the drop length)
  uint8_t dropindex_bot;
  if (drop_position < drop_length) {
    // Case: The drop exceeds the LED stripe at the top, thus only the visible pixels at the drop bottom have to be shown.
    dropindex_top = 0;
    dropindex_bot = drop_position;
  }
  else if (drop_position > N_PIXEL) {
    // Case: The drop exceeds the LED stripe at the bottom, thus only the upper drop pixels are visible.
    dropindex_top = drop_position - N_PIXEL;
    dropindex_bot = drop_length;
  }
  else {
    // Case: The whole drop can be displayed.
    dropindex_top = 0;
    dropindex_bot = drop_length;
  }
  // Go through every drop pixel and adjust the brightness. The bottom pixel will have full brightness.
  for (uint16_t i_droppixel = dropindex_top; i_droppixel < dropindex_bot; i_droppixel++) {
    float brightness_multiplier = 0;
    // If bottom pixel set to full brightness.
    if (i_droppixel == 0) {
      brightness_multiplier = 1;
    }
    // If not the bottom pixel lower brightness, the more up the pixel the lower the brightness.
    else {
      brightness_multiplier = 0.8 / sq(i_droppixel) / drop_length;
    }

    uint8_t R_adjust = color.r * brightness_multiplier;
    uint8_t G_adjust = color.g * brightness_multiplier;
    uint8_t B_adjust = color.b * brightness_multiplier;
    // Update the pixel buffer.
    pixel_buffer[drop_position - i_droppixel - 1].r += R_adjust;
    pixel_buffer[drop_position - i_droppixel - 1].g += G_adjust;
    pixel_buffer[drop_position - i_droppixel - 1].b += B_adjust;
  }
  // strip.show();
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

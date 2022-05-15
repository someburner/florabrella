#include <Arduino.h>
#include "CPlayExpress.h"
#include "FastLED.h"

#define DATA_PIN    A2
#define LED_TYPE    WS2812B
#define COLOR_ORDER RGB
#define NUM_LEDS    19
#define DEFAULT_BRIGHTNESS 32
CRGB leds[NUM_LEDS];

void setup(void)
{
    delay(2000);

    Serial.begin(115200);
    Serial.println("hello");

    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);
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


void loop(void)
{
    color_chase(CRGB::White, 200);
}

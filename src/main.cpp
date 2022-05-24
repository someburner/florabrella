#include <Arduino.h>
#include "CPlayExpress.h"
#include "LIS3DH.h"
#include "LEDconfig.h"
#include "FastLED.h"
#include "gradients.h"
#include "LUT.h"
#include "Config.h"

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Effects
#include "AccelSparkle.h"
#include "Bloom.h"
#include "DropDownFade.h"
#include "EdgeRotate.h"
#include "EdgeLoop.h"
#include "PingPong.h"
#include "GradientTest.h"
#include "TopBottom.h"
#include "MeteorRain.h"
#include "Strobe.h"

#ifdef USE_NEOPIXEL_DMA
#include <Adafruit_NeoPixel_ZeroDMA.h>
#endif

#ifdef USE_ONBOARD_BTN
    #define BTN_PIN CPLAY_RIGHTBUTTON
#else
    #define BTN_PIN CPLAY_EXT_BTN_PIN
#endif

CRGB leds[NUM_LEDS];

LIS3DH accel;

#ifdef USE_NEOPIXEL_DMA
Adafruit_NeoPixel_ZeroDMA strip(CPLAY_NUMPIXELS, A2, NEO_GRB);
#endif

static bool _isRunning = false;
static bool _cycleAll = false;

static bool isRunning(void) {
    if(_cycleAll) {
        EVERY_N_SECONDS(30) {
            _isRunning = false;
        }
    }
    return _isRunning;
}

#ifdef USE_BTN_EFFECT_CYCLE
// declare effect methods
void run_cycle_all(void);
void run_meteorrain(void);
void run_meteorrain0(void);
void run_meteorrain1(void);
void run_accel_sparkle(void);
void run_bloom(void);
void run_dropdownfade(void);
void run_edgerotate(void);
void run_edgeloop(void);
void run_pingpong(void);
void run_gradienttests(void);
void run_gradienttest0(void);
void run_gradienttest1(void);
void run_gradienttest2(void);
void run_gradienttest3(void);
void run_gradienttest4(void);
void run_gradienttest5(void);
void run_gradienttest6(void);
void run_gradienttest7(void);
void run_topbottomanims(void);
void run_topbottomanim0(void);
void run_topbottomanim1(void);
void run_topbottomanim2(void);
void run_topbottomanim3(void);
void run_topbottomanim4(void);
void run_topbottomanim5(void);
void run_topbottomanim6(void);
void run_topbottomanim7(void);
void run_topbottomanim8(void);
void run_strobe(void);

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
    run_cycle_all,
    run_meteorrain0,
    run_meteorrain0,
    run_meteorrain1,
    run_accel_sparkle,
    run_bloom,
    run_dropdownfade,
    run_edgerotate,
    run_edgeloop,
    run_pingpong,
    run_gradienttest0,
    run_gradienttest1,
    run_gradienttest2,
    run_gradienttest3,
    run_gradienttest4,
    run_gradienttest5,
    run_gradienttest6,
    run_gradienttest7,
    run_topbottomanim0,
    run_topbottomanim1,
    run_topbottomanim2,
    run_topbottomanim3,
    run_topbottomanim4,
    run_topbottomanim5,
    run_topbottomanim6,
    run_topbottomanim7,
    run_topbottomanim8,
    run_strobe
};

// External Button variables
#define BTN_DEBOUNCE_MS 100UL
static uint32_t lastBtnMillis = 0;

void ext_btn_isr(void)
{
    uint32_t now = millis();
    if((now - lastBtnMillis) > BTN_DEBOUNCE_MS) {
        lastBtnMillis = now;
        _isRunning = false; // TODO: unsafe
        _cycleAll = false;
        DEBUGln("btn!");
    }
}
#endif // USE_BTN_EFFECT_CYCLE

void setup(void)
{
    delay(500);

    DEBUGbegin(SERIAL_BAUD);
    DEBUGln("hello");

    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.clear(true);
    FastLED.setBrightness(DEFAULT_BRIGHTNESS);

    DEBUG("LIS3DH init: ");
    accel = LIS3DH(&Wire1); // i2c on wire1
    if(accel.begin(CPLAY_LIS3DH_ADDRESS)) {
        DEBUGln("success");
    } else {
        DEBUGln("fail");
    }

#ifdef USE_BTN_EFFECT_CYCLE
#if BTN_PIN == CPLAY_EXT_BTN_PIN
    pinMode(BTN_PIN, INPUT_PULLUP);
    attachInterrupt(BTN_PIN, ext_btn_isr, FALLING);
#else // BTN_PIN != CPLAY_EXT_BTN_PIN
    pinMode(BTN_PIN, INPUT_PULLDOWN);
    attachInterrupt(BTN_PIN, ext_btn_isr, RISING);
#endif // BTN_PIN == CPLAY_EXT_BTN_PIN
#endif // USE_BTN_EFFECT_CYCLE

#ifdef USE_NEOPIXEL_DMA
    strip.begin();
    strip.setBrightness(DEFAULT_BRIGHTNESS);
#endif
}

void run_meteorrain(void)
{
    _isRunning = true;
    // MeteorRain mr = MeteorRain();
    // MeteorRain mr = MeteorRain(CRGB::Blue, false);
    MeteorRain mr = MeteorRain(CRGB::Black, true);
    while(isRunning()) mr.run();
}

void run_meteorrain0(void)
{
    _isRunning = true;
    // MeteorRain mr = MeteorRain();
    // MeteorRain mr = MeteorRain(CRGB::Blue, false);
    MeteorRain mr = MeteorRain(CRGB::Black, true);
    while(isRunning()) mr.run();
}

void run_meteorrain1(void)
{
    _isRunning = true;
    // MeteorRain mr = MeteorRain();
    // MeteorRain mr = MeteorRain(CRGB::Blue, false);
    MeteorRain mr = MeteorRain(CRGB::Black, true);
    mr.SetLightning(true);
    while(isRunning()) mr.run();
}


// cool
void run_bloom(void)
{
    _isRunning = true;
    Bloom bloom = Bloom();
    while(isRunning()) bloom.run();
}

// nice but needs improvement
void run_dropdownfade(void)
{
    _isRunning = true;
    DropDownFade ddf = DropDownFade();
    while(isRunning()) ddf.run();
}

// a bit hard on the eyes but demos it
void run_edgerotate(void)
{
    _isRunning = true;
    EdgeRotate er = EdgeRotate();
    while(isRunning()) er.run();
}

// kinda cool but needs variation
void run_edgeloop(void)
{
    _isRunning = true;
    EdgeLoop el = EdgeLoop();
    while(isRunning()) el.run();
}

// also kinda cool but also needs variation
void run_pingpong(void)
{
    _isRunning = true;
    PingPong pp = PingPong();
    while(isRunning()) pp.run();
}

// ------------------------------ gradienttests ----------------------------- //
void run_gradienttests(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest();
    while(isRunning()) gt.run();
}

void run_gradienttest0(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(0);
    while(isRunning()) gt.run();
}

void run_gradienttest1(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(1);
    while(isRunning()) gt.run();
}

void run_gradienttest2(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(2);
    while(isRunning()) gt.run();
}

void run_gradienttest3(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(3);
    while(isRunning()) gt.run();
}

void run_gradienttest4(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(4);
    while(isRunning()) gt.run();
}

void run_gradienttest5(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(5);
    while(isRunning()) gt.run();
}

void run_gradienttest6(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(6);
    while(isRunning()) gt.run();
}

void run_gradienttest7(void)
{
    _isRunning = true;
    GradientTest gt = GradientTest(7);
    while(isRunning()) gt.run();
}

// ------------------------------ topbottomanims ---------------------------- //
// kiwiholmberg - some good ones in here
void run_topbottomanims(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(); // all
    // TopBottom tb = TopBottom(7); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim0(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(0); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim1(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(1); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim2(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(2); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim3(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(3); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim4(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(4); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim5(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(5); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim6(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(6); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim7(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(7); // specific
    while(isRunning()) tb.run();
}

void run_topbottomanim8(void)
{
    _isRunning = true;
    TopBottom tb = TopBottom(8); // specific
    while(isRunning()) tb.run();
}
// -------------------------------------------------------------------------- //

void run_strobe(void)
{
    _isRunning = true;
    Strobe s = Strobe(CRGB::White, 10, 50);
    while(isRunning()) s.run();
}

void run_accel_sparkle(void)
{
    _isRunning = true;
    AccelSparkle as = AccelSparkle(CRGB::White);
    while(isRunning()) as.run();
}

void run_cycle_all(void)
{
    _isRunning = true;
    _cycleAll = true;
    for(int i = 1; i < ARRAY_SIZE(gPatterns) - 1; i++) {
        gPatterns[i]();
    }
    _cycleAll = false;
}

void loop(void)
{
#ifdef USE_BTN_EFFECT_CYCLE
    static uint8_t nextPatternNum = 0;

    if(nextPatternNum == 0) {
        _cycleAll = true;
    } else {
        _cycleAll = false;
    }

    // Call the current pattern function once, updating the 'leds' array
    gPatterns[nextPatternNum]();
    // if we returned, ext button stopped the effect
    nextPatternNum = (nextPatternNum + 1) % ARRAY_SIZE(gPatterns);

#else
    // run_dropdownfade();
    // run_pingpong();
    // run_edgeloop();
    // run_edgerotate();
    // run_gradienttests();
    // run_bloom();
    // run_topbottomanims();
    // run_meteorrain();
    // run_strobe();
    run_accel_sparkle();
#endif
}

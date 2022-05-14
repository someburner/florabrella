#ifndef CPLAY_EXPRESS_H
#define CPLAY_EXPRESS_H

// samd21 definitions from:
// https://github.com/adafruit/Adafruit_CircuitPlayground/blob/master/Adafruit_Circuit_Playground.h

#define CPLAY_LEFTBUTTON 4        ///< left button pin
#define CPLAY_RIGHTBUTTON 5       ///< right button pin
#define CPLAY_SLIDESWITCHPIN 7    ///< slide switch pin
#define CPLAY_NEOPIXELPIN A1       ///< neopixel pin
#define CPLAY_NUMPIXELS (19*8)     ///< onboard neopixel led count
#define CPLAY_REDLED 13           ///< red led pin
#define CPLAY_IR_EMITTER 25       ///< IR emmitter pin
#define CPLAY_IR_RECEIVER 26      ///< IR receiver pin
#define CPLAY_BUZZER A0           ///< buzzer pin
#define CPLAY_LIGHTSENSOR A8      ///< light sensor pin
#define CPLAY_THERMISTORPIN A9    ///< thermistor pin
#define CPLAY_SOUNDSENSOR A4      ///< TBD I2S
#define CPLAY_LIS3DH_CS -1        ///< LIS3DH chip select pin
#define CPLAY_LIS3DH_INTERRUPT 27 ///< LIS3DH interrupt pin
#define CPLAY_LIS3DH_ADDRESS 0x19 ///< LIS3DH I2C address

#endif

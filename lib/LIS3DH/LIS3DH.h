/*!
 *  @file LIS3DH.h
 *
 *  This is a library for the Adafruit LIS3DH Accel breakout board
 *
 *  Designed specifically to work with the Adafruit LIS3DH Triple-Axis
 *Accelerometer
 *	(+-2g/4g/8g/16g)
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/2809
 *
 *	This sensor communicates over I2C or SPI (our library code supports
 *both) so you can share it with a bunch of other sensors on the same I2C bus.
 *  There's an address selection pin so you can have two accelerometers share an
 *I2C bus.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  K. Townsend / Limor Fried (Ladyada) - (Adafruit Industries).
 *
 *  BSD license, all text above must be included in any redistribution
 */

#ifndef _LIS3DH_H
#define _LIS3DH_H

#include "Arduino.h"

#include <SPI.h>
#include <Wire.h>

#include "Sensor.h"

/*!
 *  @brief  Class that stores state and functions for interacting with
 *          Adafruit_LIS3DH
 */
class LIS3DH : public Adafruit_Sensor {
public:
  LIS3DH(TwoWire *Wi = &Wire);
  LIS3DH(int8_t cspin, SPIClass *theSPI = &SPI);
  LIS3DH(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin);

  bool begin(uint8_t addr, uint8_t nWAI = 0x33);

  uint8_t getDeviceID();
  bool haveNewData();

  void read();
  int16_t readADC(uint8_t a);

  void setRange(uint8_t range);
  uint8_t getRange(void);

  void setDataRate(uint8_t dataRate);
  uint8_t getDataRate(void);

  bool getEvent(sensors_event_t *event);
  void getSensor(sensor_t *sensor);

  void setClick(uint8_t c, uint8_t clickthresh, uint8_t timelimit = 10,
                uint8_t timelatency = 20, uint8_t timewindow = 255);
  uint8_t getClick(void);

  void intConf(uint8_t moveType, uint8_t threshold, uint8_t timeDur, bool polarity);

  int16_t x; /**< x axis value */
  int16_t y; /**< y axis value */
  int16_t z; /**< z axis value */

  float x_g; /**< x_g axis value (calculated by selected range) */
  float y_g; /**< y_g axis value (calculated by selected range) */
  float z_g; /**< z_g axis value (calculated by selected scale) */

protected:
  uint8_t spixfer(uint8_t x = 0xFF);
  void writeRegister8(uint8_t reg, uint8_t value);
  uint8_t readRegister8(uint8_t reg);

private:
  TwoWire *I2Cinterface;
  SPIClass *SPIinterface;

  uint8_t _wai;

  int8_t _cs, _mosi, _miso, _sck;

  int8_t _i2caddr;

  int32_t _sensorID;
};

#endif

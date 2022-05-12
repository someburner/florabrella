/*!
 * @file LIS3DH.cpp
 *
 *
 *  This is a library for the Adafruit LIS3DH Accel breakout board
 *
 *  Designed specifically to work with the Adafruit LIS3DH Accel breakout board.
 *
 *  Pick one up today in the adafruit shop!
 *  ------> https://www.adafruit.com/product/2809
 *
 *  This sensor communicates over I2C or SPI (our library code supports both) so
 * you can share it with a bunch of other sensors on the same I2C bus.
 *
 *  Adafruit invests time and resources providing this open source code,
 *  please support Adafruit andopen-source hardware by purchasing products
 *  from Adafruit!
 *
 *  K. Townsend / Limor Fried (Adafruit Industries)
 *
 *  BSD license, all text above must be included in any redistribution
 */

#include "Arduino.h"
#include <Wire.h>

#include "LIS3DH.h"
#include "LIS3DH_REG.h"

// Instantiates a new LIS3DH class in I2C
LIS3DH::LIS3DH(TwoWire *Wi)
    : _cs(-1), _mosi(-1), _miso(-1), _sck(-1), _sensorID(-1)
{
    I2Cinterface = Wi;
}

// Instantiates a new LIS3DH class using hardware SPI
LIS3DH::LIS3DH(int8_t cspin, SPIClass *theSPI)
{
    _cs = cspin;
    _mosi = -1;
    _miso = -1;
    _sck = -1;
    _sensorID = -1;
    SPIinterface = theSPI;
}

// Instantiates a new LIS3DH class using software SPI
LIS3DH::LIS3DH(int8_t cspin, int8_t mosipin, int8_t misopin, int8_t sckpin)
{
    _cs = cspin;
    _mosi = mosipin;
    _miso = misopin;
    _sck = sckpin;
    _sensorID = -1;
}

/*!
 *  @brief  Setups the HW (reads coefficients values, etc.)
 *  @param  i2caddr
 *          i2c address (optional, fallback to default)
 *  @param  nWAI
 *          Who Am I register value - defaults to 0x33 (LIS3DH)
 *  @return true if successful
 */
bool LIS3DH::begin(uint8_t i2caddr = I2C_ADDR, uint8_t nWAI)
{
    _i2caddr = i2caddr;
    _wai = nWAI;

    if(_cs == -1) {
        // i2c
        I2Cinterface->begin();
    } else {
        digitalWrite(_cs, HIGH);
        pinMode(_cs, OUTPUT);

#ifndef __AVR_ATtiny85__
        if(_sck == -1) {
          // hardware SPI
          SPIinterface->begin();
        } else {
          // software SPI
          pinMode(_sck, OUTPUT);
          pinMode(_mosi, OUTPUT);
          pinMode(_miso, INPUT);
        }
#endif
    }

    /*
    Serial.println("Debug");
    for(uint8_t i=0; i<0x30; i++) {
        Serial.print("$");
        Serial.print(i, HEX); Serial.print(" = 0x");
        Serial.println(readRegister8(i), HEX);
    }
    */

    /* Check connection */
    uint8_t deviceid = readRegister8(WHO_AM_I);
    if(deviceid != _wai) {
        /* No LIS3DH detected ... return false */
        // Serial.println(deviceid, HEX);
        return false;
    }

    // enable z axis only, enable low power
    // uint8_t ctrl_reg1 = (1 << lis3dh_ctrl_reg1_lp_en) | (1 << lis3dh_ctrl_reg1_z_en)
    uint8_t ctrl_reg1 = (1 << lis3dh_ctrl_reg1_z_en)
    ;
    writeRegister8(CTRL_REG1, ctrl_reg1);
    // 50hz rate
    setDataRate(LIS3DH_DATARATE_50_HZ);

    // High res & BDU enabled
    // writeRegister8(CTRL_REG4, 0x88);

    // DRDY on INT1
    // writeRegister8(CTRL_REG3, 0x10);
    writeRegister8(CTRL_REG3, 0x40); // IA1

    // Turn on orientation config
    // writeRegister8(LIS3DH_REG_PL_CFG, 0x40);

    // enable adcs
    writeRegister8(TEMP_CFG_REG, 0x80);

    /*
    for(uint8_t i=0; i<0x30; i++) {
        Serial.print("$");
        Serial.print(i, HEX); Serial.print(" = 0x");
        Serial.println(readRegister8(i), HEX);
    }
    */

    return true;
}

/*!
 *  @brief  Get Device ID from WHO_AM_I
 *  @return WHO AM I value
 */
uint8_t LIS3DH::getDeviceID()
{
    return readRegister8(WHO_AM_I);
}

/*!
 *  @brief  Check to see if new data available
 *  @return true if there is new data available, false otherwise
 */
bool LIS3DH::haveNewData()
{
    // checking ZYXDA in REG_STATUS2 tells us if data available
    return (readRegister8(STATUS_REG) & 0x8) >> 3;
}

/*!
 *  @brief  Reads x y z values at once
 */
void LIS3DH::read()
{
    if(_cs == -1) {
        // i2c
        I2Cinterface->beginTransmission(_i2caddr);
        I2Cinterface->write(OUT_X_L | 0x80); // 0x80 for autoincrement
        I2Cinterface->endTransmission();

        I2Cinterface->requestFrom(_i2caddr, 6);
        x = I2Cinterface->read();
        x |= ((uint16_t)I2Cinterface->read()) << 8;
        y = I2Cinterface->read();
        y |= ((uint16_t)I2Cinterface->read()) << 8;
        z = I2Cinterface->read();
        z |= ((uint16_t)I2Cinterface->read()) << 8;
    }
#ifndef __AVR_ATtiny85__
    else {
        if(_sck == -1)
            SPIinterface->beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
        digitalWrite(_cs, LOW);
        spixfer(OUT_X_L | 0x80 | 0x40); // read multiple, bit 7&6 high

        x = spixfer();
        x |= ((uint16_t)spixfer()) << 8;
        y = spixfer();
        y |= ((uint16_t)spixfer()) << 8;
        z = spixfer();
        z |= ((uint16_t)spixfer()) << 8;

        digitalWrite(_cs, HIGH);
        if(_sck == -1)
            SPIinterface->endTransaction(); // release the SPI bus
    }
#endif
    uint8_t range = getRange();
    uint16_t divider = 1;
    if(range == LIS3DH_RANGE_16_G)
        divider = 1365; // different sensitivity at 16g
    if(range == LIS3DH_RANGE_8_G)
        divider = 4096;
    if(range == LIS3DH_RANGE_4_G)
        divider = 8190;
    if(range == LIS3DH_RANGE_2_G)
        divider = 16380;

    x_g = (float)x / divider;
    y_g = (float)y / divider;
    z_g = (float)z / divider;
}

/*!
 *  @brief  Read the auxilary ADC
 *  @param  adc
 *          adc index. possible values (1, 2, 3).
 *  @return auxilary ADC value
 */
int16_t LIS3DH::readADC(uint8_t adc)
{
    if((adc < 1) || (adc > 3))
        return 0;
    uint16_t value;

    adc--;

    uint8_t reg = OUT_ADC1_L + adc * 2;

    if(_cs == -1) {
        // i2c
        I2Cinterface->beginTransmission(_i2caddr);
        I2Cinterface->write(reg | 0x80); // 0x80 for autoincrement
        I2Cinterface->endTransmission();
        I2Cinterface->requestFrom(_i2caddr, 2);
        value = I2Cinterface->read();
        value |= ((uint16_t)I2Cinterface->read()) << 8;
    }
#ifndef __AVR_ATtiny85__
    else {
        if(_sck == -1)
            SPIinterface->beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
        digitalWrite(_cs, LOW);
        spixfer(reg | 0x80 | 0x40); // read multiple, bit 7&6 high

        value = spixfer();
        value |= ((uint16_t)spixfer()) << 8;

        digitalWrite(_cs, HIGH);
        if(_sck == -1)
            SPIinterface->endTransaction(); // release the SPI bus
    }
#endif

    return value;
}

/*!
 *   @brief  Set INT to output for single or double click
 *   @param  c
 *					 0 = turn off I1_CLICK
 *           1 = turn on all axes & singletap
 *					 2 = turn on all axes & doubletap
 *   @param  clickthresh
 *           CLICK threshold value
 *   @param  timelimit
 *           sets time limit (default 10)
 *   @param  timelatency
 *   				 sets time latency (default 20)
 *   @param  timewindow
 *   				 sets time window (default 255)
 */
void LIS3DH::setClick(uint8_t c, uint8_t clickthresh,
                               uint8_t timelimit, uint8_t timelatency,
                               uint8_t timewindow)
{
    if(!c) {
        // disable int
        uint8_t r = readRegister8(CTRL_REG3);
        r &= ~(0x80); // turn off I1_CLICK
        writeRegister8(CTRL_REG3, r);
        writeRegister8(CLICK_CFG, 0);
        return;
    }
    // else...

    writeRegister8(CTRL_REG3, 0x80); // turn on int1 click
    writeRegister8(CTRL_REG5, 0x08); // latch interrupt on int1

    if(c == 1)
        writeRegister8(CLICK_CFG, 0x15); // turn on all axes & singletap
    if(c == 2)
        writeRegister8(CLICK_CFG, 0x2A); // turn on all axes & doubletap

    writeRegister8(CLICK_THS, clickthresh);    // arbitrary
    writeRegister8(TIME_LIMIT, timelimit);     // arbitrary
    writeRegister8(TIME_LATENCY, timelatency); // arbitrary
    writeRegister8(TIME_WINDOW, timewindow);   // arbitrary
}

/*!
 *   @brief  Get uint8_t for single or double click
 *   @return register CLICK_SRC
 */
uint8_t LIS3DH::getClick()
{
    return readRegister8(CLICK_SRC);
}

/*!
 *   @brief  Sets the g range for the accelerometer
 *   @param  range
 *           range value
 */
void LIS3DH::setRange(uint8_t range)
{
    uint8_t r = readRegister8(CTRL_REG4);
    r &= ~(0x30);
    r |= range << 4;
    writeRegister8(CTRL_REG4, r);
}

/*!
 *  @brief  Gets the g range for the accelerometer
 *  @return Returns g range value
 */
uint8_t LIS3DH::getRange()
{
    /* Read the data format register to preserve bits */
    return (readRegister8(CTRL_REG4) >> 4) & 0x03;
}

/*!
 *  @brief  Sets the data rate for the LIS3DH (controls power consumption)
 *  @param  dataRate
 *          data rate value
 */
void LIS3DH::setDataRate(uint8_t dataRate)
{
    uint8_t ctl1 = readRegister8(CTRL_REG1);
    ctl1 &= ~(0xF0); // mask off bits
    ctl1 |= (dataRate << 4);
    writeRegister8(CTRL_REG1, ctl1);
}

/*!
 *   @brief  Gets the data rate for the LIS3DH (controls power consumption)
 *   @return Returns Data Rate value
 */
uint8_t LIS3DH::getDataRate()
{
    return (readRegister8(CTRL_REG1) >> 4) & 0x0F;
}

/*!
 *  @brief  Low level SPI
 *  @param  x
 *          value that will be written throught SPI
 *  @return reply
 */
uint8_t LIS3DH::spixfer(uint8_t x)
{
#ifndef __AVR_ATtiny85__
    if(_sck == -1)
        return SPIinterface->transfer(x);

    // software spi
    // Serial.println("Software SPI");
    uint8_t reply = 0;
    for(int i = 7; i >= 0; i--) {
        reply <<= 1;
        digitalWrite(_sck, LOW);
        digitalWrite(_mosi, x & (1 << i));
        digitalWrite(_sck, HIGH);
        if(digitalRead(_miso))
            reply |= 1;
    }
    return reply;
#endif
}

/*!
 *  @brief  Writes 8-bits to the specified destination register
 *  @param  reg
 *          register address
 *  @param  value
 *          value that will be written into selected register
 */
void LIS3DH::writeRegister8(uint8_t reg, uint8_t value)
{
    if(_cs == -1) {
        I2Cinterface->beginTransmission((uint8_t)_i2caddr);
        I2Cinterface->write((uint8_t)reg);
        I2Cinterface->write((uint8_t)value);
        I2Cinterface->endTransmission();
    }
#ifndef __AVR_ATtiny85__
    else {
        if(_sck == -1)
            SPIinterface->beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
        digitalWrite(_cs, LOW);
        spixfer(reg & ~0x80); // write, bit 7 low
        spixfer(value);
        digitalWrite(_cs, HIGH);
        if(_sck == -1)
            SPIinterface->endTransaction(); // release the SPI bus
    }
#endif
}

/*!
 *  @brief  Reads 8-bits from the specified register
 *  @param  reg
 *          register address
 *  @return read value
 */
uint8_t LIS3DH::readRegister8(uint8_t reg)
{
    uint8_t value;

    if(_cs == -1) {
        I2Cinterface->beginTransmission(_i2caddr);
        I2Cinterface->write((uint8_t)reg);
        I2Cinterface->endTransmission();

        I2Cinterface->requestFrom(_i2caddr, 1);
        value = I2Cinterface->read();
    }
#ifndef __AVR_ATtiny85__
    else {
        if(_sck == -1)
            SPIinterface->beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));
        digitalWrite(_cs, LOW);
        spixfer(reg | 0x80); // read, bit 7 high
        value = spixfer(0);
        digitalWrite(_cs, HIGH);
        if(_sck == -1)
            SPIinterface->endTransaction(); // release the SPI bus
    }
#endif
    return value;
}

//****************************************************************************//
//
//  Configure interrupts 1 or 2, stop or move, threshold and duration
//	Durationsteps and maximum values depend on the ODR chosen.
//
//****************************************************************************//
// moveType 0 = stop
// moveType 1 = move
void LIS3DH::intConf(uint8_t moveType, uint8_t threshold, uint8_t timeDur, bool polarity)
{
    uint8_t val = 0;  //Temporary variable

    //Build INT_CFG 0x30 or 0x34
    //Detect movement or stop
    if(moveType == 1)
       val = (1 << lis3dh_int1_cfg_zh_ie);
    else
       val = (1 << lis3dh_int1_cfg_zl_ie);

    // _DEBBUG ("LIS3DH_INT_CFG: 0x", val);
    writeRegister8(INT1_CFG, val);

    //Build INT_THS 0x32 or 0x36
    writeRegister8(INT1_THS, threshold);

    //Build INT_DURATION 0x33 or 0x37
    writeRegister8(INT1_DURATION, timeDur);

    val = 0 | (polarity << 1);

    //Attach configuration to Interrupt X
    // if(interrupt == 1)
    if(true)
    {
        writeRegister8(CTRL_REG3, 0x40);
    }
    else
    {
        // val |= 0x20;
    }

    writeRegister8(CTRL_REG6, val);

}

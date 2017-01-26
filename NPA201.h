/*
  NPA201.h - Library for interfacing with the Amphenol NPA 201 pressure sensor
  https://www.amphenol-sensors.com/en/products/pressure-mems/mems-sensors/3236-npa-201
  Created by operator@noctivore.com
*/

// Prevent weirdness from including header more than once
#ifndef NPA201_h
#define NPA201_h

// Include headers
#include "ArduinoI2C.h"

// Define address and read commands
#define ADDRESS 0x27
#define COMMAND 0xAC

// Class NPA201
class NPA201
{
  public:
    NPA201(); // Contructor
    uint8_t getStatus(); // Returns NPA 201 status
    uint16_t getRawPressure(); // Returns raw pressure data
    uint16_t getRawTemperature(); // Returns raw temperature data
    float getPressure(); // Returns calculated pressure data
    float getTemperature(); // Returns calculated temperature data
    float getAltitude(); // Returns caclulated altitude
  private:
    static I2CInterface* pI2CInterface; // Pointer to the I2CInterface implementation
    static uint8_t NPADATA[5]; // NPADATA is used to store bytes of data read from the NPA 201 sensor
    bool getData(); // Get data from the NPA 201
  	uint8_t sendNPAReadCommand(); // Sends the I2C command to read data
    void readNPAData(); // Reads bytes of data from NPA 201 memory
};

#endif
// End of Class NPA201
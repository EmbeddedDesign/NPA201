/*
  NPA201.cpp - Library for interfacing with the Amphenol NPA 201 pressure sensor
  https://www.amphenol-sensors.com/en/products/pressure-mems/mems-sensors/3236-npa-201
  Created by operator@noctivore.com
*/

// Include headers
#include "NPA201.h"

ArduinoI2C i2c;
I2CInterface* NPA201::pI2CInterface = &i2c;
uint8_t NPA201::NPADATA[5] = { };

// Constructor
NPA201::NPA201()
{

}

// Returns NPA 201 status
uint8_t NPA201::getStatus()
{
  // Check for new data from the NPA 201
  getData();
  
  return NPADATA[0];
}

// Returns raw pressure data
uint16_t NPA201::getRawPressure()
{
  // Check for new data from the NPA 201
  getData();

  // Raw pressure data
  // Bitwise OR of pressure data bytes
  // Upper byte is bitshifted left 8 positions creating bitmask for lower byte
  // p_raw is a 16-bit unsigned integer used to store raw pressure data
  uint16_t p_raw = ( (NPADATA[1] << 8) | NPADATA[2] );

  return p_raw;
}

// Returns raw temperature data
uint16_t NPA201::getRawTemperature()
{
  // Check for new data from the NPA 201
  getData();

  // Raw temperature data
  // Bitwise OR of temperature data bytes
  // Upper byte is bitshifted left 8 positions creating bitmask for lower byte
  // t_raw is a 16-bit unsigned integer used to store raw temperature data
  uint16_t t_raw = ( (NPADATA[3] << 8) | NPADATA[4] );

  return t_raw;
}

// Returns calculated pressure data
float NPA201::getPressure()
{
  // Calculate the actual pressure derived from the raw data
  // pressure is a 32-bit floating point used to store calculated pressure
  float pressure = ((float)getRawPressure()/65535*(1000)+260);

  return pressure;
}

// Returns calculated temperature data
float NPA201::getTemperature()
{
  // Calculate the actual temperature derived from the raw data
  // temperature is a 32-bit floating point used to store calculated temperature
  float temperature = ((float)getRawTemperature()/65535*(85+40)-40);
  
  return temperature;
}

// Returns caclulated altitude
float NPA201::getAltitude()
{
  // Calculate the altitude in meters derived from calculated pressure
  // altitude is a 32-bit floating point used to store calculated altitude
  float altitude = 44330*(1-pow(getPressure()/1013.25f,1/5.255f));

  return altitude;
}

// Gets data from the NPA 201 as often as its refresh rate allows
// Returns false is something goes wrong with the transmission
// We don't necessarily need do anything with this status but it could be useful for debugging
bool NPA201::getData()
{
  // If the sendNPAReadCommand returns success
  if(sendNPAReadCommand() == 0)
  {
  	// Wait 20 milliseconds
  	pI2CInterface->wait(20);
    readNPAData();
    return true;
  }

  // Otherwise, data in the NPADATA array will remain the same as the previous read
  return false;
}

// Sends the I2C command to read data
// Returns the status of the transmission
uint8_t NPA201::sendNPAReadCommand()
{
  // Begin transmission
  // The NPA 201 slave address is 0x27(HEX)
  // This address can be changed if required as per AAS-910-290
  pI2CInterface->beginTransmission(ADDRESS);
  // Send the command read full measurement data
  // 0xAC(HEX) is the command byte to read data
  // It is the only command supported by the NPA 201
  pI2CInterface->write(COMMAND);
  // End the transmission (stop transmitting)
  // return the status of the transmission
  // 0:success
  // 1:data too long to fit in transmit buffer
  // 2:received NACK on transmit of address
  // 3:received NACK on transmit of data
  // 4:other error
  return pI2CInterface->endTransmission();
}

// Reads bytes of data from NPA 201 memory
void NPA201::readNPAData()
{
  // Request 5 bytes from the NPA 201 (slave address 0x27(HEX))
  pI2CInterface->requestFrom(ADDRESS, 5);
  // While the I2C bus has bytes available
  // Wire.available() returns the number of bytes available for retrieval with Wire.read()
  while(pI2CInterface->available())
  {
    // For each byte of the NPADATA array
    for(uint8_t i = 0; i < 5; i++)
    {
      // Read the next byte available from the NPA 201 and write it to the NPADATA array
      NPADATA[i] = pI2CInterface->read();
      // The NPADATA array will contain the following data
      // NPADATA[0] = Status
      // NPADATA[1] = Sensor Bridge Pressure Data Upper Byte (15:8)
      // NPADATA[2] = Sensor Bridge Pressure Data Lower Byte (7:0)
      // NPADATA[3] = Temperature Data Upper Byte (15:8)
      // NPADATA[4] = Temperature Data Lower Byte (7:0)
    }
  }
}
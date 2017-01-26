// Include the NPA201 header
#include "NPA201.h"

// Create an instance of NAP201
NPA201 npa201;

// Run once
void setup() {
  // Start a serial connection to print data
  Serial.begin(9600);
  // Wait 1 second for good measure
  delay(1000);
}

// Run forever and ever
void loop() {
  // Get the status flag
  uint8_t npastatus = npa201.getStatus();
  // Get the raw pressure data
  uint16_t p_raw = npa201.getRawPressure();
  // Get the raw temperature data
  uint16_t t_raw = npa201.getRawTemperature();
  // Get the calculated pressure
  float pressure = npa201.getPressure();
  // Get the calculated temperature
  float temperature = npa201.getTemperature();
  // Get the calculated altitude
  float altitude = npa201.getAltitude();

  // Print a header
  Serial.println(F("--------------------"));
  // Print the value of the status register
  Serial.print(F("Status: "));
  Serial.println(npastatus,BIN);
  // Print the calculated temperature
  Serial.print(F("Temperature: "));
  Serial.println(temperature);
  // Print the calculated pressure
  Serial.print(F("Pressure: "));
  Serial.println(pressure);
  // Print the calculated altitude
  Serial.print(F("Altitude: "));
  Serial.println(altitude);
  // Print the raw temperature data
  Serial.print(F("Raw Temperature: "));
  Serial.println(t_raw);
  // Print the raw pressure data
  Serial.print(F("Raw Pressure: "));
  Serial.println(p_raw);
  // Print an empty line
  Serial.println();

  // Slow things down a bit so we can read the output
  delay(500);
}
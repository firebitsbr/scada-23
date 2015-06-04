// code for demonstration of 8-sensor interface board thermostat_reader_001.01

#include <Arduino.h>
#include <Sensirion.h>

#include <tstatreader_pin_definitions.h>
#include <mh_sarc_test.h>

using namespace mh_temperatureSensing;

void setup() {
  // start serial port
  Serial.begin(115200);
  delay(1000);
  // declare array reader
  sensirionArrayReaderClass sensirionReader(numDevices, dataPins, clockPins);
  // test setting resolution
  int srcRet = exerciseSensorSettings(Serial, &sensirionReader);
  if (srcRet != 0) 
  {
  Serial.print("Error - exerciseSensorSettings() returned ");
  Serial.println(srcRet);
  } 
  // test reading data
  srcRet = exerciseSensirionArrayReaderClass(Serial, &sensirionReader);
  if (srcRet != 0) 
  {
    Serial.print("Error - exerciseSensirionArrayReaderClass() returned "); 
    Serial.print((unsigned char) srcRet);
    Serial.print(", ");
    Serial.println((unsigned char) srcRet, HEX);
  }
} //end setup()

void loop() {
  delay(1000);
} // end loop()


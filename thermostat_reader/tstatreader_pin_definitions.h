#ifndef MH_SAC_TSTATREADER_PIN_DEFINITIONS_H_INCLUDED
#define MH_SAC_TSTATREADER_PIN_DEFINITIONS_H_INCLUDED

/*! \brief This file defines the pins and devices required for sensor interface and serial communication
 *  using the sensor interface board thermostat_reader_001
 *
 * sensor interface can be any two-wire protocol.
 * serial communication is via RS-485.
 */

const int numDevices = 8;

// input, output and control pins for each device
const int rxPin = 0;
const int txPin = 1;
const int dePin = 2;
const int notRePin = 3;

const int dataPins[numDevices] =  {4, 6, 8, 10, 12, 14, 16, 18};
const int clockPins[numDevices] = {5, 7, 9, 11, 13, 15, 17, 19};

#endif // MH_SAC_TSTATREADER_PIN_DEFINITIONS_H_INCLUDED

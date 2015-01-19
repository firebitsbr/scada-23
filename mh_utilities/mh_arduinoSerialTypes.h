#ifndef MH_ARDUINOSERIALTYPES_H_INCLUDED
#define MH_ARDUINOSERIALTYPES_H_INCLUDED

#include <Arduino.h>

// Notes:
// - Default communication parameters are 9600-N-8-1.  Invalid calling parameters
//   result in substitution of the default value for that parameter.
// - dataBitsEnum is an enumerated type to represent data bit options
// - parityEnum is an enumerated type to represent parity options
// - stopBitsEnum is an enumerated type to represent stop bit options
// - conformDataRate() checks the data rate and sets it to 9600 if it is not
//   one of the known good values
// - constructSerialParameters() returns the bit flags used to start an Arduino
//   HardwareSerial device
// - printParityEnumText() prints a text string to identify a parityEnum value

// Arduino HardwareSerial constants
// as implemented in HardwareSerial.h
// SERIAL_5N1 0x00
// SERIAL_6N1 0x02
// SERIAL_7N1 0x04
// SERIAL_8N1 0x06
// SERIAL_5N2 0x08
// SERIAL_6N2 0x0A
// SERIAL_7N2 0x0C
// SERIAL_8N2 0x0E
// SERIAL_5E1 0x20
// SERIAL_6E1 0x22
// SERIAL_7E1 0x24
// SERIAL_8E1 0x26
// SERIAL_5E2 0x28
// SERIAL_6E2 0x2A
// SERIAL_7E2 0x2C
// SERIAL_8E2 0x2E
// SERIAL_5O1 0x30
// SERIAL_6O1 0x32
// SERIAL_7O1 0x34
// SERIAL_8O1 0x36
// SERIAL_5O2 0x38
// SERIAL_6O2 0x3A
// SERIAL_7O2 0x3C
// SERIAL_8O2 0x3E

namespace mh_temperatureSensing {

// data bits
enum dataBitsEnum { DATA_BITS_5 = 0x00,
                    DATA_BITS_6 = 0x02,
                    DATA_BITS_7 = 0x04,
                    DATA_BITS_8 = 0x06,
                  };

// parity
enum parityEnum { PARITY_NONE = 0x00,
                  PARITY_EVEN = 0x20,
                  PARITY_ODD  = 0x30
                };

enum stopBitsEnum { STOP_BITS_1 = 0,
                    STOP_BITS_2 = 8
                  };

uint8_t constructSerialParameters(const int dataBits,
                                  const parityEnum parity,
                                  const int stopBits);
// default is 8-N-1
// invalid calling values results in use of default parameter

boolean conformDataRate(long int &dataRate);
// can modify calling value
// default is 9600
// invalid calling value results in use of default parameter

int printParityEnumText(Stream &serialDevice, const parityEnum parityValue);

} // close namespace mh_temperatureSensing

#endif // MH_ARDUINOSERIALTYPES_H_INCLUDED

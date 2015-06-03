#ifndef MH_ARDUINOPINHANDLER_H_INCLUDED
#define MH_ARDUINOPINHANDLER_H_INCLUDED

/*! \brief mh_arduinoPinHandler is a set of definitions and methods to abstract pin interface methods.
 *
 * The definitions and methods are based on the Arduino pin methods.  configure, get and set methods
 * are provided.  For the set methods, PIN_VALUE_LOW is the value that will produce 0V output and
 * PIN_VALUE_HIGH is the value that will produce higher voltage output (e.g. 5V, 3.3V, etc.)
 */

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_encodeData linux implementation incomplete"
#elif _WIN32
#error "mh_encodeData windows implementation incomplete"
#else
#error "mh_encodeData unknown compilation environment"
#endif // determine environment

namespace mh_temperatureSensing {

enum pinLevelEnum { PIN_VALUE_LOW = 0,
                    PIN_VALUE_HIGH = 1 };

enum pinModeEnum { PIN_MODE_INPUT = 0,
                   PIN_MODE_INPUTPULLUP = 1,
                   PIN_MODE_OUTPUT = 2 };


// configure functions return 0 on success, -1 on failure
// on Arduino configure functions provide no indication
// of success or failure (always return 0)
int configurePinInput(const int pinNumber);
int configurePinOutput(const int pinNumber);
int configurePinMode(const int pinNumber,
                     const pinModeEnum pinMode);

// set functions return 0 on success, -1 on failure
// on Arduino set functions provide no indication
// of success or failure (always return 0)
int setPinHigh(const int pinNumber);
int setPinLow(const int pinNumber);
int setPinValue(const int pinNumber,
                const pinLevelEnum pinValue);

// get function defaults to PIN_VALUE_LOW on error
pinLevelEnum getPinValue(const int pinNumber);

} // close namespace mh_temperatureSensing

#endif // MH_ARDUINOPINHANDLER_H_INCLUDED

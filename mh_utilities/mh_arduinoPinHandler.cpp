#include <mh_arduinoPinHandler.h>

#ifdef ARDUINO
// define supporting functions
#include <Arduino.h>
static int configurePinModeArduino(const int pinNumber,
                                   const mh_temperatureSensing::pinModeEnum pinModeValue);
static int setPinValueArduino(const int pinNumber,
                              const mh_temperatureSensing::pinLevelEnum pinValue);
static mh_temperatureSensing::pinLevelEnum getPinValueArduino(const int pinNumber);
#elif __linux__
#error "mh_encodeData linux implementation incomplete"
#elif _WIN32
#error "mh_encodeData windows implementation incomplete"
#else
#error "mh_encodeData unknown compilation environment"
#endif // define supporting functions


int
mh_temperatureSensing::
configurePinInput(const int pinNumber)
{
    return configurePinMode(pinNumber, PIN_MODE_INPUT);
} // end int configurePinInput(const int pinNumber)


int
mh_temperatureSensing::
configurePinOutput(const int pinNumber)
{
    return configurePinMode(pinNumber, PIN_MODE_OUTPUT);
} // end int configurePinOutput(const int pinNumber)


int
mh_temperatureSensing::
configurePinMode(const int pinNumber,
                 const pinModeEnum pinModeValue)
{
    int retVal = 0;
    if (pinNumber >= 0)
    {
#ifdef ARDUINO
    return configurePinModeArduino(pinNumber, pinModeValue);
#elif __linux__
    return configurePinModeLinux(pinNumber, pinModeValue);
#elif _WIN32
    return configurePinModeWindows(pinNumber, pinModeValue);
#else
#error "mh_encodeData unknown compilation environment"
#endif
    }
    else
    {
        retVal = -1;
    } // end if (pinNumber >= 0)
    return retVal;
} // end int configurePinMode()


int
mh_temperatureSensing::
setPinHigh(const int pinNumber)
{
    return setPinValue(pinNumber, PIN_VALUE_HIGH);
} // end int setPinHigh(const int pinNumber)


int
mh_temperatureSensing::
setPinLow(const int pinNumber)
{
    return setPinValue(pinNumber, PIN_VALUE_LOW);
} // end int setPinLow()


int
mh_temperatureSensing::
setPinValue(const int pinNumber,
            const pinLevelEnum pinValue)
{
    int retVal = 0;
    if (pinNumber >= 0)
    {
#ifdef ARDUINO
    retVal = setPinValueArduino(pinNumber, pinValue);
#elif __linux__
    retVal = setPinValueLinux(pinNumber, pinValue);
#elif _WIN32
    retVal = setPinValueWindows(pinNumber, pinValue);
#else
#error "mh_encodeData unknown compilation environment"
#endif
    }
    else
    {
        retVal = -1;
    } // end if (pinNumber >= 0)
    return retVal;
} // end int setPinValue()


mh_temperatureSensing::pinLevelEnum
mh_temperatureSensing::
getPinValue(const int pinNumber)
{
    pinLevelEnum retVal = PIN_VALUE_LOW;
    if (pinNumber >= 0)
    {
#ifdef ARDUINO
    return getPinValueArduino(pinNumber);
#elif __linux__
    return getPinValueLinux(pinNumber);
#elif _WIN32
    return getPinValueWindows(pinNumber);
#else
#error "mh_encodeData unknown compilation environment"
#endif
    }
    else
    {
        // no action, return default value
    } // end if (pinNumber >= 0)
    return retVal;
} // end pinLevelEnum getPinValue()


using mh_temperatureSensing::pinModeEnum;
using mh_temperatureSensing::pinLevelEnum;


#ifdef ARDUINO
int
configurePinModeArduino(const int pinNumber,
                        const pinModeEnum pinModeValue)
{
    int retVal = 0;
    switch (pinModeValue)
    {
    case mh_temperatureSensing::PIN_MODE_INPUT:
        pinMode(pinNumber, INPUT);
        break;
    case mh_temperatureSensing::PIN_MODE_INPUTPULLUP:
        pinMode(pinNumber, INPUT_PULLUP);
        break;
    case mh_temperatureSensing::PIN_MODE_OUTPUT:
        pinMode(pinNumber, OUTPUT);
        break;
    default:
        retVal = -1;
        break;
    } // end switch (pinModeValue)
    return retVal;
} // end int configurePinModeArduino()
#endif // ARDUINO


#ifdef ARDUINO
int
setPinValueArduino(const int pinNumber,
                   const pinLevelEnum pinValue)
{
    int retVal = 0;
    switch (pinValue)
    {
    case mh_temperatureSensing::PIN_VALUE_LOW:
        digitalWrite(pinNumber, LOW);
        break;
    case mh_temperatureSensing::PIN_VALUE_HIGH:
        digitalWrite(pinNumber, HIGH);
        break;
    default:
        retVal = -1;
        break;
    } // end switch (pinValue)
    return retVal;
} // end int setPinArduino()
#endif // ARDUINO


#ifdef ARDUINO
mh_temperatureSensing::pinLevelEnum
getPinValueArduino(const int pinNumber)
{
    pinLevelEnum retVal = mh_temperatureSensing::PIN_VALUE_LOW;
    int pinVal = digitalRead(pinNumber);
    switch (pinVal)
    {
    case LOW:
        retVal = mh_temperatureSensing::PIN_VALUE_LOW;
        break;
    case HIGH:
        retVal = mh_temperatureSensing::PIN_VALUE_HIGH;
        break;
    default:
        // no action, retVal set on declaration
        break;
    } // end switch (pinValue)
    return retVal;
} // end int getPinValueArduino()
#endif // ARDUINO

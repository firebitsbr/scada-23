#include <Arduino.h>

#include "/home/trrooney/MountainHeritage/thermostat_reader/software/mh_arduinoSerialTypes.h"


uint8_t
mh_temperatureSensing::
constructSerialParameters(const int dataBits,
                          const parityEnum parity,
                          const int stopBits)
{
    uint8_t retVal = 0;
    // set flags for data bits
    switch (dataBits)
    {
        case 5:
            retVal &= (uint8_t) DATA_BITS_5;
            break;
        case 6:
            retVal &= (uint8_t) DATA_BITS_6;
            break;
        case 7:
            retVal &= (uint8_t) DATA_BITS_7;
            break;
        case 8:
            retVal &= (uint8_t) DATA_BITS_8;
            break;
        default:
            retVal &= (uint8_t) DATA_BITS_8;
            break;
    } // end switch (dataBits)
    // set flags for parity
    switch (parity)
    {
        case PARITY_NONE:
            retVal &= (uint8_t) PARITY_NONE;
            break;
        case PARITY_EVEN:
            retVal &= (uint8_t) PARITY_EVEN;
            break;
        case PARITY_ODD:
            retVal &= (uint8_t) PARITY_ODD;
            break;
        default:
            retVal &= (uint8_t) PARITY_NONE;
            break;
    } // end switch (parity)
    // set flags for stop bits
    if (stopBits == 2)
    {
        retVal &= (uint8_t) STOP_BITS_2;
    }
    else
    {
        retVal &= (uint8_t) STOP_BITS_1;
    } // end if (stopBits == 2)
    return retVal;
} // end uint8_t constructSerialParameters(const int dataBits, ... )


boolean
mh_temperatureSensing::
conformDataRate(long int &dataRate)
{
    boolean retVal = true;
    // validate data rate
    switch (dataRate)
    {
        case 300L:
        case 600L:
        case 1200L:
        case 2400L:
        case 4800L:
        case 9600L:
        case 14400L:
        case 19200L:
        case 28800L:
        case 38400L:
        case 57600L:
        case 115200L:
            // valid parameters, no action
            break;
        default:
            dataRate = 9600;
            retVal = false;
            break;
    } // end switch (m_dataRate)
    return retVal;
} // end boolean conformDataRate(long int &dataRate);


int
mh_temperatureSensing::
printParityEnumText(Stream &serialDevice,
                    const parityEnum parityValue)
{
    const __FlashStringHelper *PARITY_NONE_TEXT = F("PARITY_NONE");
    const __FlashStringHelper *PARITY_EVEN_TEXT = F("PARITY_EVEN");
    const __FlashStringHelper *PARITY_ODD_TEXT = F("PARITY_ODD");

    int retVal = 0;
    switch (parityValue)
    {
    case PARITY_NONE:
        retVal += serialDevice.print(PARITY_NONE_TEXT);
        break;
    case PARITY_EVEN:
        retVal += serialDevice.print(PARITY_EVEN_TEXT);
        break;
    case PARITY_ODD:
        retVal += serialDevice.print(PARITY_ODD_TEXT);
        break;
    default:
        retVal += serialDevice.print(F("0x"));
        retVal += serialDevice.print(parityValue, HEX);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (parityValue)
    return retVal;
} // end int printParityEnumText(HardwareSerial &serialDevice, const parityEnum parityValue)




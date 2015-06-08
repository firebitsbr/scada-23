// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_cic_commandInterfaceTypes linux implementation incomplete"
#elif _WIN32
#error "mh_cic_commandInterfaceTypes windows implementation incomplete"
#else
#error "mh_cic_commandInterfaceTypes unknown compilation environment"
#endif // determine environment#include <Arduino.h>

#include <mh_cic_commandInterfaceTypes.h>

#include <stdint.h>
#include <stddef.h>

int
mh_temperatureSensing::
printCicMessageTypeText(Stream &serialDevice,
                        const CIC_MESSAGE_TYPE messageType)
{
    return printCicMessageTypeText(serialDevice, (cicMessageTypeEnum) messageType);
} // end int printCicMessageTypeText(Stream &serialDevice, const CIC_MESSAGE_TYPE messageType)


int
mh_temperatureSensing::
printCicMessageTypeText(Stream &serialDevice,
                        const cicMessageTypeEnum messageType)
{
    const __FlashStringHelper *CIC_MSG_NONE_TEXT = F("CIC_MSG_NONE");
    const __FlashStringHelper *CIC_MSG_ACK_TEXT = F("CIC_MSG_ACK");
    const __FlashStringHelper *CIC_MSG_NAK_TEXT = F("CIC_MSG_NAK");
    const __FlashStringHelper *CIC_MSG_TYPE_TEXT = F("CIC_MSG_TYPE");
    const __FlashStringHelper *CIC_MSG_STATUS_TEXT = F("CIC_MSG_STATUS");
    const __FlashStringHelper *CIC_MSG_CAPABILITIES_TEXT = F("CIC_MSG_CAPABILITIES");
    const __FlashStringHelper *CIC_MSG_COMMAND_TEXT = F("CIC_MSG_COMMAND");
    const __FlashStringHelper *CIC_MSG_DATA_TEXT = F("CIC_MSG_DATA");

    int retVal = 0;
    switch (messageType)
    {
    case CIC_MSG_NONE:
        retVal += serialDevice.print(CIC_MSG_NONE_TEXT);
        break;
    case CIC_MSG_ACK:
        retVal += serialDevice.print(CIC_MSG_ACK_TEXT);
        break;
    case CIC_MSG_NAK:
        retVal += serialDevice.print(CIC_MSG_NAK_TEXT);
        break;
    case CIC_MSG_TYPE:
        retVal += serialDevice.print(CIC_MSG_TYPE_TEXT);
        break;
    case CIC_MSG_STATUS:
        retVal += serialDevice.print(CIC_MSG_STATUS_TEXT);
        break;
    case CIC_MSG_CAPABILITIES:
        retVal += serialDevice.print(CIC_MSG_CAPABILITIES_TEXT);
        break;
    case CIC_MSG_COMMAND:
        retVal += serialDevice.print(CIC_MSG_COMMAND_TEXT);
        break;
    case CIC_MSG_DATA:
        retVal += serialDevice.print(CIC_MSG_DATA_TEXT);
        break;
    default:
        retVal += serialDevice.print(F("0x"));
        retVal += serialDevice.print(messageType, HEX);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (messageType)
    return retVal;
} // end int printCicMessageTypeText(Stream &serialDevice, const cicMessageTypeEnum messageType)


int
mh_temperatureSensing::
printCicCommandTypeText(Stream &serialDevice,
                        const CIC_COMMAND_TYPE commandType)
{
    cicCommandTypeEnum cmdTyp = (cicCommandTypeEnum) commandType;
    return printCicCommandTypeText(serialDevice, cmdTyp);
} // end int printCicCommandTypeText(Stream &serialDevice, const CIC_COMMAND_TYPE commandType)


int
mh_temperatureSensing::
printCicCommandTypeText(Stream &serialDevice,
                        const cicCommandTypeEnum commandType)
{
    const __FlashStringHelper *CCT_NO_COMMAND_TEXT = F("CCT_NO_COMMAND");
    const __FlashStringHelper *CCT_REPORT_TYPE_TEXT = F("CCT_REPORT_TYPE");
    const __FlashStringHelper *CCT_REPORT_STATUS_TEXT = F("CCT_REPORT_STATUS");
    const __FlashStringHelper *CCT_REPORT_SENSOR_CAPABILITIES_TEXT = F("CCT_REPORT_SENSOR_CAPABILITIES");
    const __FlashStringHelper *CCT_SET_MEAS_TYPE_TEXT = F("CCT_SET_MEAS_TYPE");
    const __FlashStringHelper *CCT_SET_MEAS_RESOLUTION_TEXT = F("CCT_SET_MEAS_RESOLUTION");
    const __FlashStringHelper *CCT_START_MEASUREMENT_TEXT = F("CCT_START_MEASUREMENT");
    const __FlashStringHelper *CCT_REPORT_VALUES_TEXT = F("CCT_REPORT_VALUES");
    const __FlashStringHelper *CCT_CLEAR_ERRORS_TEXT = F("CCT_CLEAR_ERRORS");

    int retVal = 0;
    switch (commandType)
    {
    case CCT_NO_COMMAND:
        retVal += serialDevice.print(CCT_NO_COMMAND_TEXT);
        break;
    case CCT_REPORT_TYPE:
        retVal += serialDevice.print(CCT_REPORT_TYPE_TEXT);
        break;
    case CCT_REPORT_STATUS:
        retVal += serialDevice.print(CCT_REPORT_STATUS_TEXT);
        break;
    case CCT_REPORT_SENSOR_CAPABILITIES:
        retVal += serialDevice.print(CCT_REPORT_SENSOR_CAPABILITIES_TEXT);
        break;
    case CCT_SET_MEAS_TYPE:
        retVal += serialDevice.print(CCT_SET_MEAS_TYPE_TEXT);
        break;
    case CCT_SET_MEAS_RESOLUTION:
        retVal += serialDevice.print(CCT_SET_MEAS_RESOLUTION_TEXT);
        break;
    case CCT_START_MEASUREMENT:
        retVal += serialDevice.print(CCT_START_MEASUREMENT_TEXT);
        break;
    case CCT_REPORT_VALUES:
        retVal += serialDevice.print(CCT_REPORT_VALUES_TEXT);
        break;
    case CCT_CLEAR_ERRORS:
        retVal += serialDevice.print(CCT_CLEAR_ERRORS_TEXT);
        break;
    default:
        retVal += serialDevice.print(F("0x"));
        retVal += serialDevice.print(commandType, HEX);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (commandType)
    return retVal;
} // end int printCicCommandTypeText(Stream &serialDevice, const cicCommandTypeEnum messageType)


int
mh_temperatureSensing::
printCicErrorTypeText(Stream &serialDevice,
                      const CIC_ERROR_TYPE errorValue)
{
    return (printCicErrorTypeText(serialDevice, (cicErrorTypeEnum) errorValue));
} // end int printCicErrorTypeText(Stream &serialDevice, const Cic_ERROR_TYPE errorValue)


int
mh_temperatureSensing::
printCicErrorTypeText(Stream &serialDevice,
                      const cicErrorTypeEnum errorValue)
{

    const __FlashStringHelper *CIC_SUCCESS_TEXT = F("CIC_SUCCESS");
    const __FlashStringHelper *CIC_ERR_UNINIT_TEXT = F("CIC_ERR_UNINIT");
    const __FlashStringHelper *CIC_ERR_BAD_COUNT_TEXT = F("CIC_ERR_BAD_COUNT");
    const __FlashStringHelper *CIC_ERR_BAD_ARRAY_PTR_TEXT = F("CIC_ERR_BAD_ARRAY_PTR");
    const __FlashStringHelper *CIC_COMM_ERROR_TEXT = F("CIC_COMM_ERROR");
    const __FlashStringHelper *CIC_COMMAND_ERROR_TEXT = F("CIC_COMMAND_ERROR");
    const __FlashStringHelper *CIC_ERR_TIMEOUT_TEXT = F("CIC_ERR_TIMEOUT");
    const __FlashStringHelper *CIC_ERR_INSUFF_MEMORY_TEXT = F("CIC_ERR_INSUFF_MEMORY");
    const __FlashStringHelper *CIC_ERR_STATE_ERROR_TEXT = F("CIC_ERR_STATE_ERROR");
    const __FlashStringHelper *CIC_OTHER_ERROR_TEXT = F("CIC_OTHER_ERROR");
    const __FlashStringHelper *CIC_ERROR_UNKNOWN_TEXT = F("CIC_ERROR_UNKNOWN");

    const int cicErrorTypeStringsCount = 11;
    const __FlashStringHelper * cicErrorTypeStrings[cicErrorTypeStringsCount] PROGMEM = { CIC_SUCCESS_TEXT,
                                                                                          CIC_ERR_UNINIT_TEXT,
                                                                                          CIC_ERR_BAD_COUNT_TEXT,
                                                                                          CIC_ERR_BAD_ARRAY_PTR_TEXT,
                                                                                          CIC_COMM_ERROR_TEXT,
                                                                                          CIC_COMMAND_ERROR_TEXT,
                                                                                          CIC_ERR_TIMEOUT_TEXT,
                                                                                          CIC_ERR_INSUFF_MEMORY_TEXT,
                                                                                          CIC_ERR_STATE_ERROR_TEXT,
                                                                                          CIC_OTHER_ERROR_TEXT,
                                                                                          CIC_ERROR_UNKNOWN_TEXT
                                                                                        };

    int retVal = 0;
    if (errorValue == CIC_SUCCESS)
    {
        retVal += serialDevice.print(CIC_SUCCESS_TEXT);
    }
    else
    {
        CIC_ERROR_TYPE workVar = errorValue;
        int maxCounter = 8 * sizeof(CIC_ERROR_TYPE);
        CIC_ERROR_TYPE errorSelectBit = 1;
        boolean havePrinted = false;
        for (int i=0; i<maxCounter; i++)
        {
            if (workVar & errorSelectBit)
            {
                if (havePrinted == true)
                {
                    retVal += serialDevice.print(F(", "));
                }
                else
                {
                    havePrinted = true;
                } // end if (havePrinted == true)
                // verify array index (i+1) is within cicErrorTypeStrings array bounds
                // array index is (i+1) because CIC_SUCCESS_TEXT is at array index 0
                if ((i+1) < cicErrorTypeStringsCount)
                {
                    retVal += serialDevice.print(cicErrorTypeStrings[i+1]);
                }
                else
                {
                    retVal += serialDevice.print(F("0x"));
                    retVal += serialDevice.print(errorSelectBit, HEX);
                } // end if ((i+1) < cicErrorTypeStringsCount)
                workVar &= ~errorSelectBit;
            } // end if (workVar & errorSelectBit)
            errorSelectBit <<= 1;
        } // end for (i)
    } // end if (errorsToPrint == CIC_SUCCESS)
    return retVal;
} // end int printCicErrorTypeText(Stream &serialDevice, const cicErrorTypeEnum errorValue)


int
mh_temperatureSensing::
printComErrorTypeText(Stream &serialDevice,
                      const COM_ERROR_TYPE errorValue)
{
    return (printComErrorTypeText(serialDevice, (comErrorTypeEnum) errorValue));
} // end int printComErrorTypeText(Stream &serialDevice, const COM_ERROR_TYPE errorValue)


int
mh_temperatureSensing::
printComErrorTypeText(Stream &serialDevice,
                      const comErrorTypeEnum errorValue)
{
    const __FlashStringHelper *COMM_SUCCESS_TEXT = F("COMM_SUCCESS");
    const __FlashStringHelper *COM_UNKNOWN_TYPE_TEXT = F("COM_UNKNOWN_TYPE");
    const __FlashStringHelper *COM_BAD_COUNT_TEXT = F("COM_BAD_COUNT");
    const __FlashStringHelper *COM_INSUFF_MEMORY_TEXT = F("COM_INSUFF_MEMORY");
    const __FlashStringHelper *COM_BAD_CRC_TEXT = F("COM_BAD_CRC");
    const __FlashStringHelper *COM_TIMEOUT_TEXT = F("COM_TIMEOUT");
    const __FlashStringHelper *COM_UNABLE_TEXT = F("COM_UNABLE");
    const __FlashStringHelper *COM_OTHER_ERROR_TEXT = F("COM_OTHER_ERROR");
    const __FlashStringHelper *COM_ERROR_UNKNOWN_TEXT = F("COM_ERROR_UNKNOWN");

    const int comErrorTypeStringsCount = 9;
    const __FlashStringHelper * comErrorTypeStrings[comErrorTypeStringsCount] PROGMEM = { COMM_SUCCESS_TEXT,
                                                                                          COM_UNKNOWN_TYPE_TEXT,
                                                                                          COM_BAD_COUNT_TEXT,
                                                                                          COM_INSUFF_MEMORY_TEXT,
                                                                                          COM_BAD_CRC_TEXT,
                                                                                          COM_TIMEOUT_TEXT,
                                                                                          COM_UNABLE_TEXT,
                                                                                          COM_OTHER_ERROR_TEXT,
                                                                                          COM_ERROR_UNKNOWN_TEXT
                                                                                        };

    int retVal = 0;
    if (errorValue == COMM_SUCCESS)
    {
        retVal += serialDevice.print(COMM_SUCCESS_TEXT);
    }
    else
    {
        COM_ERROR_TYPE workVar = errorValue;
        int maxCounter = 8 * sizeof(COM_ERROR_TYPE);
        COM_ERROR_TYPE errorSelectBit = 1;
        boolean havePrinted = false;
        for (int i=0; i<maxCounter; i++)
        {
            if (workVar & errorSelectBit)
            {
                if (havePrinted == true)
                {
                    retVal += serialDevice.print(F(", "));
                }
                else
                {
                    havePrinted = true;
                } // end if (havePrinted == true)
                // verify array index (i+1) is within comErrorTypeStrings array bounds
                // array index is (i+1) because COMM_SUCCESS_TEXT is at array index 0
                if ((i+1) < comErrorTypeStringsCount)
                {
                    retVal += serialDevice.print(comErrorTypeStrings[i+1]);
                }
                else
                {
                    retVal += serialDevice.print(F("0x"));
                    retVal += serialDevice.print(errorSelectBit, HEX);
                } // end if ((i+1) < comErrorTypeStringsCount)
                workVar &= ~errorSelectBit;
            } // end if (workVar & errorSelectBit)
            errorSelectBit <<= 1;
        } // end for (i)
    } // end if (errorsToPrint == COMM_SUCCESS)
    return retVal;
} // end int printComErrorTypeText(Stream &serialDevice, const comErrorTypeEnum errorValue)


int
mh_temperatureSensing::
printCicSensorTypeText(Stream &serialDevice, const CIC_SENSOR_TYPE sensorType)
{
    return printCicSensorTypeText(serialDevice, (cicSensorTypeEnum) sensorType);
} // end int printCicSensorTypeText(Stream &serialDevice, const CIC_SENSOR_TYPE sensorType)


int
mh_temperatureSensing::
printCicSensorTypeText(Stream &serialDevice, const cicSensorTypeEnum sensorType)
{
    const __FlashStringHelper *CST_NONE_TEXT = F("CST_NONE");
    const __FlashStringHelper *CST_TEMP_ANY_TEXT = F("CST_TEMP_ANY");
    const __FlashStringHelper *CST_TEMP_SENSIRION_TEXT = F("CST_TEMP_SENSIRION");
    const __FlashStringHelper *CST_TEMP_TC_B_TEXT = F("CST_TEMP_TC_B");
    const __FlashStringHelper *CST_TEMP_TC_E_TEXT = F("CST_TEMP_TC_E");
    const __FlashStringHelper *CST_TEMP_TC_J_TEXT = F("CST_TEMP_TC_J");
    const __FlashStringHelper *CST_TEMP_TC_K_TEXT = F("CST_TEMP_TC_K");
    const __FlashStringHelper *CST_TEMP_TC_N_TEXT = F("CST_TEMP_TC_N");
    const __FlashStringHelper *CST_TEMP_TC_R_TEXT = F("CST_TEMP_TC_R");
    const __FlashStringHelper *CST_TEMP_TC_S_TEXT = F("CST_TEMP_TC_S");
    const __FlashStringHelper *CST_TEMP_TC_T_TEXT = F("CST_TEMP_TC_T");
    const __FlashStringHelper *CST_TEMP_TC_OTHER_TEXT = F("CST_TEMP_TC_OTHER");
    const __FlashStringHelper *CST_TEMP_RTD_PT10_TEXT = F("CST_TEMP_RTD_PT10");
    const __FlashStringHelper *CST_TEMP_RTD_PT50_TEXT = F("CST_TEMP_RTD_PT50");
    const __FlashStringHelper *CST_TEMP_RTD_PT100_TEXT = F("CST_TEMP_RTD_PT100");
    const __FlashStringHelper *CST_TEMP_RTD_PT200_TEXT = F("CST_TEMP_RTD_PT200");
    const __FlashStringHelper *CST_TEMP_RTD_PT500_TEXT = F("CST_TEMP_RTD_PT500");
    const __FlashStringHelper *CST_TEMP_RTD_PT1000_TEXT = F("CST_TEMP_RTD_PT1000");
    const __FlashStringHelper *CST_TEMP_RTD_1000_TEXT = F("CST_TEMP_RTD_1000");
    const __FlashStringHelper *CST_TEMP_RTD_NI120_TEXT = F("CST_TEMP_RTD_NI120");
    const __FlashStringHelper *CST_TEMP_RTD_OTHER_TEXT = F("CST_TEMP_RTD_OTHER");
    const __FlashStringHelper *CST_THERMISTOR_44004_44033_TEXT = F("CST_THERMISTOR_44004_44033");
    const __FlashStringHelper *CST_THERMISTOR_44005_44030_TEXT = F("CST_THERMISTOR_44005_44030");
    const __FlashStringHelper *CST_THERMISTOR_44007_44034_TEXT = F("CST_THERMISTOR_44007_44034");
    const __FlashStringHelper *CST_THERMISTOR_44006_44031_TEXT = F("CST_THERMISTOR_44006_44031");
    const __FlashStringHelper *CST_THERMISTOR_44008_44032_TEXT = F("CST_THERMISTOR_44008_44032");
    const __FlashStringHelper *CST_THERMISTOR_YSI400_TEXT = F("CST_THERMISTOR_YSI400");
    const __FlashStringHelper *CST_THERMISTOR_SPECTRUM_1003K_1K_TEXT = F("CST_THERMISTOR_SPECTRUM_1003K_1K");
    const __FlashStringHelper *CST_THERMISTOR_CUSTOM_SH_TEXT = F("CST_THERMISTOR_CUSTOM_SH");
    const __FlashStringHelper *CST_THERMISTOR_OTHER_TEXT = F("CST_THERMISTOR_OTHER");
    const __FlashStringHelper *CST_TEMP_DIODE_TEXT = F("CST_TEMP_DIODE");
    const __FlashStringHelper *CST_TEMP_SENS_RESISTOR_TEXT = F("CST_TEMP_SENS_RESISTOR");
    const __FlashStringHelper *CST_TEMP_OTHER_TEXT = F("CST_TEMP_OTHER");
    const __FlashStringHelper *CST_DIRECT_ADC_TEXT = F("CST_DIRECT_ADC");
    const __FlashStringHelper *CST_HUMID_ANY_TEXT = F("CST_HUMID_ANY");
    const __FlashStringHelper *CST_HUMID_SENSIRION_TEXT = F("CST_HUMID_SENSIRION");
    const __FlashStringHelper *CST_HUMID_OTHER_TEXT = F("CST_HUMID_OTHER");
    const __FlashStringHelper *CST_PRESSURE_ANY_TEXT = F("CST_PRESSURE_ANY");
    const __FlashStringHelper *CST_PRESSURE_THRESHOLD_TEXT = F("CST_PRESSURE_THRESHOLD");
    const __FlashStringHelper *CST_PRESSURE_OTHER_TEXT = F("CST_PRESSURE_OTHER");
    const __FlashStringHelper *CST_CURRENT_ANY_TEXT = F("CST_CURRENT_ANY");
    const __FlashStringHelper *CST_CURRENT_AC_TEXT = F("CST_CURRENT_AC");
    const __FlashStringHelper *CST_CURRENT_DC_TEXT = F("CST_CURRENT_DC");
    const __FlashStringHelper *CST_CURRENT_OTHER_TEXT = F("CST_CURRENT_OTHER");

    int retVal = 0;
    switch (sensorType)
    {
    case CST_NONE:
        retVal += serialDevice.print(CST_NONE_TEXT);
        break;
    case CST_TEMP_ANY:
        retVal += serialDevice.print(CST_TEMP_ANY_TEXT);
        break;
    case CST_TEMP_SENSIRION:
        retVal += serialDevice.print(CST_TEMP_SENSIRION_TEXT);
        break;
    case CST_TEMP_TC_B:
        retVal += serialDevice.print(CST_TEMP_TC_B_TEXT);
        break;
    case CST_TEMP_TC_E:
        retVal += serialDevice.print(CST_TEMP_TC_E_TEXT);
        break;
    case CST_TEMP_TC_J:
        retVal += serialDevice.print(CST_TEMP_TC_J_TEXT);
        break;
    case CST_TEMP_TC_K:
        retVal += serialDevice.print(CST_TEMP_TC_K_TEXT);
        break;
    case CST_TEMP_TC_N:
        retVal += serialDevice.print(CST_TEMP_TC_N_TEXT);
        break;
    case CST_TEMP_TC_R:
        retVal += serialDevice.print(CST_TEMP_TC_R_TEXT);
        break;
    case CST_TEMP_TC_S:
        retVal += serialDevice.print(CST_TEMP_TC_S_TEXT);
        break;
    case CST_TEMP_TC_T:
        retVal += serialDevice.print(CST_TEMP_TC_T_TEXT);
        break;
    case CST_TEMP_TC_OTHER:
        retVal += serialDevice.print(CST_TEMP_TC_OTHER_TEXT);
        break;
    case CST_TEMP_RTD_PT10:
        retVal += serialDevice.print(CST_TEMP_RTD_PT10_TEXT);
        break;
    case CST_TEMP_RTD_PT50:
        retVal += serialDevice.print(CST_TEMP_RTD_PT50_TEXT);
        break;
    case CST_TEMP_RTD_PT100:
        retVal += serialDevice.print(CST_TEMP_RTD_PT100_TEXT);
        break;
    case CST_TEMP_RTD_PT200:
        retVal += serialDevice.print(CST_TEMP_RTD_PT200_TEXT);
        break;
    case CST_TEMP_RTD_PT500:
        retVal += serialDevice.print(CST_TEMP_RTD_PT500_TEXT);
        break;
    case CST_TEMP_RTD_PT1000:
        retVal += serialDevice.print(CST_TEMP_RTD_PT1000_TEXT);
        break;
    case CST_TEMP_RTD_1000:
        retVal += serialDevice.print(CST_TEMP_RTD_1000_TEXT);
        break;
    case CST_TEMP_RTD_NI120:
        retVal += serialDevice.print(CST_TEMP_RTD_NI120_TEXT);
        break;
    case CST_TEMP_RTD_OTHER:
        retVal += serialDevice.print(CST_TEMP_RTD_OTHER_TEXT);
        break;
    case CST_THERMISTOR_44004_44033:
        retVal += serialDevice.print(CST_THERMISTOR_44004_44033_TEXT);
        break;
    case CST_THERMISTOR_44005_44030:
        retVal += serialDevice.print(CST_THERMISTOR_44005_44030_TEXT);
        break;
    case CST_THERMISTOR_44007_44034:
        retVal += serialDevice.print(CST_THERMISTOR_44007_44034_TEXT);
        break;
    case CST_THERMISTOR_44006_44031:
        retVal += serialDevice.print(CST_THERMISTOR_44006_44031_TEXT);
        break;
    case CST_THERMISTOR_44008_44032:
        retVal += serialDevice.print(CST_THERMISTOR_44008_44032_TEXT);
        break;
    case CST_THERMISTOR_YSI400:
        retVal += serialDevice.print(CST_THERMISTOR_YSI400_TEXT);
        break;
    case CST_THERMISTOR_SPECTRUM_1003K_1K:
        retVal += serialDevice.print(CST_THERMISTOR_SPECTRUM_1003K_1K_TEXT);
        break;
    case CST_THERMISTOR_CUSTOM_SH:
        retVal += serialDevice.print(CST_THERMISTOR_CUSTOM_SH_TEXT);
        break;
    case CST_THERMISTOR_OTHER:
        retVal += serialDevice.print(CST_THERMISTOR_OTHER_TEXT);
        break;
    case CST_TEMP_DIODE:
        retVal += serialDevice.print(CST_TEMP_DIODE_TEXT);
        break;
    case CST_TEMP_SENS_RESISTOR:
        retVal += serialDevice.print(CST_TEMP_SENS_RESISTOR_TEXT);
        break;
    case CST_TEMP_OTHER:
        retVal += serialDevice.print(CST_TEMP_OTHER_TEXT);
        break;
    case CST_DIRECT_ADC:
        retVal += serialDevice.print(CST_DIRECT_ADC_TEXT);
        break;
    case CST_HUMID_ANY:
        retVal += serialDevice.print(CST_HUMID_ANY_TEXT);
        break;
    case CST_HUMID_SENSIRION:
        retVal += serialDevice.print(CST_HUMID_SENSIRION_TEXT);
        break;
    case CST_HUMID_OTHER:
        retVal += serialDevice.print(CST_HUMID_OTHER_TEXT);
        break;
    case CST_PRESSURE_ANY:
        retVal += serialDevice.print(CST_PRESSURE_ANY_TEXT);
        break;
    case CST_PRESSURE_THRESHOLD:
        retVal += serialDevice.print(CST_PRESSURE_THRESHOLD_TEXT);
        break;
    case CST_PRESSURE_OTHER:
        retVal += serialDevice.print(CST_PRESSURE_OTHER_TEXT);
        break;
    case CST_CURRENT_ANY:
        retVal += serialDevice.print(CST_CURRENT_ANY_TEXT);
        break;
    case CST_CURRENT_AC:
        retVal += serialDevice.print(CST_CURRENT_AC_TEXT);
        break;
    case CST_CURRENT_DC:
        retVal += serialDevice.print(CST_CURRENT_DC_TEXT);
        break;
    case CST_CURRENT_OTHER:
        retVal += serialDevice.print(CST_CURRENT_OTHER_TEXT);
        break;
    default:
        retVal += serialDevice.print(F("0x"));
        retVal += serialDevice.print(sensorType, HEX);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (sensorType)
    return retVal;
} // end int printCicSensorTypeText(Stream &serialDevice, const cicSensorTypeEnum sensorType)


int
mh_temperatureSensing::
printBufferSourceText(Stream &serialDevice,
                      const bufferSourceEnum bufferSource)
{
    const __FlashStringHelper *NO_BUFFER_TEXT = F("NO_BUFFER");
    const __FlashStringHelper *ALLOCATED_TEXT = F("ALLOCATED");
    const __FlashStringHelper *BY_REFERENCE_TEXT = F("BY_REFERENCE");

    int retVal = 0;
    switch (bufferSource)
    {
    case NO_BUFFER:
        retVal += serialDevice.print(NO_BUFFER_TEXT);
        break;
    case ALLOCATED:
        retVal += serialDevice.print(ALLOCATED_TEXT);
        break;
    case BY_REFERENCE:
        retVal += serialDevice.print(BY_REFERENCE_TEXT);
        break;
    default:
        retVal += serialDevice.print(F("0x"));
        retVal += serialDevice.print(bufferSource, HEX);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (bufferSource)
    return retVal;
} // end int printBufferSourceText(Stream &serialDevice, const bufferSourceEnum bufferSource)


int
mh_temperatureSensing::
printCicStateTypeText(Stream &serialDevice,
                      const cicStateEnum stateValue)
{
    const __FlashStringHelper *CIC_UNINIT_TEXT = F("CIC_UNINIT");
    const __FlashStringHelper *CIC_INITIALIZED_TEXT = F("CIC_INITIALIZED");
    const __FlashStringHelper *CIC_START_TEXT = F("CIC_START");
    const __FlashStringHelper *CIC_INIT_TEMP_TEXT = F("CIC_INIT_TEMP");
    const __FlashStringHelper *CIC_READING_TEMP_TEXT = F("CIC_READING_TEMP");
    const __FlashStringHelper *CIC_TEMP_REPORTERROR_TEXT = F("CIC_TEMP_REPORTERROR");
    const __FlashStringHelper *CIC_TEMP_COMPLETE_TEXT = F("CIC_TEMP_COMPLETE");
    const __FlashStringHelper *CIC_INIT_HUMID_TEXT = F("CIC_INIT_HUMID");
    const __FlashStringHelper *CIC_READING_HUMID_TEXT = F("CIC_READING_HUMID");
    const __FlashStringHelper *CIC_HUMID_REPORTERROR_TEXT = F("CIC_HUMID_REPORTERROR");
    const __FlashStringHelper *CIC_HUMID_COMPLETE_TEXT = F("CIC_HUMID_COMPLETE");
    const __FlashStringHelper *CIC_WAIT_NEXT_ACTION_TEXT = F("CIC_WAIT_NEXT_ACTION");
    const __FlashStringHelper *CIC_ALL_COMPLETE_TEXT = F("CIC_ALL_COMPLETE");
    const __FlashStringHelper *CIC_SHUTDOWN_TEXT = F("CIC_SHUTDOWN");
    const __FlashStringHelper *CIC_STATE_ERROR_TEXT = F("CIC_STATE_ERROR");
    const __FlashStringHelper *CIC_STOP_TEXT = F("CIC_STOP");
    const __FlashStringHelper *CIC_SAFETY_STOP_TEXT = F("CIC_SAFETY_STOP");

    int retVal = 0;
    switch (stateValue)
    {
    case CIC_UNINIT:
        retVal += serialDevice.print(CIC_UNINIT_TEXT);
        break;
    case CIC_INITIALIZED:
        retVal += serialDevice.print(CIC_INITIALIZED_TEXT);
        break;
    case CIC_START:
        retVal += serialDevice.print(CIC_START_TEXT);
        break;
    case CIC_INIT_TEMP:
        retVal += serialDevice.print(CIC_INIT_TEMP_TEXT);
        break;
    case CIC_READING_TEMP:
        retVal += serialDevice.print(CIC_READING_TEMP_TEXT);
        break;
    case CIC_TEMP_REPORTERROR:
        retVal += serialDevice.print(CIC_TEMP_REPORTERROR_TEXT);
        break;
    case CIC_TEMP_COMPLETE:
        retVal += serialDevice.print(CIC_TEMP_COMPLETE_TEXT);
        break;
    case CIC_INIT_HUMID:
        retVal += serialDevice.print(CIC_INIT_HUMID_TEXT);
        break;
    case CIC_READING_HUMID:
        retVal += serialDevice.print(CIC_READING_HUMID_TEXT);
        break;
    case CIC_HUMID_REPORTERROR:
        retVal += serialDevice.print(CIC_HUMID_REPORTERROR_TEXT);
        break;
    case CIC_HUMID_COMPLETE:
        retVal += serialDevice.print(CIC_HUMID_COMPLETE_TEXT);
        break;
    case CIC_WAIT_NEXT_ACTION:
        retVal += serialDevice.print(CIC_WAIT_NEXT_ACTION_TEXT);
        break;
    case CIC_ALL_COMPLETE:
        retVal += serialDevice.print(CIC_ALL_COMPLETE_TEXT);
        break;
    case CIC_SHUTDOWN:
        retVal += serialDevice.print(CIC_SHUTDOWN_TEXT);
        break;
    case CIC_STATE_ERROR:
        retVal += serialDevice.print(CIC_STATE_ERROR_TEXT);
        break;
    case CIC_STOP:
        retVal += serialDevice.print(CIC_STOP_TEXT);
        break;
    case CIC_SAFETY_STOP:
        retVal += serialDevice.print(CIC_SAFETY_STOP_TEXT);
        break;
    default:
        retVal += serialDevice.print(stateValue);
        retVal += serialDevice.print(F("(Error - invalid)"));
        break;
    } // end switch (stateValue)
    return retVal;
} // end int printCicStateTypeText(Stream &serialDevice, ... )


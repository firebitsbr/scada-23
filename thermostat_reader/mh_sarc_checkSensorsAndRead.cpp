// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_checkSensorsAndRead linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_checkSensorsAndRead windows implementation incomplete"
#else
#error "mh_sarc_checkSensorsAndRead unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <string.h>
#include <stddef.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>
#include <mh_sarc_sensirionArrayReaderClass.h>
#include <mh_sarc_encodeData.h>


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
checkSensorsTemp(SAC_FLAGS_TYPE& dataReadyFlags)
{
    SAC_ERROR_TYPE retVal = 0;
    dataReadyFlags = 0;
#ifdef MH_SARC_DEBUG_TEMP_MEASUREMENT
    sarcDebugStream.print(F("checkSensorsTemp m_tempInProcessFlags = 0x"));
    sarcDebugStream.println(m_tempInProcessFlags, HEX);
#endif // MH_SARC_DEBUG_TEMP_MEASUREMENT
    if (m_tempInProcessFlags != 0)
    {
        retVal += checkSensorsAndRead(m_tempInProcessFlags, dataReadyFlags, m_tempErrorFlags);
#ifdef MH_SARC_DEBUG_TEMP_MEASUREMENT
        sarcDebugStream.print(F("checkSensorsTemp dataReadyFlags = 0x"));
        sarcDebugStream.println(dataReadyFlags, HEX);
#endif // MH_SARC_DEBUG_TEMP_MEASUREMENT
        if (dataReadyFlags != (SAC_FLAGS_TYPE) 0)
        {
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<maxSensors; i++)
            {
                if (sensorSelectBit & dataReadyFlags)
                {
                    m_lastTemperatureArray[i] = m_sensorPtrArray[i]->calcTemp(m_rawDataArray[i]);
#ifdef MH_SARC_OUTPUT_DATA_WHEN_READ
                    sarcDebugStream.print(F("checkSensorsTemp Sensor "));
                    sarcDebugStream.print(i);
                    sarcDebugStream.print(F(" Temperature "));
                    sarcDebugStream.print(m_lastTemperatureArray[i]);
                    sarcDebugStream.println(F("C"));
#endif // MH_SARC_OUTPUT_DATA_WHEN_READ
                } // end if (sensorSelectBit & dataReadyFlags)
                sensorSelectBit <<= 1;
            } // end for (int i=0; i<maxSensors; i++)
        } // end if (dataReadyFlags != 0)
    } // end if (m_tempInProcessFlags != 0)
    return retVal;
} // end SAC_ERROR_TYPE checkSensorsTemp(SAC_FLAGS_TYPE& dataReadyFlags)


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
checkSensorsHumid(SAC_FLAGS_TYPE& dataReadyFlags)
{
    SAC_ERROR_TYPE retVal = 0;
    dataReadyFlags = 0;
#ifdef MH_SARC_DEBUG_HUMID_MEASUREMENT
    sarcDebugStream.print(F("checkSensorsHumid m_humidInProcessFlags = 0x"));
    sarcDebugStream.println(m_humidInProcessFlags, HEX);
#endif // MH_SARC_DEBUG_HUMID_MEASUREMENT
    if (m_humidInProcessFlags != 0)
    {
        retVal += checkSensorsAndRead(m_humidInProcessFlags, dataReadyFlags, m_humidErrorFlags);
#ifdef MH_SARC_DEBUG_HUMID_MEASUREMENT
        sarcDebugStream.print(F("checkSensorsHumid dataReadyFlags = 0x"));
        sarcDebugStream.println(dataReadyFlags, HEX);
#endif // MH_SARC_DEBUG_HUMID_MEASUREMENT
        if (dataReadyFlags != 0)
        {
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<maxSensors; i++)
            {
                if (sensorSelectBit & dataReadyFlags)
                {
                    m_lastHumidityArray[i]= m_sensorPtrArray[i]->calcHumi(m_rawDataArray[i], m_lastTemperatureArray[i]);
#ifdef MH_SARC_OUTPUT_DATA_WHEN_READ
                    sarcDebugStream.print(F("checkSensorsHumidity Sensor "));
                    sarcDebugStream.print(i);
                    sarcDebugStream.print(F(" Humidity "));
                    sarcDebugStream.print(m_lastHumidityArray[i]);
                    sarcDebugStream.println(F("%"));
#endif // MH_SARC_OUTPUT_DATA_WHEN_READ
                } // end if (sensorSelectBit & dataReadyFlags)
                sensorSelectBit <<= 1;
            } // end for (int i=0; i<maxSensors; i++)
        } // end if (dataReadyFlags != 0)
    } // end if (m_humidInProcessFlags != 0)
    return retVal;
} // end SAC_ERROR_TYPE checkSensorsHumid(SAC_FLAGS_TYPE& dataReadyFlags)


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
checkSensorsAndRead(SAC_FLAGS_TYPE& activeSensorSet,
                    SAC_FLAGS_TYPE& dataReadyFlags,
                    SAC_FLAGS_TYPE& errorFlags)
{
#ifdef MH_SARC_DEBUG_MEASUREMENT
    sarcDebugStream.print(F("calling checkSensorsAndRead, activeSensorSet "));
    printSensirionFlagsType(sarcDebugStream, activeSensorSet);
    sarcDebugStream.println();
#endif // MH_SARC_DEBUG_MEASUREMENT
    SAC_ERROR_TYPE retVal = 0;
    if (activeSensorSet != 0)
    {
        #ifdef MH_SARC_DEBUG_MEASUREMENT
        sarcDebugStream.println("start of checkSensorsAndRead");
        sarcDebugStream.print(F("  activeSensorSet = ( "));
        printSensirionFlagsType(sarcDebugStream, activeSensorSet);
        sarcDebugStream.println(F(" )"));
        sarcDebugStream.print(F("  dataReadyFlags  = ( "));
        printSensirionFlagsType(sarcDebugStream, dataReadyFlags);
        sarcDebugStream.println(F(" )"));
        sarcDebugStream.print(F("  errorFlags      = ( "));
        printSensirionFlagsType(sarcDebugStream, errorFlags);
        sarcDebugStream.println(F(" )"));
        #endif // MH_SARC_DEBUG_MEASUREMENT
        SAC_FLAGS_TYPE sensorSelectBit = 1;
        for (int i=0; i<maxSensors; i++)
        {
            if (sensorSelectBit & activeSensorSet)
            {
                // note - measRdy() both checks for ready and reads value
                // this is accomplished using the result pointer provided
                // in the call to start measurement meas(uint8_t cmd, uint16_t *result, bool block)
                // measRdy() returns:
                //   - 0 when waiting
                //   - S_Meas_Rdy when message is read
                //   - S_Meas_Rdy when error due to null internal data pointer
                //   - non-zero on error
                uint8_t readyVal = m_sensorPtrArray[i]->measRdy();
#ifdef MH_SARC_DEBUG_MEASUREMENT
                sarcDebugStream.print(F("sensor "));
                sarcDebugStream.print(i);
                sarcDebugStream.print(F(" readyVal "));
                sarcDebugStream.print(readyVal, HEX);
                sarcDebugStream.print(", measurement result = 0x");
                sarcDebugStream.println((unsigned long) m_rawDataArray[i], HEX);
#endif // MH_SARC_DEBUG_MEASUREMENT
                switch (readyVal)
                {
                case 0:
                    // waiting for sensor ready, no action
                    break;
                case S_Meas_Rdy:
                    // measurement was ready and has been completed
                    activeSensorSet &= (~sensorSelectBit);
                    dataReadyFlags |= sensorSelectBit;
                    errorFlags &= (~sensorSelectBit);
                    break;
                case S_Err_CRC:
                    // communication error
                    activeSensorSet &= (~sensorSelectBit);
                    dataReadyFlags &= (~sensorSelectBit);
                    errorFlags |= sensorSelectBit;
                    m_errorFlags |= SAC_COMM_ERROR;
                    retVal |= SAC_COMM_ERROR;
                    break;
                default:
                    // other error
                    activeSensorSet &= (~sensorSelectBit);
                    errorFlags |= sensorSelectBit;
                    m_errorFlags |= SAC_MEASURE_ERROR;
                    retVal |= SAC_MEASURE_ERROR;
                    break;
                } // end switch (readyVal)
            } // end if (sensorSelectBit & activeSensorSet)
            sensorSelectBit <<= 1;
        } // end for (i=0; i<maxSensors; i++)
        #ifdef MH_SARC_DEBUG_MEASUREMENT
        sarcDebugStream.println("end of checkSensorsAndRead");
        sarcDebugStream.print(F("  activeSensorSet = ( "));
        printSensirionFlagsType(sarcDebugStream, activeSensorSet);
        sarcDebugStream.println(F(" )"));
        sarcDebugStream.print(F("  dataReadyFlags  = ( "));
        printSensirionFlagsType(sarcDebugStream, dataReadyFlags);
        sarcDebugStream.println(F(" )"));
        sarcDebugStream.print(F("  errorFlags      = ( "));
        printSensirionFlagsType(sarcDebugStream, errorFlags);
        sarcDebugStream.println(F(" )"));
        #endif // MH_SARC_DEBUG_MEASUREMENT
    } // end if (activeSensorSet != 0)
    return retVal;
} // int checkSensorsAndRead()

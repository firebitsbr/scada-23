// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_beginMeasurement linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_beginMeasurement windows implementation incomplete"
#else
#error "mh_sarc_beginMeasurement unknown compilation environment"
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
beginTempMeasurement(const SAC_FLAGS_TYPE sensorNumberFlags)
{
#ifdef MH_SARC_DEBUG_TEMP_MEASUREMENT
    sarcDebugStream.println(F("Begin temperature measurement"));
    sarcDebugStream.print(F("sensorNumberFlags = 0x"));
    sarcDebugStream.println(sensorNumberFlags, HEX);
#endif // MH_SARC_DEBUG_TEMP_MEASUREMENT
    // command the start of temperature measurement
    SAC_ERROR_TYPE retVal = clearArrayElements(sensorNumberFlags,
                                               m_readerArraySize,
                                               m_lastTemperatureArray);
    retVal |= beginMeasurement(sensorNumberFlags,
                               m_tempInProcessFlags,
                               m_tempErrorFlags,
                               TEMP);
    m_tempMeasStartTime = millis();
#ifdef MH_SARC_DEBUG_TEMP_MEASUREMENT
    sarcDebugStream.print(F("m_tempInProcessFlags = 0x"));
    sarcDebugStream.println(m_tempInProcessFlags, HEX);
    sarcDebugStream.print(F("m_tempMeasStartTime "));
    sarcDebugStream.println(m_tempMeasStartTime);
    sarcDebugStream.print(F("beginTempMeasurement return value = 0x"));
    sarcDebugStream.println(retVal, HEX);
#endif // MH_SARC_DEBUG_TEMP_MEASUREMENT
    return retVal;
} // end SAC_ERROR_TYPE beginTempMeasurement()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
beginHumidMeasurement(const SAC_FLAGS_TYPE sensorNumberFlags)
{
#ifdef MH_SARC_DEBUG_HUMID_MEASUREMENT
    sarcDebugStream.println(F("Begin humidity measurement"));
    sarcDebugStream.print(F("sensorNumberFlags = 0x"));
    sarcDebugStream.println(sensorNumberFlags, HEX);
#endif // MH_SARC_DEBUG_HUMID_MEASUREMENT
    // command the start of humidity measurement
    SAC_ERROR_TYPE retVal = clearArrayElements(sensorNumberFlags,
                                               m_readerArraySize,
                                               m_lastHumidityArray);
    retVal |= beginMeasurement(sensorNumberFlags,
                               m_humidInProcessFlags,
                               m_humidErrorFlags,
                               HUMI);
    m_humidMeasStartTime = millis();
#ifdef MH_SARC_DEBUG_HUMID_MEASUREMENT
    sarcDebugStream.print(F("m_humidInProcessFlags = 0x"));
    sarcDebugStream.println(m_humidInProcessFlags, HEX);
    sarcDebugStream.print(F("m_humidMeasStartTime "));
    sarcDebugStream.println(m_humidMeasStartTime);
    sarcDebugStream.print(F("beginHumidMeasurement return value = 0x"));
    sarcDebugStream.println(retVal, HEX);
#endif // MH_SARC_DEBUG_HUMID_MEASUREMENT
    return retVal;
} // end SAC_ERROR_TYPE beginHumidMeasurement()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
beginMeasurement(const SAC_FLAGS_TYPE sensorNumberFlags,
                 SAC_FLAGS_TYPE& inProcessFlags,
                 SAC_FLAGS_TYPE& beginErrorFlags,
                 const uint8_t measurementType)
{
    SAC_ERROR_TYPE retVal = 0;
    // clear bit field indicating measurement in progress
    inProcessFlags = 0;
    // clear bit field indicating measurement error
    beginErrorFlags = 0;
    SAC_FLAGS_TYPE sensorSelectBit = 1;
    for (int i=0; i<maxSensors; i++)
    {
        if (sensorSelectBit & sensorNumberFlags)
        {
            m_rawDataArray[i] = 0;
            uint8_t startResult = m_sensorPtrArray[i]->meas(measurementType,
                                  &(m_rawDataArray[i]),
                                  NONBLOCK);
            if (startResult == 0)
            {
                // success, record measurement in progress
                inProcessFlags |= sensorSelectBit;
            }
            else
            {
                // failure, record measurement error
                beginErrorFlags |= sensorSelectBit;
                m_errorFlags |= SAC_COMMAND_ERROR;
                retVal = SAC_COMMAND_ERROR;
            } // end if (startResult == 0)
#ifdef MH_SARC_DEBUG_MEASUREMENT
            sarcDebugStream.print(F("sensor "));
            sarcDebugStream.print(i);
            sarcDebugStream.print(F(" initializeMeasurement startResult "));
            sarcDebugStream.println(startResult);
#endif // MH_SARC_DEBUG_MEASUREMENT
        } // end if (sensorSelectBit & sensorNumberFlags)
        sensorSelectBit <<= 1;
    } // end for (i=0; i<maxSensors; i++)
    return retVal;
} // end int initializeMeasurement()



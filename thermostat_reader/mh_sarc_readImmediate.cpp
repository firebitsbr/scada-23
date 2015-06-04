// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_readImmediate linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_readImmediate windows implementation incomplete"
#else
#error "mh_sarc_readImmediate unknown compilation environment"
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
readTemperatureImmediate(const SAC_FLAGS_TYPE valuesRequested,
                         SAC_FLAGS_TYPE &valuesMeasured)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    retVal |= beginTempMeasurement(valuesRequested);
    delayMicroseconds(m_tempTimeoutMillis);
    retVal |= checkSensorsTemp(valuesMeasured);
    if (valuesMeasured != valuesRequested)
    {
        retVal |= SAC_MEASURE_ERROR;
    } // end if (dataReadyFlags != valuesRequested)
    return retVal;
} // end SAC_ERROR_TYPE readTemperatureImmediate(const SAC_FLAGS_TYPE valuesRequested, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
readHumidityImmediate(const SAC_FLAGS_TYPE valuesRequested,
                      SAC_FLAGS_TYPE &valuesMeasured)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    retVal |= beginHumidMeasurement(valuesRequested);
    delayMicroseconds(m_humidTimeoutMillis);
    valuesMeasured = 0;
    retVal |= checkSensorsHumid(valuesMeasured);
    if (valuesMeasured != valuesRequested)
    {
        retVal |= SAC_MEASURE_ERROR;
    } // end if (dataReadyFlags != valuesRequested)
    return retVal;
} // end SAC_ERROR_TYPE readHumidityImmediate(const SAC_FLAGS_TYPE valuesRequested, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
readAllImmediate(void)
{
    SAC_ERROR_TYPE retVal = 0;
    SAC_FLAGS_TYPE sensorSelectBit = 1;
    for (int i=0; i<maxSensors; i++)
    {
        if (m_sensorActiveMask & sensorSelectBit)
        {
            m_sensorPtrArray[i]->measTemp(&(m_rawDataArray[i]));                // sht.meas(TEMP, &rawData, BLOCK)
            m_lastTemperatureArray[i] = m_sensorPtrArray[i]->calcTemp(m_rawDataArray[i]);
            m_sensorPtrArray[i]->measHumi(&(m_rawDataArray[i]));                // sht.meas(HUMI, &rawData, BLOCK)
            m_lastHumidityArray[i]= m_sensorPtrArray[i]->calcHumi(m_rawDataArray[i], m_lastTemperatureArray[i]);
#ifdef MH_SARC_DEBUG_READ_IMMEDIATE
            SAC_CONVERTED_TYPE dewpoint = m_sensorPtrArray[i]->calcDewpoint(m_lastHumidityArray[i], m_lastTemperatureArray[i]);
            sarcDebugStream.print(F("sensor "));
            sarcDebugStream.println(i);
            sarcDebugStream.print(F("Temperature = "));
            sarcDebugStream.print(m_lastTemperatureArray[i]);
            sarcDebugStream.print(F(" C, Humidity = "));
            sarcDebugStream.print(m_lastHumidityArray[i]);
            sarcDebugStream.print(F(" %, Dewpoint = "));
            sarcDebugStream.print(dewpoint);
            sarcDebugStream.println(F(" C"));
#endif // MH_SARC_DEBUG_READ_IMMEDIATE
            sensorSelectBit <<= 1;
        } // end if (m_sensorActiveMask & sensorSelectBit)
    } // end for (i)
    return retVal;
} // end int readAllImmediate()



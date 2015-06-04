// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_encode_print linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_encode_print windows implementation incomplete"
#else
#error "mh_sarc_encode_print unknown compilation environment"
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
printTemperatures(Stream &serialDevice,
                  SAC_FLAGS_TYPE valuesRequested)
{
    SAC_ERROR_TYPE retVal = 0;
#ifdef MH_SARC_DEBUG_SARC_PRINT
    serialDevice.print(F("Temperature values requested = ( "));
    printSensirionFlagsType(serialDevice, valuesRequested);
    serialDevice.println(F(" )"));
#endif // MH_SARC_DEBUG_SARC_PRINT
    SAC_FLAGS_TYPE valuesToPrint = valuesRequested & m_sensorActiveMask;
#ifdef MH_SARC_DEBUG_SARC_PRINT
    serialDevice.print(F("Temperature values to print  = ( "));
    printSensirionFlagsType(serialDevice, valuesToPrint);
    serialDevice.println(F(" )"));
#endif // MH_SARC_DEBUG_SARC_PRINT
    if (valuesToPrint != 0)
    {
        serialDevice.println(F("Sensor, Temperature (C) "));
        SAC_FLAGS_TYPE sensorSelectBit = 1;
        for (int i=0; i<maxSensors; i++)
        {
#ifdef MH_SARC_DEBUG_SARC_PRINT
            serialDevice.print(F("i = "));
            serialDevice.print(i);
            serialDevice.print(F(", sensorSelectBit = "));
            serialDevice.print(sensorSelectBit);
            serialDevice.print(F(", m_lastTemperatureArray[i] = "));
            serialDevice.println(m_lastTemperatureArray[i]);
#endif // MH_SARC_DEBUG_SARC_PRINT
            if (valuesToPrint & sensorSelectBit)
            {
                serialDevice.print(i);
                serialDevice.print(F(", "));
                serialDevice.println(m_lastTemperatureArray[i]);
            } // end if (valuesToPrint & sensorSelectBit)
            sensorSelectBit <<= 1;
        } // end for (i)
        serialDevice.println(F("end Sensor, Temperature"));
    } // end if (valuesToPrint != 0) {
    return retVal;
} // end printTemperatures(Stream &serialDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
printHumidities(Stream &serialDevice,
                SAC_FLAGS_TYPE valuesRequested)
{
    SAC_ERROR_TYPE retVal = 0;
#ifdef MH_SARC_DEBUG_SARC_PRINT
    serialDevice.print(F("Humidity values requested = ( "));
    printSensirionFlagsType(serialDevice, valuesRequested);
    serialDevice.println(F(" )"));
#endif // MH_SARC_DEBUG_SARC_PRINT
    SAC_FLAGS_TYPE valuesToPrint = valuesRequested & m_sensorActiveMask;
#ifdef MH_SARC_DEBUG_SARC_PRINT
    serialDevice.print(F("Humidity values to print  = ( "));
    printSensirionFlagsType(serialDevice, valuesToPrint);
    serialDevice.println(F(" )"));
#endif // MH_SARC_DEBUG_SARC_PRINT
    if (valuesToPrint != 0)
    {
        serialDevice.println(F("Sensor, Humidity (%) "));
        SAC_FLAGS_TYPE sensorSelectBit = 1;
        for (int i=0; i<maxSensors; i++)
        {
#ifdef MH_SARC_DEBUG_SARC_PRINT
            serialDevice.print(F("i = "));
            serialDevice.print(i);
            serialDevice.print(F(", sensorSelectBit = "));
            serialDevice.print(sensorSelectBit);
            serialDevice.print(F(", m_lastHumidityArray[i] = "));
            serialDevice.println(m_lastHumidityArray[i]);
#endif // MH_SARC_DEBUG_SARC_PRINT
            if (valuesToPrint & sensorSelectBit)
            {
                serialDevice.print(i);
                serialDevice.print(F(", "));
                serialDevice.println(m_lastHumidityArray[i]);
            } // end if (valuesToPrint & sensorSelectBit)
            sensorSelectBit <<= 1;
        } // end for (i)
        serialDevice.println(F("end Sensor, Humidity"));
    } // end if (valuesToPrint != 0) {
    return retVal;
} // end printHumidities(Stream &serialDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
encodeTemperatures(char *buffer,
                   const size_t bufferSize,
                   size_t &bytesEncoded,
                   SAC_FLAGS_TYPE valuesRequested)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // verify input
    // mask for existence of sensors (output is set immediately prior to return)
    SAC_FLAGS_TYPE valuesToPrint = valuesRequested & m_sensorActiveMask;

    measurementTypeEnum measurementType = MH_TEMP_SENSIRION;
    retVal |= encodeDataArray(buffer,
                              bufferSize,
                              bytesEncoded,
                              measurementType,
                              m_errorFlags,
                              valuesRequested,
                              m_tempErrorFlags,
                              m_readerArraySize,
                              m_lastTemperatureArray);
    // note any mismatch in requested and encoded data
    if (valuesToPrint != valuesRequested)
    {
        retVal |= SAC_SENSOR_UNAVAIL;
    } // end if (valuesToPrint != valuesRequested)
    return retVal;
} // end SAC_ERROR_TYPE encodeTemperatures()

// encode humidities
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
encodeHumidities(char *buffer,
                 const size_t bufferSize,
                 size_t &bytesEncoded,
                 SAC_FLAGS_TYPE valuesRequested)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // verify input
    // mask for existence of sensors (output is set immediately prior to return)
    SAC_FLAGS_TYPE valuesToPrint = valuesRequested & m_sensorActiveMask;
    measurementTypeEnum measurementType = MH_HUMI_SENSIRION;
    return encodeDataArray(buffer,
                           bufferSize,
                           bytesEncoded,
                           measurementType,
                           m_errorFlags,
                           valuesRequested,
                           m_humidErrorFlags,
                           m_readerArraySize,
                           m_lastHumidityArray);
    // note any mismatch in requested and encoded data
    if (valuesToPrint != valuesRequested)
    {
        retVal |= SAC_SENSOR_UNAVAIL;
    } // end if (valuesToPrint != valuesRequested)
    return retVal;
} // end SAC_ERROR_TYPE encodeHumidities()


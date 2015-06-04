// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_sensirionArrayReaderClass linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_sensirionArrayReaderClass windows implementation incomplete"
#else
#error "mh_sarc_sensirionArrayReaderClass unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <string.h>
#include <stddef.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>
#include <mh_sarc_sensirionArrayReaderClass.h>
#include <mh_sarc_encodeData.h>

// sarcDebugStream must be defined for all debug blocks
#ifdef ARDUINO
Stream& sarcDebugStream = Serial;
#elif __linux__
#error "linux debug output not defined"
#elif _WIN32
#error "windows debug output not defined"
#else
#error "unknown environment, debug output not defined"
#endif // debug stream


mh_temperatureSensing::sensirionArrayReaderClass::
sensirionArrayReaderClass()
{
    sensirionArrayReaderClass::initializeNoAlloc();
    // initializes base class and extensions for this class
    // without allocation of arrays
} // end sensirionArrayReaderClass


mh_temperatureSensing::sensirionArrayReaderClass::
sensirionArrayReaderClass(const int numSensors,
                          const int *dataPinArrayPtr,
                          const int *clockPinArrayPtr)
{
    sensirionArrayReaderClass::initialize(numSensors,
                                          dataPinArrayPtr,
                                          clockPinArrayPtr);
    // initializes base class and extensions for this class
} // end sensirionArrayReaderClass(const int numSensors, ... )


mh_temperatureSensing::sensirionArrayReaderClass::
~sensirionArrayReaderClass()
{
    sensirionArrayReaderClass::freeMemory();
} // end sensirionArrayReaderClass


unsigned long
mh_temperatureSensing::sensirionArrayReaderClass::
getTempTimeoutMillis(void)
{
    return m_tempTimeoutMillis;
} // end getTempTimeoutMillis(void)


unsigned long
mh_temperatureSensing::sensirionArrayReaderClass::
getHumidTimeoutMillis(void)
{
    return m_humidTimeoutMillis;
} // end getHumidTimeoutMillis(void)


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
clearErrors(void)
{
    SAC_ERROR_TYPE previousErrors = m_errorFlags;
    m_errorFlags = SAC_SUCCESS;
    m_resolutionErrorFlags = 0x00;
    m_tempErrorFlags = 0x00;
    m_humidErrorFlags = 0x00;
    return previousErrors;
} // end SAC_ERROR_TYPE clearErrors()


void
mh_temperatureSensing::sensirionArrayReaderClass::
initializeNoAlloc(void)
{
    // initialize base class
    sensirionArrayClass::initializeNoAlloc();
    // initialze the extensions for this class
    sensirionArrayReaderClass::initializeLocalNoAlloc();
} // end void initializeNoAlloc()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
initialize(const int numSensors, const int *dataPinArrayPtr, const int *clockPinArrayPtr)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // initialize base class
    retVal = sensirionArrayClass::initialize(numSensors, dataPinArrayPtr, clockPinArrayPtr);
    // initialize the extensions for this class
    // members without memory allocation
    sensirionArrayReaderClass::initializeLocalNoAlloc();
    retVal |= sensirionArrayReaderClass::initializeLocalArrays(numSensors);
    return retVal;
} // end initialize(const int numSensors, ... )


void
mh_temperatureSensing::sensirionArrayReaderClass::
initializeLocalNoAlloc(void)
{
    m_tempInProcessFlags = 0;
    m_humidInProcessFlags = 0;
    m_resolutionErrorFlags = 0;
    m_tempErrorFlags = 0;
    m_humidErrorFlags = 0;
    m_tempMeasStartTime = 0;
    m_humidMeasStartTime = 0;
    m_tempTimeoutMillis = highResTempTimeout;
    m_humidTimeoutMillis = highResHumidTimeout;
    m_readerArraySize = 0;
    m_rawDataArray = NULL;
    m_lastTemperatureArray = NULL;
    m_lastHumidityArray = NULL;
} // end void initializeLocalNoAlloc()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
initializeLocalArrays(const int numSensors)
{
    // arrays to store measurement data and times
    // types are per Sensirion.h in Arduino Sensirion library
    m_rawDataArray = new SAC_RAW_TYPE[numSensors];
    m_lastTemperatureArray = new SAC_CONVERTED_TYPE[numSensors];
    m_lastHumidityArray = new SAC_CONVERTED_TYPE[numSensors];
    // if any array allocates set m_readerArraySize to allocated size
    if (    (m_rawDataArray != NULL)         ||
            (m_lastTemperatureArray != NULL) ||
            (m_lastHumidityArray != NULL)         )
    {
        m_readerArraySize = numSensors;
    } // end if ( (m_rawDataArray != NULL) ... )
    // if any array does not allocate, set return to error
    if (    (m_rawDataArray == NULL)         ||
            (m_lastTemperatureArray == NULL) ||
            (m_lastHumidityArray == NULL)         )
    {
        // bad allocation of array
        // m_sensorActiveMask must be set to zero to prevent
        // null pointer access and subsequent segmentation
        // fault
        m_sensorActiveMask = 0;
        m_errorFlags |= (SAC_ERROR_TYPE) SAC_BAD_ARRAY_PTR;
        m_sensorState = SSS_SAFETY_STOP;
    } // end if ( (m_rawDataArray == NULL) ... )
    // if any array allocates
    return m_errorFlags;
} // end initializeLocalArrays()


void
mh_temperatureSensing::sensirionArrayReaderClass::
freeMemory(void)
{
    sensirionArrayReaderClass::freeLocalMemory();
    sensirionArrayClass::freeMemory();
} // end SAC_ERROR_TYPE freeMemory()


void
mh_temperatureSensing::sensirionArrayReaderClass::
freeLocalMemory(void)
{
    if (m_rawDataArray != NULL)
    {
        delete m_rawDataArray;
        m_rawDataArray = NULL;
    } // end if (m_rawDataArray != NULL)
    if (m_lastTemperatureArray != NULL)
    {
        delete m_lastTemperatureArray;
        m_lastTemperatureArray = NULL;
    } // end if (m_lastTemperatureArray != NULL)
    if (m_lastHumidityArray != NULL)
    {
        delete m_lastHumidityArray;
        m_lastHumidityArray = NULL;
    } // end if (m_lastHumidityArray != NULL)
    // m_sensorActiveMask must be set to zero to prevent
    // null pointer access and subsequent segmentation
    // fault
    m_sensorActiveMask = 0;
    m_sensorActiveFlags = m_sensorActiveMask;
} // end SAC_ERROR_TYPE freeLocalMemory()





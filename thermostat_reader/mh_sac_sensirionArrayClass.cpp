// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sac_sensirionArrayClass linux implementation incomplete"
#elif _WIN32
#error "mh_sac_sensirionArrayClass windows implementation incomplete"
#else
#error "mh_sac_sensirionArrayClass unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>

// WARNINGS and notes
// see sensirionArrayClass.h for warnings and notes

// uncomment below for debug output
// #define MH_DEBUG_ALLOCATION

mh_temperatureSensing::sensirionArrayClass::
sensirionArrayClass()
{
    sensirionArrayClass::initializeNoAlloc();
} // end sensirionArrayClass()


mh_temperatureSensing::sensirionArrayClass::
sensirionArrayClass(const int numSensors,
                    const int *dataPinArrayPtr,
                    const int *clockPinArrayPtr)
{
    sensirionArrayClass::initialize(numSensors, dataPinArrayPtr, clockPinArrayPtr);
} // end sensirionArrayClass(const int numSensors, ... )


mh_temperatureSensing::sensirionArrayClass::
~sensirionArrayClass()
{
    sensirionArrayClass::freeMemory();
} // end ~sensirionArrayClass()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayClass::
getErrorFlags(void)
{
    return m_errorFlags;
} // end getErrorFlags()


mh_temperatureSensing::sensirionSensorStateEnum
mh_temperatureSensing::sensirionArrayClass::
getState(void)
{
    return m_sensorState;
} // end getState()


mh_temperatureSensing::SAC_FLAGS_TYPE
mh_temperatureSensing::sensirionArrayClass::
getActiveMask(void)
{
    return m_sensorActiveMask;
} // end getActiveMask(void)


void
mh_temperatureSensing::sensirionArrayClass::
initializeNoAlloc(void)
{
    // re-initialize member variables (a precaution to address warning on
    // non-static data member initializers)
    m_numSensors = 0;
    m_sensorPtrArray = NULL;
    m_sensorActiveFlags = 0x00;
    m_sensorActiveMask = 0x00;
    m_sensorState = SSS_UNINIT;
    m_errorFlags = SAC_SUCCESS;
} // end initializeNoAlloc()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayClass::
initialize(const int numSensors,
           const int *dataPinArrayPtr,
           const int *clockPinArrayPtr)
{
    // initialize non-allocated member variables
    sensirionArrayClass::initializeNoAlloc();
    // check input to limit number of devices
    int devicesToAllocate = numSensors;
    if (devicesToAllocate < 0)
    {
        devicesToAllocate = 0;
        m_errorFlags |= (SAC_ERROR_TYPE) SAC_BAD_COUNT;
        m_errorFlags |= (SAC_ERROR_TYPE) SAC_BAD_ARRAY_PTR;
        m_sensorState = SSS_SAFETY_STOP;
    } // end if (devicesToAllocate < 0)
    if (devicesToAllocate > maxSensors)
    {
        devicesToAllocate = maxSensors;
        m_errorFlags |= SAC_BAD_COUNT;
    } // end if (devicesToAllocate > maxSensors)
    // allocate pointer array and sensor objects
    if (devicesToAllocate > 0)
    {
        // allocate memory for pointer array
        // note - no exception handling is default for Arduino (compiled -fno-exceptions)
        m_sensorPtrArray = new Sensirion* [devicesToAllocate];
        // create Sensirion objects and populate pointer array
        int sensorsAllocated = 0;
        if (m_sensorPtrArray != NULL)
        {
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<devicesToAllocate; i++)
            {
                m_sensorPtrArray[i] = new Sensirion(dataPinArrayPtr[i], clockPinArrayPtr[i]);
                // check for bad allocation
                if (m_sensorPtrArray[i] != NULL)
                {
                    m_sensorActiveMask |= sensorSelectBit;
                    sensorsAllocated++;
                }
                else
                {
                    m_errorFlags |= (SAC_ERROR_TYPE) SAC_BAD_SENSOR_PTR;
                    m_sensorState = SSS_SAFETY_STOP;
                } // end sensirion object allocation
                sensorSelectBit <<= 1;
            } // end for (i=0; i<devicesToAllocate; i++)
            // set the bit field of active sensors to the set of
            // sensors properly allocated
            m_sensorActiveFlags = m_sensorActiveMask;
            m_numSensors = sensorsAllocated;
        }
        else
        {
            // bad allocation of pointer array
            m_errorFlags |= (SAC_ERROR_TYPE) SAC_BAD_ARRAY_PTR;
            m_sensorState = SSS_SAFETY_STOP;
            // m_sensorActiveMask must be zero for any bad allocation
            // m_sensorActiveMask is set to zero in initializeNoAlloc()
            // prior to calling this function
        } // end if (m_sensorPtrArray != NULL)
        // test for success
    } // end if (devicesToAllocate > 0)
    if (m_sensorState != SSS_SAFETY_STOP)
    {
        m_sensorState = SSS_INITIALIZED;
    } // end if (m_sensorState != SSS_SAFETY_STOP)
    // return status of initialization
#ifdef MH_DEBUG_ALLOCATION
    Serial.print(sensorsAllocated);
    Serial.print(" sensors allocated, active mask ");
    Serial.println(m_sensorActiveMask, HEX);
#endif // MH_DEBUG_ALLOCATION
    return m_errorFlags;
} // end int initialize()


void
mh_temperatureSensing::sensirionArrayClass::
freeMemory(void)
{
    if (m_sensorPtrArray != NULL)
    {
        for (int i=0; i<m_numSensors; i++)
        {
            if (m_sensorPtrArray[i] != NULL)
            {
                delete m_sensorPtrArray[i];
                m_sensorPtrArray[i] = NULL;
            } // end if (m_sensorPtrArray[i] != NULL)
        } // end for (i=0; i<m_numSensors; i++)
        delete[] m_sensorPtrArray;
        m_sensorPtrArray = NULL;
    } // end if (m_sensorPtrArray != NULL)
    m_sensorActiveMask = 0;
    m_sensorActiveFlags = m_sensorActiveMask;
} // end SAC_ERROR_TYPE freeMemory()


// clearErrors() returns previous error state
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayClass::
clearErrors(void)
{
    SAC_ERROR_TYPE previousErrors = m_errorFlags;
    m_errorFlags = SAC_SUCCESS;
    return previousErrors;
} // end SAC_ERROR_TYPE clearErrors()


// setState() returns previous value of state
mh_temperatureSensing::sensirionSensorStateEnum
mh_temperatureSensing::sensirionArrayClass::
setState(sensirionSensorStateEnum newState)
{
    sensirionSensorStateEnum lastState = m_sensorState;
    m_sensorState = newState;
    return lastState;
} // end sensirionSensorStateEnum setState()



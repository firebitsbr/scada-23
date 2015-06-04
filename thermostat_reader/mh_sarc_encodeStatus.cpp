// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_encodeStatus linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_encodeStatus windows implementation incomplete"
#else
#error "mh_sarc_encodeStatus unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <string.h>
#include <stddef.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>
#include <mh_sarc_sensirionArrayReaderClass.h>
#include <mh_sarc_encodeData.h>


// Status format
//  Total data size
//  Reader state value
//  Sensors available flags
//  Base class error flags
//  Resolution error flags
//  Temperature error flags
//  Humidity error flags
const mh_temperatureSensing::SAC_SIZE_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
statusMessageLength = sizeof(SAC_SIZE_TYPE) +
                      sizeof(SAC_STATE_TYPE) +
                      sizeof(SAC_FLAGS_TYPE) +
                      sizeof(SAC_ERROR_TYPE) +
                      sizeof(SAC_FLAGS_TYPE);


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
encodeStatus(char *bufferPtr,
             const size_t bufferSize,
             size_t &bytesEncoded)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    if (bufferPtr == NULL)
    {
        retVal |= SAC_BAD_ARRAY_PTR;
    } // end if (buffer == NULL)
    if (bufferSize <= 0)
    {
        retVal |= SAC_BAD_COUNT;
    } // end if (bufferSize <= 0)
    // test sufficiency of buffer length
    // message length is pre-calculated as a static constant
    const SAC_SIZE_TYPE& messageLength = sensirionArrayReaderClass::statusMessageLength;
    if (bufferSize < messageLength)
    {
        retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
    } // end (bufferSize < messageLength)
    // end of input testing
    // if valid input, encode data
    if (retVal == SAC_SUCCESS)
    {
        bytesEncoded = messageLength;
        char *workPtr = bufferPtr;
        // message length
        memcpy((void*) workPtr, (void*) &messageLength, sizeof(messageLength));
        workPtr += sizeof(messageLength);
        // sensor state
        SAC_STATE_TYPE sensorState = (SAC_STATE_TYPE) m_sensorState;
        memcpy((void*) workPtr, (void*) &sensorState, sizeof(sensorState));
        workPtr += sizeof(sensorState);
        // active sensors
        memcpy((void*) workPtr, (void*) &m_sensorActiveMask, sizeof(m_sensorActiveMask));
        workPtr += sizeof(m_sensorActiveMask);
        // error flags
        memcpy((void*) workPtr, (void*) &m_errorFlags, sizeof(m_errorFlags));
        workPtr += sizeof(m_errorFlags);
        // sensor error flags
        SAC_FLAGS_TYPE sensorErrors = m_resolutionErrorFlags &
                                      m_tempErrorFlags &
                                      m_humidErrorFlags;
        memcpy((void*) workPtr, (void*) &sensorErrors, sizeof(sensorErrors));
    }
    else
    {
        bytesEncoded = 0;
        // set buffer to zeros
        if (bufferSize > 0)
        {
            memset(bufferPtr, 0, bufferSize);
        } // end if (bufferSize > 0)
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE encodeStatus()


// decode state and errors errors
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
decodeStatus(char *bufferPtr,
             const size_t bufferSize,
             size_t &bytesUsed,
             SAC_SIZE_TYPE &statedBytes,
             SAC_STATE_TYPE &sensorState,
             SAC_FLAGS_TYPE &activeMask,
             SAC_ERROR_TYPE &sacErrors,
             SAC_FLAGS_TYPE &sensorErrors)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // initial values for arguments passed by reference
    bytesUsed = 0;
    statedBytes = 0;
    sensorState = SAC_UNINIT;
    activeMask = 0;
    sacErrors = SAC_SUCCESS;
    sensorErrors = 0;
    // check input
    if (bufferPtr == NULL)
    {
        retVal |= SAC_BAD_ARRAY_PTR;
    } // end if (bufferPtr == NULL)
    if (bufferSize <= 0)
    {
        retVal |= SAC_BAD_COUNT;
    } // end if (bufferSize <= 0)
    // test sufficiency of buffer length
    // message length is pre-calculated as a static constant
    if (bufferSize < sensirionArrayReaderClass::statusMessageLength)
    {
        retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
    } // end (bufferSize < sensirionArrayReaderClass::statusMessageLength)
    // end of input testing
    // if valid input, decode data
    if (retVal == SAC_SUCCESS)
    {
        char *workPtr = bufferPtr;
        // message length
        memcpy((void*) &statedBytes, (void*) workPtr, sizeof(statedBytes));
        workPtr += sizeof(statedBytes);
        // sensor state
        memcpy((void*) &sensorState, (void*) workPtr, sizeof(sensorState));
        workPtr += sizeof(sensorState);
        // active sensors
        memcpy((void*) &activeMask, (void*) workPtr, sizeof(activeMask));
        workPtr += sizeof(activeMask);
        // error flags
        memcpy((void*) &sacErrors, (void*) workPtr, sizeof(sacErrors));
        workPtr += sizeof(sacErrors);
        // sensor error flags
        memcpy((void*) &sensorErrors, (void*) workPtr, sizeof(sensorErrors));
        // check translated values
        if (statedBytes != sensirionArrayReaderClass::statusMessageLength)
        {
            retVal |= SAC_BAD_COUNT;
        } // end if (statedBytes != sensirionArrayReaderClass::statusMessageLength)
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE decodeStatus()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
printStatus(Stream &serialDevice)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    serialDevice.print(F("Sensor state = "));
    retVal |= printSensirionSensorStateEnum(serialDevice, m_sensorState);
    serialDevice.println();
    serialDevice.print(F("Available sensors: "));
    retVal |= printSensirionFlagsType(serialDevice, m_sensorActiveMask);
    serialDevice.println();
    serialDevice.print(F("Error state: "));
    retVal |= printSacErrorsType(serialDevice, m_errorFlags);
    serialDevice.println();
    return retVal;
} // end printStatus(Stream &serialDevice, ... )


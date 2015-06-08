#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
setOutgoingSensorType(const cicSensorTypeEnum sensorEnumVal,
                      int &dataOffset,
                      int &maxDataBytes)
// sets outgoing command value and value of m_payloadByteCounter
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set outgoing message type to data
    m_outgoingMessageType = CIC_MSG_DATA;
    // payload can be used only if m_payloadBufferPtr != NULL
    // and m_payloadBufferSize > size of data type identifier
    CIC_SENSOR_TYPE sensorType = (CIC_SENSOR_TYPE) sensorEnumVal;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    } // end if (m_payloadBufferPtr != NULL)
    if ( (m_payloadBufferSize < sizeof(sensorType)) ||
            (maxPayloadSize < sizeof(sensorType))      )
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
    } // end if ((m_payloadBufferSize < sizeof(sensorType)) || ... )
    if (retVal == CIC_SUCCESS)
    {
        // entire payload buffer is available for use as data
        // because outgoing header is constructed using member variables
        // first N bytes are the sensor type value
        memcpy((void*) m_payloadBufferPtr,
               (void*) &sensorType,
               sizeof(sensorType));
        m_payloadByteCounter = sizeof(sensorType);
        // remaining bytes are available for command data
        dataOffset = sizeof(sensorType);
        maxDataBytes = min(m_payloadBufferSize, maxPayloadSize) - sizeof(sensorType);
    }
    else
    {
        dataOffset = 0;
        maxDataBytes = 0;
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE setOutgoingSensorType(const cicSensorTypeEnum sensorEnumVal, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
checkDataAvailable(void)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if ( (m_payloadBufferSize < (framingSize + sizeof(CIC_DATAREP_TYPE)))    ||
            (m_receivedBytesRead < (framingSize + sizeof(CIC_DATAREP_TYPE)))    )
    {
        retVal |= CIC_ERR_BAD_COUNT;
    }
    return retVal;
} // end checkDataAvailable()


// decode data
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeData(const char *bufferPtr,
           const size_t bufferSize,
           cicSensorTypeEnum &sensorTypeEnum,
           int &dataOffset,
           int &dataBytes)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // initial values of parameters passed by reference
    sensorTypeEnum = CST_NONE;
    dataOffset = 0;
    dataBytes = 0;
    // check input
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDDATA
        cicDebugStream.println(F("decodeData() error, bufferPtr == NULL"));
#endif // MH_CIC_DEBUG_RESPONDDSATA
    }
    // variable CIC_SENSOR_TYPE sensorType is the encoded representation
    // of sensor type
    CIC_SENSOR_TYPE sensorType = 0;
    if (bufferSize < sizeof(sensorType))
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDDATA
        cicDebugStream.print(F("decodeData() error, insufficient memory - "));
        cicDebugStream.print(bufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(sizeof(sensorType));
#endif // MH_CIC_DEBUG_RESPONDDATA
    } // end if (bufferSize < sizeof(sensorType))
    if (retVal == CIC_SUCCESS)
    {
        // first N bytes are the sensor type value
        // remaining bytes are encoded data
        memcpy((void*) &sensorType,
               (void*) bufferPtr,
               sizeof(sensorType));
        sensorTypeEnum = (cicSensorTypeEnum) sensorType;
        // remaining bytes are available for command data
        dataOffset = sizeof(sensorType);
        dataBytes = bufferSize - dataOffset;
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeData(char *buffer, ... )


// decode data
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeData(cicSensorTypeEnum &sensorTypeEnum,
           int &dataOffset,
           int &dataBytes)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDDATA
        cicDebugStream.println(F("decodeData() error, m_payloadBufferPtr == NULL"));
#endif // MH_CIC_DEBUG_RESPONDDSATA
    }
    // note minimumSize is dependent on the representation of sensor type
    // in the expanded, static version of decodeData()
    CIC_SENSOR_TYPE sensorType = CST_NONE;
    const size_t minimumSize = headerSize + sizeof(sensorType);
    if (m_payloadBufferSize < minimumSize)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDDATA
        cicDebugStream.print(F("decodeData() error, insufficient memory - "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(minimumSize);
#endif // MH_CIC_DEBUG_RESPONDDATA
    }
    if (m_payloadByteCounter < minimumSize)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDDATA
        cicDebugStream.print(F("decodeData() error, insufficient data available - "));
        cicDebugStream.print(m_payloadByteCounter);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(minimumSize);
#endif // MH_CIC_DEBUG_RESPONDDATA
    }
    if (retVal == CIC_SUCCESS)
    {
        char* workPtr = m_payloadBufferPtr + headerSize;
        size_t bytesAvailable = min(m_payloadBufferSize, m_payloadByteCounter);
        size_t bufferSize = bytesAvailable - headerSize;
        retVal |= decodeData(workPtr,
                             bufferSize,
                             sensorTypeEnum,
                             dataOffset,
                             dataBytes);
        // add header size to offset
        // compensates for subtraction from bufferSize above
        dataOffset += headerSize;
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeData(cicSensorTypeEnum &sensorType, ... )



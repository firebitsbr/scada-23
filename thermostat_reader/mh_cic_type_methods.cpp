#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondType(const CIC_SENSOR_TYPE sensorType,
            const MH_VERSION_TYPE sensorVersion,
            const CIC_SIZE_TYPE numSensors,
            const CIC_FLAGS_TYPE sensorFlags)
{
    if (m_serialBasePtr != NULL)
    {
        return respondType(*m_serialBasePtr,
                           sensorType,
                           sensorVersion,
                           numSensors,
                           sensorFlags);
    }
    else
    {
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.println(F("respondType() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_RESPONDTYPE
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE respondType(const CIC_SENSOR_TYPE sensorType, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondType(Stream &serialDevice,
            const CIC_SENSOR_TYPE sensorType,
            const MH_VERSION_TYPE sensorVersion,
            const CIC_SIZE_TYPE numSensors,
            const CIC_FLAGS_TYPE sensorFlags)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // check calling parameters
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.println(F("Error - cic.respondType() bad pointer"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    if (m_payloadBufferSize < typeMessagePayloadSize)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.print(F("Error - cic.respondType() insufficient memory, "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" of "));
        cicDebugStream.print(typeMessagePayloadSize);
        cicDebugStream.println(F(" bytes needed"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    if (retVal == CIC_SUCCESS)
    {
        size_t bytesRemaining = m_payloadBufferSize;
        char* workPtr = m_payloadBufferPtr;
        // use defined constant for MH_VERSION_TYPE size
        // size of data differs from size of pointer
//        CIC_SIZE_TYPE pldSiz = (CIC_SIZE_TYPE) payloadSize;
//        memcpy((void*) workPtr, (void*) &pldSiz, sizeof(pldSiz));
//        workPtr += sizeof(pldSiz);
//        bytesRemaining -= sizeof(pldSiz);
        // encode CIC version number
        { // code block encode CIC version
            size_t bytesUsed = 0;
            int encRet = encodeVersionNumber(workPtr,
                                             bytesRemaining,
                                             bytesUsed,
                                             versionNumber);
            if (encRet != 0)
            {
                retVal |= CIC_OTHER_ERROR;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
            cicDebugStream.println(F("Error - cic.respondType() error encoding version"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
            } // end if (encRet != 0)
            workPtr += bytesUsed;
            bytesRemaining -= bytesUsed;
        } // end code block encode CIC version
        // encode sensor type
        memcpy((void*) workPtr, (void*) &sensorType, sizeof(sensorType));
        workPtr += sizeof(sensorType);
        bytesRemaining -= sizeof(sensorType);
        // encode sensor version
        { // code block encode sensor version
            size_t bytesUsed = 0;
            int encRet = encodeVersionNumber(workPtr,
                                             bytesRemaining,
                                             bytesUsed,
                                             sensorVersion);
            if (encRet != 0)
            {
                retVal |= CIC_OTHER_ERROR;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
            cicDebugStream.println(F("Error - cic.respondType() error encoding sensor version"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
            } // end if (encRet != 0)
            workPtr += bytesUsed;
        } // end code block encode sensor version
        memcpy((void*) workPtr, (void*) &numSensors, sizeof(numSensors));
        workPtr += sizeof(numSensors);
        memcpy((void*) workPtr, (void*) &sensorFlags, sizeof(sensorFlags));
        // set payload counter and send message
        m_payloadByteCounter = typeMessagePayloadSize;
        int bytesWritten = 0;
        retVal |= sendMessage(serialDevice,
                              bytesWritten,
                              m_receivedSourceAddress,
                              CIC_MSG_TYPE);
        // check for complete write
        const int messageSize = framingSize + typeMessagePayloadSize;
        if (bytesWritten != messageSize)
        {
            retVal |= CIC_COMM_ERROR;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
            cicDebugStream.print(F("Error - cic.respondType() write error, "));
            cicDebugStream.print(bytesWritten);
            cicDebugStream.print(F(" of "));
            cicDebugStream.print(messageSize);
            cicDebugStream.println(F(" bytes written"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
        } // end if (bytesWritten != typeMessagePayloadSize)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE respondType(Stream &serialDevice, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeType(MH_VERSION_TYPE &cicVersion,
           CIC_SENSOR_TYPE &sensorType,
           MH_VERSION_TYPE &sensorVersion,
           CIC_SIZE_TYPE &numSensors,
           CIC_FLAGS_TYPE &sensorFlags)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // check calling parameters
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDRESPONDTYPE
        cicDebugStream.println(F("Error - cic.decodeType() bad pointer"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    const size_t sizeNeeded = headerSize + typeMessagePayloadSize;
    if (m_payloadBufferSize < sizeNeeded)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.print(F("Error - decodeType() insufficient memory, "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" of "));
        cicDebugStream.print(sizeNeeded);
        cicDebugStream.println(F(" bytes needed"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    if (retVal == CIC_SUCCESS)
    {
        char* workPtr = m_payloadBufferPtr + headerSize;
        size_t bytesUsed = 0;
        size_t bufferSize = m_payloadBufferSize - headerSize;
        retVal |= decodeType(workPtr,
                             bufferSize,
                             bytesUsed,
                             cicVersion,
                             sensorType,
                             sensorVersion,
                             numSensors,
                             sensorFlags);
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE decodeType(MH_VERSION_TYPE &cicVersion, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeType(const char *bufferPtr,
           const size_t bufferSize,
           size_t &bytesUsed,
           MH_VERSION_TYPE &cicVersion,
           CIC_SENSOR_TYPE &sensorType,
           MH_VERSION_TYPE &sensorVersion,
           CIC_SIZE_TYPE &numSensors,
           CIC_FLAGS_TYPE &sensorFlags)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set initial values of parameters passed by reference
    bytesUsed = 0;
    // check calling parameters
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.println(F("Error - cic.decodeType() bad pointer"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    const size_t sizeNeeded = typeMessagePayloadSize;
    if (bufferSize < sizeNeeded)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDTYPE
        cicDebugStream.print(F("Error - decodeType() insufficient memory, "));
        cicDebugStream.print(bufferSize);
        cicDebugStream.print(F(" of "));
        cicDebugStream.print(sizeNeeded);
        cicDebugStream.println(F(" bytes needed"));
#endif //  MH_CIC_DEBUG_RESPONDTYPE
    }
    if (retVal == CIC_SUCCESS)
    {
        bytesUsed = typeMessagePayloadSize;
        char* workPtr = (char*) bufferPtr;
        // CIC version
        memcpy((void*) &cicVersion, (void*) workPtr, MH_VERSION_TYPE_SIZE);
        workPtr += MH_VERSION_TYPE_SIZE;
        // sensor type
        memcpy((void*) &sensorType, (void*) workPtr, sizeof(sensorType));
        workPtr += sizeof(sensorType);
        // sensor version
        memcpy((void*) &sensorVersion, (void*) workPtr, MH_VERSION_TYPE_SIZE);
        workPtr += MH_VERSION_TYPE_SIZE;
        // number of sensors
        memcpy((void*) &numSensors, (void*) workPtr, sizeof(numSensors));
        workPtr += sizeof(numSensors);
        // sensor flags
        memcpy((void*) &sensorFlags, (void*) workPtr, sizeof(sensorFlags));
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE decodeType(char bufferPtr, ... )




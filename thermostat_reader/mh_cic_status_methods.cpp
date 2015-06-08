#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>

mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondStatus(const size_t payloadSize)
{
    if (m_serialBasePtr != NULL)
    {
        return respondStatus(*m_serialBasePtr,
                             payloadSize);
    }
    else
    {
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
        cicDebugStream.println(F("respondStatus() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_RESPONDSTATUS
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE respondStatus(const size_t payloadSize)


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondStatus(Stream &serialDevice,
              const size_t payloadSize)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
        cicDebugStream.println(F("Error - cic.respondStatus() bad pointer"));
#endif //  MH_CIC_DEBUG_RESPONDSTATUS
    }
    if (m_payloadBufferSize < payloadSize)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
        cicDebugStream.print(F("Error - cic.respondStatus() insufficient memory, "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" of "));
        cicDebugStream.print(payloadSize);
        cicDebugStream.println(F(" bytes needed"));
#endif //  MH_CIC_DEBUG_RESPONDSTATUS
    }
    if (retVal == CIC_SUCCESS)
    {
        const int messageSize = framingSize + payloadSize;
        m_payloadByteCounter = payloadSize;
        int bytesWritten = 0;
        retVal |= sendMessage(serialDevice,
                              bytesWritten,
                              m_receivedSourceAddress,
                              CIC_MSG_STATUS);
        if (bytesWritten != messageSize)
        {
            retVal |= CIC_COMM_ERROR;
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
            cicDebugStream.print(F("Error - cic.respondStatus() write error, "));
            cicDebugStream.print(bytesWritten);
            cicDebugStream.print(F(" of "));
            cicDebugStream.print(messageSize);
            cicDebugStream.println(F(" bytes written"));
#endif //  MH_CIC_DEBUG_RESPONDSTATUS
        } // end if (bytesWritten != messageSize)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE respondStatus(Stream &serialDevice, ... )


// encode state and errors errors
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
encodeStatus(char *buffer,
             const size_t bufferSize,
             size_t &bytesEncoded)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (buffer == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    } // end if (buffer == NULL)
    if (bufferSize <= 0)
    {
        retVal |= CIC_ERR_BAD_COUNT;
    } // end if (bufferSize <= 0)
    // test sufficiency of buffer length
    // message length is pre-calculated as a static constant
    const CIC_SIZE_TYPE& messageLength = commandInterfaceClass::statusMessageLength;
    if (bufferSize < messageLength)
    {
        retVal |= (CIC_ERROR_TYPE) CIC_ERR_INSUFF_MEMORY;
    } // end (bufferSize < messageLength)
    // end of input testing
    // if valid input, encode data
    if (retVal == CIC_SUCCESS)
    {
        bytesEncoded = messageLength;
        char *workPtr = buffer;
        // message length
        memcpy((void*) workPtr, (void*) &messageLength, sizeof(messageLength));
        workPtr += sizeof(messageLength);
        // current state
        CIC_STATE_TYPE currentState = (CIC_STATE_TYPE) m_cicState;
        memcpy((void*) workPtr, (void*) &currentState, sizeof(currentState));
        workPtr += sizeof(currentState);
        // error flags
        memcpy((void*) workPtr, (void*) &m_errorFlags, sizeof(m_errorFlags));
        workPtr += sizeof(m_errorFlags);
        // comm error flags
        memcpy((void*) workPtr, (void*) &m_readErrors, sizeof(m_readErrors));
    }
    else
    {
        bytesEncoded = 0;
        // set buffer to zeros
        if (bufferSize > 0)
        {
            memset(buffer, 0, bufferSize);
        } // end if (bufferSize > 0)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE encodeStatus()


// decode state and errors errors
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeStatus(CIC_SIZE_TYPE &statedBytes,
             CIC_STATE_TYPE &cicState,
             CIC_ERROR_TYPE &cicErrors,
             COM_ERROR_TYPE &commErrors)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
        cicDebugStream.println(F("decodeStatus() error, m_payloadBufferPtr == NULL"));
#endif // MH_CIC_DEBUG_RESPONDSTATUS
    }
    if (m_payloadBufferSize < commandInterfaceClass::statusMessageLength)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_RESPONDSTATUS
        cicDebugStream.print(F("decodeStatus() error, insufficient memory - "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(commandInterfaceClass::statusMessageLength);
#endif // MH_CIC_DEBUG_RESPONDSTATUS
    }
    if (retVal == CIC_SUCCESS)
    {
        char* workPtr = m_payloadBufferPtr + headerSize;
        size_t bytesUsed = 0;
        size_t bufferSize = m_payloadBufferSize - headerSize;
        retVal |= decodeStatus(workPtr,
                               bufferSize,
                               bytesUsed,
                               statedBytes,
                               cicState,
                               cicErrors,
                               commErrors);
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeStatus(CIC_SIZE_TYPE &statedBytes, ... )


// decode state and errors errors
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeStatus(const char *bufferPtr,
             const size_t bufferSize,
             size_t &bytesUsed,
             CIC_SIZE_TYPE &statedBytes,
             CIC_STATE_TYPE &cicState,
             CIC_ERROR_TYPE &cicErrors,
             COM_ERROR_TYPE &commErrors)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // initial values for arguments passed by reference
    bytesUsed = 0;
    statedBytes = 0;
    cicState = CIC_UNINIT;
    cicErrors = CIC_SUCCESS;
    commErrors = COMM_SUCCESS;
    // check input
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if (bufferSize <= 0)
    {
        retVal |= CIC_ERR_BAD_COUNT;
    }
    // test sufficiency of buffer length
    // message length is pre-calculated as a static constant
    if (bufferSize < commandInterfaceClass::statusMessageLength)
    {
        retVal |= (CIC_ERROR_TYPE) CIC_ERR_INSUFF_MEMORY;
    } // end (bufferSize < commandInterfaceClass::statusMessageLength)
    // end of input testing
    // if valid input, decode data
    if (retVal == CIC_SUCCESS)
    {
        bytesUsed = commandInterfaceClass::statusMessageLength;
        char *workPtr = (char*) bufferPtr;
        // message length
        memcpy((void*) &statedBytes, (void*) workPtr, sizeof(statedBytes));
        workPtr += sizeof(statedBytes);
        // current state
        memcpy((void*) &cicState, (void*) workPtr, sizeof(cicState));
        workPtr += sizeof(cicState);
        // error flags
        memcpy((void*) &cicErrors, (void*) workPtr, sizeof(cicErrors));
        workPtr += sizeof(cicErrors);
        // comm error flags
        memcpy((void*) &commErrors, (void*) workPtr, sizeof(commErrors));
        // check translated values
        if (statedBytes != commandInterfaceClass::statusMessageLength)
        {
            retVal |= CIC_ERR_BAD_COUNT;
        } // end if (statedBytes != commandInterfaceClass::statusMessageLength)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE decodeStatus()



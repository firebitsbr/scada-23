#include <stdint.h>
#include <stddef.h>

#include <mh_versionNumberType.h>
#include <mh_arduinoTimeFunctions.h>
#include <mh_arduinoPinHandler.h>
#include <mh_arduinoSerialTypes.h>
#include <mh_serialHandlerClass.h>
#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>

using mh_timeFunctions::microsElapsed;
using mh_timeFunctions::HALF_MAX_UNSIGNED_LONG;
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondAck(void)
{
    if (m_serialBasePtr != NULL)
    {
        return respondAck(*m_serialBasePtr);
    }
    else
    {
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.println(F("respondAck() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_ACK
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE respondAck()

mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondAck(Stream &serialDevice)
{
#ifdef MH_CIC_DEBUG_ACK
    cicDebugStream.println(F("responding ACK"));
#endif // MH_CIC_DEBUG_NAK
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    const int ackSize = framingSize + sizeof(CIC_MSG_NUM_TYPE);
    int bytesWritten = 0;
    CIC_MSG_NUM_TYPE messageNumberOut = m_receivedMessageNumber;
    retVal |= sendMessage(serialDevice,
                          bytesWritten,
                          m_receivedSourceAddress,
                          CIC_MSG_ACK,
                          sizeof(messageNumberOut),
                          (char*) &messageNumberOut);
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.print(F("respondAck() "));
        cicDebugStream.print(bytesWritten);
        cicDebugStream.println(F(" bytes written"));
#endif // MH_CIC_DEBUG_ACK
    if (bytesWritten != ackSize)
    {
        retVal |= CIC_COMM_ERROR;
    }
    return retVal;
} // end respondAck()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondNak(const COM_ERROR_TYPE commErrors)
{
    if (m_serialBasePtr != NULL)
    {
        return respondNak(*m_serialBasePtr,
                          commErrors);
    }
    else
    {
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.println(F("respondNak() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_NAK
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE respondAck()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondNak(Stream &serialDevice,
           const COM_ERROR_TYPE commErrors)
{
#ifdef MH_CIC_DEBUG_NAK
    cicDebugStream.println(F("responding NAK"));
#endif // MH_CIC_DEBUG_NAK
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.println(F("respondNak() error, m_payloadBufferPtr == NULL"));
#endif // MH_CIC_DEBUG_NAK
    }
    if (m_payloadBufferSize < (sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE)))
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.print(F("respondNak() error, insufficient memory - "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println((sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE)));
#endif // MH_CIC_DEBUG_NAK
    }
    if (retVal == CIC_SUCCESS)
    {
        const size_t payloadSize = sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE);
        const int messageSize = framingSize + payloadSize;
        char* workPtr = m_payloadBufferPtr;
        CIC_MSG_NUM_TYPE messageNumberOut = m_receivedMessageNumber;
        memcpy((void*) workPtr, (void*) &messageNumberOut, sizeof(CIC_MSG_NUM_TYPE));
        workPtr += sizeof(CIC_MSG_NUM_TYPE);
        memcpy((void*) workPtr, (void*) &commErrors, sizeof(COM_ERROR_TYPE));
        m_payloadByteCounter = payloadSize;
        int bytesWritten = 0;
        retVal |= sendMessage(serialDevice,
                              bytesWritten,
                              m_receivedSourceAddress,
                              CIC_MSG_NAK,
                              payloadSize,
                              m_payloadBufferPtr);
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.print(F("respondNak() "));
        cicDebugStream.print(bytesWritten);
        cicDebugStream.println(F(" bytes written"));
#endif // MH_CIC_DEBUG_NAK
        if (bytesWritten != messageSize)
        {
            retVal |= CIC_COMM_ERROR;
        } // end if (bytesWritten != messageSize)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end respondNak()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeAck(CIC_MSG_NUM_TYPE &messageNumber)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.println(F("decodeAck() error, m_payloadBufferPtr == NULL"));
#endif // MH_CIC_DEBUG_ACK
    }
    size_t sizeNeeded = headerSize + sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE);
    if (m_payloadBufferSize < sizeNeeded)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.print(F("decodeAck() error, insufficient memory - "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(sizeNeeded);
#endif // MH_CIC_DEBUG_ACK
    }
    if (retVal == CIC_SUCCESS)
    {
        char* workPtr = m_payloadBufferPtr + headerSize;
        size_t bytesUsed = 0;
        size_t bufferSize = m_payloadBufferSize - headerSize;
        retVal |= decodeAck(workPtr,
                            bufferSize,
                            bytesUsed,
                            messageNumber);
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeAck(CIC_MSG_NUM_TYPE &messageNumber, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondUnable(const COM_ERROR_TYPE commErrors)
{
    if (m_serialBasePtr != NULL)
    {
        return respondUnable(*m_serialBasePtr,
                             commErrors);
    }
    else
    {
#ifdef MH_CIC_DEBUG_RESPONDUNABLE
        cicDebugStream.println(F("respondUnable() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_RESPONDUNABLE
        return CIC_OTHER_ERROR;
    } // end if (m_serialBasePtr != NULL)
} // end CIC_ERROR_TYPE respondUnable(const COM_ERROR_TYPE commErrors)


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondUnable(Stream &serialDevice,
              const COM_ERROR_TYPE commErrors)
{
    return respondNak(serialDevice, (commErrors | COM_UNABLE));
#ifdef MH_CIC_DEBUG_RESPONDUNABLE
            cicDebugStream.println(F("Error - cic.respondUnable() responded NAK(unable)"));
#endif //  MH_CIC_DEBUG_RESPONDUNABLE
} // end CIC_ERROR_TYPE respondUnable()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeAck(const char *bufferPtr,
          const size_t bufferSize,
          size_t &bytesUsed,
          CIC_MSG_NUM_TYPE &messageNumber)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // initial value of parameters passed by reference
    bytesUsed = 0;
    messageNumber = 0;
    // check input
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.println(F("decodeAck() error, bufferPtr == NULL"));
#endif // MH_CIC_DEBUG_ACK
    }
    if (bufferSize < sizeof(CIC_MSG_NUM_TYPE))
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.print(F("decodeAck() error, insufficient memory - "));
        cicDebugStream.print(bufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(sizeof(CIC_MSG_NUM_TYPE));
#endif // MH_CIC_DEBUG_ACK
    }
    if (retVal == CIC_SUCCESS)
    {
        bytesUsed = sizeof(CIC_MSG_NUM_TYPE);
        memcpy((void*) &messageNumber, (void*) bufferPtr, sizeof(CIC_MSG_NUM_TYPE));
#ifdef MH_CIC_DEBUG_ACK
        cicDebugStream.print(F("decodeAck() message accepted "));
        cicDebugStream.println(messageNumber);
        cicDebugStream.println();
#endif // MH_CIC_DEBUG_ACK
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeAck()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeNak(CIC_MSG_NUM_TYPE &messageNumber,
          COM_ERROR_TYPE &commErrors)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    size_t sizeNeeded = headerSize + sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE);
    if (m_payloadBufferSize < sizeNeeded)
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.print(F("decodeNak() error, insufficient memory - "));
        cicDebugStream.print(m_payloadBufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println(sizeNeeded);
#endif // MH_CIC_DEBUG_NAK
    }
    if (retVal == CIC_SUCCESS)
    {
        char* workPtr = m_payloadBufferPtr + headerSize;
        size_t bytesUsed = 0;
        size_t bufferSize = m_payloadBufferSize - headerSize;
        retVal |= decodeNak(workPtr,
                            bufferSize,
                            bytesUsed,
                            messageNumber,
                            commErrors);
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeNak(CIC_MSG_NUM_TYPE &messageNumber, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeNak(const char *bufferPtr,
          const size_t bufferSize,
          size_t &bytesUsed,
          CIC_MSG_NUM_TYPE &messageNumber,
          COM_ERROR_TYPE &commErrors)
{
#ifdef MH_CIC_DEBUG_NAK
    cicDebugStream.println(F("decoding NAK"));
#endif // MH_CIC_DEBUG_NAK
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.println(F("decodeNak() error, bufferPtr == NULL"));
#endif // MH_CIC_DEBUG_NAK
    }
    if (bufferSize < (sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE)))
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.print(F("decodeNak() error, insufficient memory - "));
        cicDebugStream.print(bufferSize);
        cicDebugStream.print(F(" bytes, need "));
        cicDebugStream.println((sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE)));
#endif // MH_CIC_DEBUG_NAK
    }
    if (retVal == CIC_SUCCESS)
    {
        bytesUsed = sizeof(CIC_MSG_NUM_TYPE) + sizeof(COM_ERROR_TYPE);
        char* workPtr = (char*) bufferPtr;
        memcpy((void*) &messageNumber, (void*) workPtr, sizeof(CIC_MSG_NUM_TYPE));
        workPtr += sizeof(CIC_MSG_NUM_TYPE);
        memcpy((void*) &commErrors, (void*) workPtr, sizeof(COM_ERROR_TYPE));
#ifdef MH_CIC_DEBUG_NAK
        cicDebugStream.print(F("decodeNak() message rejected "));
        cicDebugStream.println(messageNumber);
        cicDebugStream.print(F("decodeNak() comm errors       "));
        printComErrorTypeText(cicDebugStream, commErrors);
        cicDebugStream.println();
#endif // MH_CIC_DEBUG_NAK
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end decodeNak()



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
resetForNewRead(void)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    m_receivingMessage = false;
    m_readErrors = COMM_SUCCESS;
    m_receivedSourceAddress = 0;
    m_receivedDestinationAddress = 0;
    m_receivedMessageNumber = 0;
    m_receivedMessageType = CIC_MSG_NONE;
    m_payloadByteCounter = 0;
    m_receivedSizeStated = 0;
    m_receivedBytesRead = 0;
    m_lastByteReceivedMicros = 0;
    return retVal;
} // end CIC_ERROR_TYPE resetForNewRead()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
readMessageData(boolean &messageReady,
                int &bytesThisRead)
{
    if (m_serialBasePtr != NULL)
    {
        return readMessageData(*m_serialBasePtr,
                               messageReady,
                               bytesThisRead);
    }
    else
    {
#ifdef MH_CIC_DEBUG_READMESSAGE
        cicDebugStream.println(F("readMessage() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_READMESSAGE
        return CIC_OTHER_ERROR;
    } // end if (m_serialBasePtr != NULL)
} // end int readMessageData(boolean &messageReady, ... )



mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
readMessageData(Stream &serialDevice,
                boolean &messageReady,
                int &bytesThisRead)
{
    // input checks are performed by called method
    return readMessageData(serialDevice,
                           messageReady,
                           bytesThisRead,
                           m_payloadBufferSize,
                           m_payloadByteCounter,
                           m_payloadBufferPtr);
} // end int readMessageData(Stream &serialDevice, ... member buffer )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
readMessageData(Stream &serialDevice,
                boolean &messageReady,
                int &bytesThisRead,
                size_t &bufferSize,
                size_t &bufferCounter,
                char* bufferPtr)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // check input
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if ((bufferCounter < 0) || (bufferCounter > bufferSize))
    {
        retVal |= CIC_ERR_BAD_COUNT;
    }
    if (bufferSize < framingSize)
    {
        retVal |= CIC_ERR_BAD_COUNT;
        retVal |= CIC_ERR_INSUFF_MEMORY;
    }
    // initial values of parameters passed by reference
    messageReady = false;
    bytesThisRead = 0;
#ifdef MH_CIC_DEBUG_READMESSAGE
    cicDebugStream.print(F("readMessageData() buffer is "));
    cicDebugStream.print(bufferSize);
    cicDebugStream.print(F(" bytes at "));
    cicDebugStream.println((unsigned long) bufferPtr);
#endif // MH_CIC_DEBUG_READMESSAGE
    if (retVal == CIC_SUCCESS)
    {
        while (serialDevice.available() > 0)
        {
            int readReturn = serialDevice.read();
            if (readReturn != -1)
            {
                // place byte in buffer
                if (bufferCounter < bufferSize)
                {
                    bufferPtr[bufferCounter] = (unsigned char) readReturn;
                    bufferCounter++;
                }
                else
                {
                    m_readErrors |= COM_INSUFF_MEMORY;
                } // end if (bufferCounter < bufferSize)
                // note time, increment counters and update timer
                bytesThisRead++;
                m_receivedBytesRead++;
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.print(F("readMessageData(), read 0x"));
                    cicDebugStream.print((unsigned char) readReturn, HEX);
                    cicDebugStream.print(F(", "));
                    cicDebugStream.print(bytesThisRead);
                    cicDebugStream.print(F(" bytes this read, "));
                    cicDebugStream.print(m_receivedBytesRead);
                    cicDebugStream.print(F(" m_receidvedBytesRead, "));
                    cicDebugStream.print(bufferCounter);
                    cicDebugStream.println(F(" bufferCounter"));
#endif // MH_CIC_DEBUG_READMESSAGE
                if (m_unusedReadBytes > 0)
                {
                    // bytes were in buffer prior to processing last message
                    // do not update last receive time
                    m_unusedReadBytes--;
                }
                else
                {
                    // new data, update last receive time
                    m_lastByteReceivedMicros = micros();
                } // end if (m_unusedReadBytes > 0)
                // read until length of synchronization pattern
                if (m_receivedBytesRead < sizeof(CIC_SYNC_TYPE))
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.println(F("Waiting for synchronization length"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    continue;
                } // end if (m_receivedBytesRead < sizeof(CIC_SYNC_TYPE))
                // check for syncronization pattern
                if (m_receivedBytesRead == sizeof(CIC_SYNC_TYPE))
                {
                    CIC_SYNC_TYPE syncWorkVar = 0;
                    memcpy((void*) &syncWorkVar, (void*) bufferPtr, sizeof(CIC_SYNC_TYPE));
#ifdef MH_CIC_DEBUG_READMESSAGE
                        cicDebugStream.print(F("Synchronization value copied as, 0x"));
                        cicDebugStream.print(syncWorkVar, HEX);
#endif // MH_CIC_DEBUG_READMESSAGE
                    if (syncWorkVar == syncValue)
                    {
                        m_receivingMessage = true;
#ifdef MH_CIC_DEBUG_READMESSAGE
                        cicDebugStream.println(F(", synchronized"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    }
                    else
                    {
                        // not synchronized, shift buffer contents
                        // one byte to continue search for synchronization
                        // pattern
                        // m_receivedBytesRead is known > 0 because size_t returns only positive values
                        // bufferCounter is known > 0 because it was checked non-negative
                        // index i will not exceed bufferSize because bufferSize was checked > framingSize
                        m_receivedBytesRead--;
                        bufferCounter--;
                        for (size_t i=0; i<bufferCounter; i++)
                        {
                            bufferPtr[i] = bufferPtr[i+1];
                        } // end for (i)
#ifdef MH_CIC_DEBUG_READMESSAGE
                        cicDebugStream.println(F(", extended search for synchronization pattern"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    } // end if (syncWorkVar == syncValue)
                    continue;
                } // end if (m_receivedBytesRead == sizeof(CIC_SYNC_TYPE))
                // read until entire header is received
                if (m_receivedBytesRead < headerSize)
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.println(F("reading header"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    // continue reading until header is complete
                    continue;
                } // end if (m_receivedBytesRead < headerSize)
                // interpret header
                if (m_receivedBytesRead == headerSize)
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.println(F("header received"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    // interpret header values
                    // buffer will not be exceeded because bufferSize was
                    // checked > framingSize
                    CIC_SIZE_TYPE workOffset = sizeof(CIC_SYNC_TYPE);
                    // source address
                    memcpy((void*) &m_receivedSourceAddress,
                           (void*) &(bufferPtr[workOffset]),
                           sizeof(CIC_ADDRESS_TYPE));
                    workOffset += sizeof(CIC_ADDRESS_TYPE);
                    // destination address
                    memcpy((void*) &m_receivedDestinationAddress,
                           (void*) &(bufferPtr[workOffset]),
                           sizeof(CIC_ADDRESS_TYPE));
                    workOffset += sizeof(CIC_ADDRESS_TYPE);
                    // message number
                    memcpy((void*) &m_receivedMessageNumber,
                           (void*) &(bufferPtr[workOffset]),
                           sizeof(CIC_SIZE_TYPE));
                    workOffset += sizeof(CIC_MSG_NUM_TYPE);
                    // message type
                    memcpy((void*) &m_receivedMessageType,
                           (void*) &(bufferPtr[workOffset]),
                           sizeof(CIC_MESSAGE_TYPE));
                    workOffset += sizeof(CIC_MESSAGE_TYPE);
                    // stated message size
                    // computed as buffer size stated in message plus
                    // framing size
                    memcpy((void*) &m_receivedSizeStated,
                           (void*) &(bufferPtr[workOffset]),
                           sizeof(CIC_MESSAGE_TYPE));
                    m_receivedSizeStated += framingSize;
                    workOffset += sizeof(CIC_SIZE_TYPE);
                    // check header values
                    // full error checking only if message is addressed to this object
                    if (m_receivedDestinationAddress == m_myAddress)
                    {
                        if (m_receivedSizeStated < framingSize)
                        {
                            m_readErrors |= COM_BAD_COUNT;
                            messageReady = true;
                        }
                        if (m_receivedSizeStated > bufferSize)
                        {
                            m_readErrors |= COM_INSUFF_MEMORY;
                        }
#ifdef MH_CIC_RESTRICT_MESSAGE_TYPE
                        if ( (m_receivedMessageType != CIC_MSG_NONE) &&
                                (m_receivedMessageType != CIC_MSG_ACK) &&
                                (m_receivedMessageType != CIC_MSG_NAK) &&
                                (m_receivedMessageType != CIC_MSG_TYPE) &&
                                (m_receivedMessageType != CIC_MSG_STATUS) &&
                                (m_receivedMessageType != CIC_MSG_CAPABILITIES) &&
                                (m_receivedMessageType != CIC_MSG_COMMAND) &&
                                (m_receivedMessageType != CIC_MSG_DATA)           )
                        {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.print(F("Error - unknown message type 0x"));
                    cicDebugStream.println(m_receivedMessageType, HEX);
#endif // MH_CIC_DEBUG_READMESSAGE
                            m_readErrors |= COM_UNKNOWN_TYPE;
                        } // end if if ( (m_receivedMessageType != CIC_MSG_NONE) && ... )
#endif // MH_CIC_RESTRICT_MESSAGE_TYPE
                    } // end if (m_receivedDestinationAddress == m_myAddress)
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.print(F("Source address:       "));
                    cicDebugStream.println(m_receivedSourceAddress);
                    cicDebugStream.print(F("Destination address:  "));
                    cicDebugStream.println(m_receivedDestinationAddress);
                    cicDebugStream.print(F("Message number:       "));
                    cicDebugStream.println(m_receivedMessageNumber);
                    cicDebugStream.print(F("Message type:         "));
                    printCicMessageTypeText(cicDebugStream, m_receivedMessageType);
                    cicDebugStream.println();
                    cicDebugStream.print(F("Message size stated:  "));
                    cicDebugStream.println(m_receivedSizeStated);
                    cicDebugStream.print(F("Communicatoin errors: "));
                    printCicErrorTypeText(cicDebugStream, m_readErrors);
                    cicDebugStream.println();
#endif // MH_CIC_DEBUG_READMESSAGE
                    // only return to top of loop if message is incomplete
                    // if m_receivedBytesRead == m_receivedSizeStated CRC will be checked below
                    // if m_receivedBytesRead > m_receivedSizeStated error will be set below
                    if (m_receivedBytesRead < m_receivedSizeStated)
                    {
                        continue;
                    } // end if (m_receivedBytesRead < m_receivedSizeStated)
                } // end if (m_receivedBytesRead == headerSize)
                // read until stated message length
                if (m_receivedBytesRead < m_receivedSizeStated)
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.println(F("reading body"));
#endif // MH_CIC_DEBUG_READMESSAGE
                    continue;
                }
                // check CRC and indicate message ready if appropriate
                if (m_receivedBytesRead == m_receivedSizeStated)
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.print(F("received stated size, "));
                    cicDebugStream.print(m_receivedSizeStated);
                    cicDebugStream.println();
#endif // MH_CIC_DEBUG_READMESSAGE
                    // check CRC
                    if ( (m_receivedBytesRead >= framingSize)   &&
                            (bufferCounter == m_receivedBytesRead)    )
                    {
                        CIC_CRC_TYPE crcReceivedValue;
                        size_t offsetWorkVar = m_receivedBytesRead - sizeof(CIC_CRC_TYPE);
                        memcpy((void*) &crcReceivedValue,
                               (void*) &(bufferPtr[offsetWorkVar]),
                               sizeof(CIC_CRC_TYPE));
                        CIC_CRC_TYPE crcWorkVar = 0;
                        crcInit(crcWorkVar);
                        size_t sizeWorkVar = m_receivedBytesRead - sizeof(CIC_CRC_TYPE);
                        updateCrc(crcWorkVar, (const uint8_t*) bufferPtr, sizeWorkVar);
                        completeCrc(crcWorkVar);
#ifdef MH_CIC_DEBUG_READMESSAGE
                        cicDebugStream.print(F("Received CRC:   0x"));
                        cicDebugStream.println(crcReceivedValue, HEX);
                        cicDebugStream.print(F("Calculated CRC: 0x"));
                        cicDebugStream.println(crcWorkVar, HEX);
#endif // MH_CIC_DEBUG_READMESSAGE
                        if (crcWorkVar != crcReceivedValue)
                        {
                            m_readErrors |= COM_BAD_CRC;
                        } // end if (crcWorkVar != crcReceivedValue)
                    }
                    else
                    {
                        m_readErrors |= COM_BAD_COUNT;
                        m_readErrors |= COM_BAD_CRC;
#ifdef MH_CIC_DEBUG_READMESSAGE
                        cicDebugStream.print(F("Insufficient buffer bytes to check CRC, "));
                        cicDebugStream.println(bufferCounter);
#endif // MH_CIC_DEBUG_READMESSAGE
                    } // end if ((m_receivedBytesRead >= framingSize) && ... )
                    // indicate message ready
                    messageReady = true;
                    break;
                } // end if (m_receivedBytesRead == m_receiveSizeStated)
                // catch error condition if m_receiveSizeStated < bytes already read
                if (m_receivedBytesRead > m_receivedSizeStated)
                {
#ifdef MH_CIC_DEBUG_READMESSAGE
                    cicDebugStream.print(F("receiving beyond stated bytes, "));
                    cicDebugStream.print(m_receivedBytesRead);
                    cicDebugStream.print(F(" of "));
                    cicDebugStream.println(m_receivedSizeStated);
#endif // MH_CIC_DEBUG_READMESSAGE
                    m_readErrors |= COM_BAD_COUNT;
                    messageReady = true;
                    break;
                } // end if (m_receivedBytesRead > m_receiveSizeStated)
            }
            else
            {
                // readReturn == -1 indicates read error
                // break to avoid possible infinite loop
                break;
            } // end if (readReturn != -1)
        } // end while (serialDevice.available)
    } // end if (retVal == CIC_SUCCESS)
    // note any unused bytes
    m_unusedReadBytes = serialDevice.available();
#ifdef MH_CIC_DEBUG_READMESSAGE
    cicDebugStream.print(F("readMessageData() "));
    cicDebugStream.print(m_unusedReadBytes);
    cicDebugStream.println(F(" bytes unused"));
#endif // MH_CIC_DEBUG_READMESSAGE
    return retVal;
} // end int readMessageData(Stream &serialDevice, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
readMessageBlocking(Stream &serialDevice,
                    boolean &messageReady,
                    int &bytesThisRead,
                    const unsigned long timeoutMillis)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set parameters passed by reference
    messageReady = false;
    bytesThisRead = 0;
    // read data until complete message or timeout
    unsigned long timeoutMicros = 1000UL * timeoutMillis;
    unsigned long startMicros = micros();
    unsigned long rxTime = getByteTxTime();
    boolean continueLoop = true;
    while (continueLoop)
    {
        unsigned long currentMicros = micros();
        unsigned long microsLooping = microsElapsed(startMicros, currentMicros);
        if (microsLooping > timeoutMicros)
        {
            continueLoop = false;
        } // end if (microsLooping > timeoutMicros)

        if (serialDevice.available() > 0)
        {
            int bytesRead = 0;
            readMessageData(serialDevice,
                            messageReady,
                            bytesRead);
            if (bytesRead > 0)
            {
                bytesThisRead += bytesRead;
            } // end if (bytesRead > 0)
            if (messageReady)
            {
                continueLoop = false;
            } // end if (messageReady)
        }
        else
        {
            // delay one byte period
            delayMicroseconds(rxTime);
        } // end
    } // end while (loopControl)
    return retVal;
} // end CIC_ERROR_TYPE readMessageBlocking(Stream &serialDevice, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
checkPayloadPtrAndSize(const char* bufferPtr,
                       const size_t bufferSize)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (bufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if ((bufferSize <=0) || (((size_t) framingSize + bufferSize) > (size_t) cicSizeTypeMax))
    {
        retVal |= CIC_ERR_BAD_COUNT;
    }
    return retVal;
} // end CIC_ERROR_TYPE checkPayloadPtrAndSize(const char* bufferPtr = m_payloadBufferPtr, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
checkHeaderAvailable(void)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if ( (m_payloadBufferSize < headerSize)    ||
            (m_receivedBytesRead < headerSize)    )
    {
        retVal |= CIC_ERR_BAD_COUNT;
    }
    return retVal;
} // end checkHeaderAvailable()



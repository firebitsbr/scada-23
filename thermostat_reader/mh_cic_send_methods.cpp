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
sendMessage(int &bytesWritten,
            const CIC_ADDRESS_TYPE destinationAddress,
            const CIC_MESSAGE_TYPE messageType)
{
    if (m_serialBasePtr != NULL)
    {
        return sendMessage(*m_serialBasePtr,
                           bytesWritten,
                           destinationAddress,
                           messageType);
    }
    else
    {
#ifdef MH_CIC_DEBUG_SENDMESSAGE
        cicDebugStream.println(F("sendMessage() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_SENDMESSAGE
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE sendMessage(int &bytesWritten, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
sendMessage(Stream &serialDevice,
            int &bytesWritten,
            const CIC_ADDRESS_TYPE destinationAddress,
            const CIC_MESSAGE_TYPE messageType)
{
    // input checks are performed by called method
    size_t bufferBytes = min(m_payloadBufferSize, m_payloadByteCounter);
    return ( sendMessage(serialDevice,
                         bytesWritten,
                         destinationAddress,
                         messageType,
                         bufferBytes,
                         m_payloadBufferPtr) );

} // end sendMessage(Stream &serialDevice, ... member buffer )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
sendMessage(int &bytesWritten,
            const CIC_ADDRESS_TYPE destinationAddress,
            const CIC_MESSAGE_TYPE messageType,
            const size_t bufferSize,
            const char* bufferPtr)
{
    if (m_serialBasePtr != NULL)
    {
        return sendMessage(*m_serialBasePtr,
                           bytesWritten,
                           destinationAddress,
                           messageType,
                           bufferSize,
                           bufferPtr);
    }
    else
    {
#ifdef MH_CIC_DEBUG_SENDMESSAGE
        cicDebugStream.println(F("sendMessage() m_serialBasePtr is NULL"));
#endif // MH_CIC_DEBUG_SENDMESSAGE
        return CIC_OTHER_ERROR;
    }
} // end CIC_ERROR_TYPE sendMessage(int &bytesWritten, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
sendMessage(Stream &serialDevice,
            int &bytesWritten,
            const CIC_ADDRESS_TYPE destinationAddress,
            const CIC_MESSAGE_TYPE messageType,
            const size_t bufferSize,
            const char* bufferPtr)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set initial values of parameters passed by reference
    bytesWritten = 0;
    unsigned long byteTxTime = getByteTxTime();
    // check input
    retVal |= checkPayloadPtrAndSize();
    if (retVal == CIC_SUCCESS)
    {
        // wait for two byte periods after last byte received
        // to detect line is clear
        boolean lineIsClear = false;
        while (!lineIsClear)
        {
            // calculate time since last byte received
            // if new data has been received since m_unusedReadBytes was updated,
            // microsSinceRead is 0
            // detection of new data received is complicated by the possibility of
            // a full buffer.  if the buffer is full, read one byte from buffer
            // (m_unusedReadBytes must be decremented and m_lastByteReceivedMicros
            // is made current assuming overflow of additional bytes not inserted
            // into full buffer)
            unsigned long currentMicros = micros();
            if (m_unusedReadBytes >= hardwareSerialMaxBytes)
            {
                int byteIsDiscarded = serialDevice.read();
                m_unusedReadBytes--;
                m_lastByteReceivedMicros = currentMicros;
                continue;
            } // end if (m_unusedReadBytes >= hardwareSerialMaxBytes)
            int serialBytesWaiting = serialDevice.available();
            if (serialBytesWaiting > m_unusedReadBytes)
            {
                m_unusedReadBytes = serialBytesWaiting;
                m_lastByteReceivedMicros = currentMicros;
            } // end if (serialBytesWaiting > m_unusedReadBytes)
            // if 2 byte periods have elapsed since last byte received, assume line is clear
            unsigned long microsSinceRead = microsElapsed(m_lastByteReceivedMicros, currentMicros);
            if (microsSinceRead >= (lineClearByteIntervals * byteTxTime))
            {
                // exit delay loop
                lineIsClear = true;
            }
            else
            {
                // wait and continue to test
                // delay is remainder of 2 byte periods
                delayMicroseconds((lineClearByteIntervals * byteTxTime) - microsSinceRead);
                // will continue
            } // end if (microsSinceRead < (lineClearByteIntervals * byteTxTime))
        } // end while (!lineIsClear)
        // construct and transmit message
        txEnable();
        unsigned long txStartMicros = micros();
        CIC_CRC_TYPE crcValue;
        crcInit(crcValue);
        bytesWritten += serialDevice.write((uint8_t*) &syncValue, sizeof(CIC_SYNC_TYPE));
        updateCrc(crcValue, (const uint8_t*) &syncValue, sizeof(CIC_SYNC_TYPE));
        bytesWritten += serialDevice.write((uint8_t*) &m_myAddress, sizeof(CIC_ADDRESS_TYPE));
        updateCrc(crcValue, (const uint8_t*) &m_myAddress, sizeof(CIC_ADDRESS_TYPE));
        bytesWritten += serialDevice.write((uint8_t*) &destinationAddress, sizeof(CIC_ADDRESS_TYPE));
        updateCrc(crcValue, (const uint8_t*) &destinationAddress, sizeof(CIC_ADDRESS_TYPE));
        ++m_outgoingMessageCounter;
        bytesWritten += serialDevice.write((uint8_t*) &m_outgoingMessageCounter, sizeof(CIC_MSG_NUM_TYPE));
        updateCrc(crcValue, (const uint8_t*) &m_outgoingMessageCounter, sizeof(CIC_MSG_NUM_TYPE));
        bytesWritten += serialDevice.write((uint8_t*) &messageType, sizeof(CIC_MESSAGE_TYPE));
        updateCrc(crcValue, (const uint8_t*) &messageType, sizeof(CIC_MESSAGE_TYPE));
        CIC_SIZE_TYPE payloadSize = bufferSize;
        bytesWritten += serialDevice.write((uint8_t*) &payloadSize, sizeof(CIC_SIZE_TYPE));
        updateCrc(crcValue, (const uint8_t*) &bufferSize, sizeof(CIC_SIZE_TYPE));
        bytesWritten += serialDevice.write((uint8_t*) bufferPtr, bufferSize);
        updateCrc(crcValue, (const uint8_t*) bufferPtr, bufferSize);
        completeCrc(crcValue);
        bytesWritten += serialDevice.write((uint8_t*) &crcValue, sizeof(CIC_CRC_TYPE));
        // wait for transmission of all data
        serialDevice.flush();
        // if sufficient time has passed for complete transmission
        // call time will be greater than end of transmission time
        // causing delay value to wrap.  if delay value did not wrap,
        // delay for the remaining microseconds.
        // in calculating the time to transmit one byte is added to ensure
        // transmission is complete.
        unsigned long endOfTxMicros = txStartMicros + (unsigned long) (bytesWritten + 1) * byteTxTime;
        unsigned long callTimeMicros = micros();
        unsigned long microsToDelay = endOfTxMicros - callTimeMicros;
        if (microsToDelay < HALF_MAX_UNSIGNED_LONG)
        {
            delayMicroseconds(microsToDelay);
        } // end if (microsToDelay < HALF_MAX_UNSIGNED_LONG)
        rxEnable();
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end int sendMessage(Stream &serialDevice, ... full parameters )



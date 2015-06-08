#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
setOutgoingCommandValue(const CIC_COMMAND_TYPE commandValue,
                        int &dataOffset,
                        int &maxDataBytes)
// sets outgoing command value and value of m_payloadByteCounter
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set outgoing message type to command
    m_outgoingMessageType = CIC_MSG_COMMAND;
    // payload can be used only if m_payloadBufferPtr != NULL
    // and m_payloadBufferSize > size of command
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    } // end if (m_payloadBufferPtr != NULL)
    if ( (m_payloadBufferSize < sizeof(commandValue)) ||
            (maxPayloadSize < sizeof(commandValue))      )
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
    } // end if (m_payloadBufferSize > 0)
    if (retVal == CIC_SUCCESS)
    {
        // entire payload buffer is available for use as command
        // because outgoing header is constructed using member variables
        // first N bytes are the command value
        memcpy((void*) m_payloadBufferPtr,
               (void*) &commandValue,
               sizeof(commandValue));
        m_payloadByteCounter = sizeof(commandValue);
        // remaining bytes are available for command data
        dataOffset = sizeof(commandValue);
        maxDataBytes = min(m_payloadBufferSize, maxPayloadSize) - sizeof(commandValue);
    }
    else
    {
        dataOffset = 0;
        maxDataBytes = 0;
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE setOutgoingCommandValue(const CIC_COMMAND_TYPE commandValue, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
checkCommandAvailable(int &commandOffset,
                      int &commandBytes)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    const size_t minimumSize = headerSize + sizeof(CIC_COMMAND_TYPE);
    const size_t minimumRead = m_receivedSizeStated - sizeof(CIC_CRC_TYPE);
    if ( (m_payloadBufferSize < minimumSize)   ||
            (m_payloadBufferSize < minimumRead)   ||
            (m_receivedSizeStated < minimumSize) ||
            (m_receivedBytesRead < minimumSize)   ||
            (m_receivedBytesRead < minimumRead)      )
    {
        commandOffset = 0;
        commandBytes = 0;
        retVal |= CIC_ERR_BAD_COUNT;
    }
    else
    {
        commandOffset = headerSize;
        commandBytes = m_receivedSizeStated - framingSize;
    }
    return retVal;
} // end checkCommandAvailable()


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeCommand(CIC_COMMAND_TYPE &commandValue,
              int &dataOffset,
              int &dataBytes)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // set initial values of arguments passed by reference
    int commandOffset = 0;
    int commandBytes = 0;
    retVal |= checkCommandAvailable(commandOffset, commandBytes);
#ifdef MH_CIC_DEBUG_DECODECOMMAND
    cicDebugStream.print(F("checkCommandAvailable() returned 0x"));
    cicDebugStream.println(retVal, HEX);
    cicDebugStream.print(F("commandOffset and commandBytes are "));
    cicDebugStream.print(commandOffset),
    cicDebugStream.print(F(", "));
    cicDebugStream.println(commandBytes);
#endif // MH_CIC_DEBUG_DECODECOMMAND
    if (retVal == CIC_SUCCESS)
    {
        char* commandPtr = m_payloadBufferPtr + commandOffset;
        retVal |= decodeCommand(commandPtr,
                                commandBytes,
                                commandValue,
                                dataOffset,
                                dataBytes);
        // increment dataOffset to include pre-call commandOffset
        dataOffset += commandOffset;
    } // end if (retVal = CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE decodeCommand(CIC_COMMAND_TYPE &commandValue, ... )


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
decodeCommand(const char* commandPtr,
              const size_t commandBytes,
              CIC_COMMAND_TYPE &commandValue,
              int &dataOffset,
              int &dataBytes)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (commandBytes >= sizeof(CIC_COMMAND_TYPE))
    {
        memcpy((void*) &commandValue, (void*) commandPtr, sizeof(CIC_COMMAND_TYPE));
        dataOffset = sizeof(CIC_COMMAND_TYPE);
        dataBytes = commandBytes - sizeof(CIC_COMMAND_TYPE);
#ifdef MH_CIC_DEBUG_DECODECOMMAND
    cicDebugStream.print(F("decodeCommand(), command is "));
    cicCommandTypeEnum cmdVal = (cicCommandTypeEnum) commandValue;
    printCicCommandTypeText(cicDebugStream, cmdVal);
    cicDebugStream.println();
    cicDebugStream.print(F("dataOffset and dataBytes are "));
    cicDebugStream.print(dataOffset);
    cicDebugStream.print(F(", "));
    cicDebugStream.println(dataBytes);
#endif // MH_CIC_DEBUG_DECODECOMMAND
    }
    else
    {
        retVal |= CIC_ERR_BAD_COUNT;
        dataOffset = 0;
        dataBytes = 0;
#ifdef MH_CIC_DEBUG_DECODECOMMAND
    cicDebugStream.print(F("decodeCommand(), insufficient data ("));
    cicDebugStream.print(commandBytes);
    cicDebugStream.println(F(" bytes)"));
#endif // MH_CIC_DEBUG_DECODECOMMAND
    } // end if (commandBytes >= sizeof(CIC_COMMAND_TYPE))
    return retVal;
} // end CIC_ERROR_TYPE decodeCommand(const char* bufferPtr, ... )



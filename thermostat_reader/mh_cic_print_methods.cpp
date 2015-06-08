#include <stdint.h>
#include <stddef.h>

#include <mh_arduinoSerialTypes.h>
#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>
#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sarc_encodeData.h>

int
mh_temperatureSensing::
printCicMembers(Stream &serialDevice,
                commandInterfaceClass *cicPtr)
{
    int retVal = 0;
    retVal += serialDevice.println(F("commandInterfaceClass member data:"));
    retVal += serialDevice.print(F("  m_myAddress                   "));
    serialDevice.flush();
    retVal += serialDevice.println(cicPtr->m_myAddress);
    retVal += serialDevice.print(F("  m_receivingMessage            "));
    serialDevice.flush();
    retVal += serialDevice.println(cicPtr->m_receivingMessage);
    retVal += serialDevice.print(F("  m_readErrors                  "));
    serialDevice.flush();
    retVal += printComErrorTypeText(serialDevice, cicPtr->m_readErrors);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedSourceAddress       "));
    retVal += serialDevice.println(cicPtr->m_receivedSourceAddress);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedDestinationAddress  "));
    retVal += serialDevice.println(cicPtr->m_receivedDestinationAddress);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_outgoingDestinationAddress   "));
    retVal += serialDevice.println(cicPtr->m_outgoingDestinationAddress);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedMessageNumber       "));
    retVal += serialDevice.println(cicPtr->m_receivedMessageNumber);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_outgoingMessageCounter      "));
    retVal += serialDevice.println(cicPtr->m_outgoingMessageCounter);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedMessageType         "));
    retVal += printCicMessageTypeText(serialDevice, cicPtr->m_receivedMessageType);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_outgoingMessageType         "));
    retVal += printCicMessageTypeText(serialDevice, cicPtr->m_outgoingMessageType);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedSizeStated          "));
    retVal += serialDevice.println(cicPtr->m_receivedSizeStated);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedBytesRead           "));
    retVal += serialDevice.println(cicPtr->m_receivedBytesRead);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_payloadBufferSize           "));
    retVal += serialDevice.println(cicPtr->m_payloadBufferSize);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_payloadByteCounter          "));
    retVal += serialDevice.println(cicPtr->m_payloadByteCounter);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_lastByteReceivedMicros      "));
    retVal += serialDevice.println(cicPtr->m_lastByteReceivedMicros);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_unusedReadBytes             "));
    retVal += serialDevice.println(cicPtr->m_unusedReadBytes);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_bufferSource                "));
    retVal += printBufferSourceText(serialDevice, cicPtr->m_bufferSource);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_payloadBufferPtr            "));
    uint32_t ptrVal = (uint32_t) cicPtr->m_payloadBufferPtr;
    retVal += serialDevice.println(ptrVal, HEX);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_dataRate                    "));
    retVal += serialDevice.println(cicPtr->m_dataRate);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_dataBits                    "));
    retVal += serialDevice.println(cicPtr->m_dataBits);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_paritySetting               "));
    retVal += printParityEnumText(serialDevice, cicPtr->m_paritySetting);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_stopBits                    "));
    retVal += serialDevice.println(cicPtr->m_stopBits);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_cicState                    "));
    retVal += printCicStateTypeText(serialDevice, cicPtr->m_cicState);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_errorFlags                  "));
    retVal += printCicErrorTypeText(serialDevice, cicPtr->m_errorFlags);
    retVal += serialDevice.println();
    serialDevice.flush();
    return retVal;
} // end int printCicMembers(Stream &serialDevice, commandInterfaceClass *cicPtr)


int
mh_temperatureSensing::
printCicBuffer(Stream &serialDevice,
               commandInterfaceClass *cicPtr)
{
    if ( (cicPtr->m_payloadBufferSize == 0)     ||
            (cicPtr->m_payloadBufferPtr == NULL)  ||
            (cicPtr->m_payloadBufferSize < 0)      ||
            (cicPtr->m_payloadBufferSize > cicSizeTypeMax)    )
    {
        return 0;
    } // end if ((m_paloadBufferSize == 0) || ... )
    int retVal = 0;
    for (size_t i=0; i<cicPtr->m_payloadBufferSize; i++)
    {

        unsigned char printChar = cicPtr->m_payloadBufferPtr[i];
        retVal += serialDevice.print("0x");
        if (printChar < 16)
        {
            retVal += serialDevice.print('0');
        }
        retVal += serialDevice.print(printChar, HEX);
        if ((i % 8) == 7)
        {
            retVal += serialDevice.println();
        }
        else
        {
            retVal += serialDevice.print(" ");
        } // end if ((i % 8) == 7)
    } // end for (i)
    return retVal;
} // end int printCicBuffer(Stream &serialDevice, commandInterfaceClass *cicPtr)


int
mh_temperatureSensing::
printReceivedMessage(Stream &serialDevice,
                     commandInterfaceClass *cicPtr)
{
    int retVal = 0;
    retVal += serialDevice.println(F("commandInterfaceClass received data:"));
    retVal += serialDevice.print(F("  m_readErrors                  "));
    serialDevice.flush();
    retVal += printComErrorTypeText(serialDevice, cicPtr->m_readErrors);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedSourceAddress       "));
    retVal += serialDevice.println(cicPtr->m_receivedSourceAddress);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedDestinationAddress  "));
    retVal += serialDevice.println(cicPtr->m_receivedDestinationAddress);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedMessageNumber       "));
    retVal += serialDevice.println(cicPtr->m_receivedMessageNumber);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedMessageType         "));
    retVal += printCicMessageTypeText(serialDevice, cicPtr->m_receivedMessageType);
    retVal += serialDevice.println();
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedSizeStated          "));
    retVal += serialDevice.println(cicPtr->m_receivedSizeStated);
    serialDevice.flush();
    retVal += serialDevice.print(F("  m_receivedBytesRead           "));
    retVal += serialDevice.println(cicPtr->m_receivedBytesRead);
    serialDevice.flush();
    switch (cicPtr->m_receivedMessageType)
    {
        case CIC_MSG_NONE:
            // nothing to decode
            break;
        case CIC_MSG_ACK:
            { // code block decode ACK
                CIC_MSG_NUM_TYPE messageNumber = 0;
                cicPtr->decodeAck(messageNumber);
                retVal += serialDevice.print(F("  message number                "));
                retVal += serialDevice.println(messageNumber);
                serialDevice.flush();
            } // close code block decode ACK
            break;
        case CIC_MSG_NAK:
            { // code block decode NAK
                CIC_MSG_NUM_TYPE messageNumber = 0;
                COM_ERROR_TYPE commErrors = CIC_SUCCESS;
                cicPtr->decodeNak(messageNumber,
                                  commErrors);
                retVal += serialDevice.print(F("  message number                "));
                retVal += serialDevice.println(messageNumber);
                serialDevice.flush();
                retVal += serialDevice.print(F("  comm errors                   "));
                retVal += printComErrorTypeText(serialDevice, commErrors);
                retVal += serialDevice.println();
                serialDevice.flush();
            } // close code block decode NAK
            break;
        case CIC_MSG_TYPE:
            { // code block decode type
                MH_VERSION_TYPE cicVersion = { 'x', 'x', 'x', 'x' };
                CIC_SENSOR_TYPE sensorType = CST_NONE;
                MH_VERSION_TYPE sensorVersion = { 'x', 'x', 'x', 'x' };
                CIC_SIZE_TYPE numSensors = 0;
                CIC_FLAGS_TYPE sensorFlags = 0;
                cicPtr->decodeType(cicVersion,
                                   sensorType,
                                   sensorVersion,
                                   numSensors,
                                   sensorFlags);
                retVal += serialDevice.print(F("  CIC version                   "));
                retVal += outputVersionNumber(serialDevice, cicVersion);
                retVal += serialDevice.println();
                retVal += serialDevice.print(F("  sensor type                   "));
                printCicSensorTypeText(serialDevice, sensorType);
                retVal += serialDevice.print(F(", 0x"));
                retVal += serialDevice.println((unsigned long) sensorType, HEX);
                retVal += serialDevice.print(F("  sensor version                "));
                retVal += outputVersionNumber(serialDevice, sensorVersion);
                retVal += serialDevice.println();
                retVal += serialDevice.print(F("  number of sensors             "));
                retVal += serialDevice.println(numSensors);
                serialDevice.flush();
                retVal += serialDevice.print(F("  sensor flags                  "));
                retVal += serialDevice.println(sensorFlags, BIN);
                serialDevice.flush();
            } // close code block decode type
            break;
        case CIC_MSG_STATUS:
            { // code block decode status
                CIC_SIZE_TYPE statedBytes = 0;
                CIC_STATE_TYPE cicState = CIC_UNINIT;
                CIC_ERROR_TYPE cicErrors = CIC_SUCCESS;
                COM_ERROR_TYPE commErrors = COMM_SUCCESS;
                cicPtr->decodeStatus(statedBytes,
                                     cicState,
                                     cicErrors,
                                     commErrors);
                retVal += serialDevice.print(F("  CIC stated bytes              "));
                retVal += serialDevice.println(statedBytes);
                serialDevice.flush();
                retVal += serialDevice.print(F("  CIC state                     "));
                cicStateEnum stateEnum = (cicStateEnum) cicState;
                retVal += printCicStateTypeText(serialDevice, stateEnum);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  CIC errors                    "));
                cicErrorTypeEnum errorEnum = (cicErrorTypeEnum) cicErrors;
                retVal += printCicErrorTypeText(serialDevice, errorEnum);
                retVal += serialDevice.println();
                retVal += serialDevice.print(F("  comm errors                   "));
                retVal += printComErrorTypeText(serialDevice, commErrors);
                retVal += serialDevice.println();
                serialDevice.flush();
                // calculate bytes available to decode SARC status
                size_t bytesAvailable = min(cicPtr->m_payloadBufferSize, cicPtr->m_payloadByteCounter);
                if (bytesAvailable > statedBytes)
                {
                    // calculate location of SARC status
                    char* workPtr = cicPtr->m_payloadBufferPtr + commandInterfaceClass::headerSize + statedBytes;
                    size_t bufferSize = bytesAvailable - statedBytes;
                    size_t bytesUsed = 0;
                    SAC_SIZE_TYPE sacBytes = 0;
                    SAC_STATE_TYPE sensorState = SAC_UNINIT;
                    SAC_FLAGS_TYPE activeMask = 0x0;
                    SAC_ERROR_TYPE sacErrors = 0x0;
                    SAC_FLAGS_TYPE sensorErrors = 0x0;
                    sensirionArrayReaderClass::decodeStatus(workPtr,
                                         bufferSize,
                                         bytesUsed,
                                         sacBytes,
                                         sensorState,
                                         activeMask,
                                         sacErrors,
                                         sensorErrors);
                    retVal += serialDevice.print(F("  SAC stated bytes              "));
                    retVal += serialDevice.println(sacBytes);
                    serialDevice.flush();
                    retVal += serialDevice.print(F("  SAC state                     "));
                    sensirionSensorStateEnum stateEnum = (sensirionSensorStateEnum) sensorState;
                    retVal += printSensirionSensorStateEnum(serialDevice, stateEnum);
                    retVal += serialDevice.println();
                    serialDevice.flush();
                    retVal += serialDevice.print(F("  active sensors                "));
                    retVal += printSensirionFlagsType(serialDevice, activeMask);
                    retVal += serialDevice.println();
                    serialDevice.flush();
                    retVal += serialDevice.print(F("  SAC errors                    "));
                    retVal += printSacErrorsType(serialDevice, sacErrors);
                    retVal += serialDevice.println();
                    serialDevice.flush();
                    retVal += serialDevice.print(F("  sensors in error              "));
                    retVal += printSensirionFlagsType(serialDevice, sensorErrors);
                    retVal += serialDevice.println();
                    serialDevice.flush();
                } // end if (bytesAvailable > statedBytes)
            } // end code block decode status
            break;
        case CIC_MSG_CAPABILITIES:
            // not currently implemented
            break;
        case CIC_MSG_DATA:
            { // code block decode data
                cicSensorTypeEnum cicSensorType = CST_NONE;
                int dataOffset = 0;
                int dataBytes = 0;
                CIC_ERROR_TYPE decodeReturn = cicPtr->decodeData(cicSensorType,
                                                                 dataOffset,
                                                                 dataBytes);
                retVal += serialDevice.print(F("  CIC sensor type               "));
                printCicSensorTypeText(serialDevice, cicSensorType);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  data offset                   "));
                retVal += serialDevice.println(dataOffset);
                serialDevice.flush();
                retVal += serialDevice.print(F("  data bytes                    "));
                retVal += serialDevice.println(dataBytes);
                serialDevice.flush();
                if (decodeReturn == CIC_SUCCESS)
                {
                    char *workPtr = cicPtr->m_payloadBufferPtr + dataOffset;
                    size_t bytesDecoded = 0;

                    SAC_SIZE_TYPE reportedSize = 0;
                    measurementTypeEnum reportedType = MH_NO_MEASUREMENT;
                    SAC_ERROR_TYPE reportedErrors = 0;
                    SAC_FLAGS_TYPE valuesStated = 0;
                    SAC_FLAGS_TYPE valuesInError = 0;
                    SAC_FLAGS_TYPE valuesReceived = 0;
                    const int arraySize = 8 * sizeof(SAC_FLAGS_TYPE);
                    SAC_CONVERTED_TYPE dataArray[arraySize] = { 0.0 };
                    SAC_ERROR_TYPE arrayReturn = decodeDataArray(workPtr,
                                                                 (size_t) dataBytes,
                                                                 bytesDecoded,
                                                                 reportedSize,
                                                                 reportedType,
                                                                 reportedErrors,
                                                                 valuesStated,
                                                                 valuesInError,
                                                                 valuesReceived,
                                                                 arraySize,
                                                                 dataArray);
                    if (arrayReturn == SAC_SUCCESS)
                    {
                        retVal += serialDevice.print(F("  bytes decoded                 "));
                        retVal += serialDevice.println(bytesDecoded);
                        serialDevice.flush();
                        retVal += serialDevice.print(F("  reported size                 "));
                        retVal += serialDevice.println(reportedSize);
                        serialDevice.flush();
                        retVal += serialDevice.print(F("  reported errors               "));
                        retVal += printSacErrorsType(serialDevice, reportedErrors);
                        retVal += serialDevice.println();
                        serialDevice.flush();
                        retVal += serialDevice.print(F("  values stated                 "));
                        retVal += printSensirionFlagsType(serialDevice, valuesStated);
                        retVal += serialDevice.println();
                        serialDevice.flush();
                        retVal += serialDevice.print(F("  values in error               "));
                        retVal += printSensirionFlagsType(serialDevice, valuesInError);
                        retVal += serialDevice.println();
                        serialDevice.flush();
                        retVal += serialDevice.print(F("  values received               "));
                        retVal += printSensirionFlagsType(serialDevice, valuesReceived);
                        retVal += serialDevice.println();
                        serialDevice.flush();
                        retVal += serialDevice.println(F("  data values:                  "));
                        for (int i=0; i< arraySize; i++)
                        {
                            retVal += serialDevice.print(F("              "));
                            retVal += serialDevice.println(dataArray[i]);
                        } // end for (i)
                    }
                    else
                    {
                        serialDevice.print(F("  Error - decodeDataArray() returned "));
                        printSacErrorsType(serialDevice, arrayReturn);
                        serialDevice.println();
                        serialDevice.flush();
                    } // end if (arrayReturn == SAC_SUCCESS)

                }
                else
                {
                    serialDevice.print(F("  Error - decodeData() returned "));
                    printCicErrorTypeText(serialDevice, decodeReturn);
                    serialDevice.println();
                    serialDevice.flush();
                } // end if (decodeReturn == CIC_SUCCESS)
            } // close code block decode data
            // requires knowledge of data type
            break;
        case CIC_MSG_COMMAND:
            { // code block decode command
                CIC_COMMAND_TYPE commandValue = CCT_NO_COMMAND;
                int dataOffset = 0;
                int dataBytes = 0;
                cicPtr->decodeCommand(commandValue,
                                      dataOffset,
                                      dataBytes);
                retVal += serialDevice.print(F("  command value                 "));
                cicCommandTypeEnum commandEnum = (cicCommandTypeEnum) commandValue;
                retVal += printCicCommandTypeText(serialDevice, commandEnum);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  data offset                   "));
                retVal += serialDevice.println(dataOffset);
                serialDevice.flush();
                retVal += serialDevice.print(F("  data bytes                    "));
                retVal += serialDevice.println(dataBytes);
                serialDevice.flush();
                // sensor type specific decoding
                char* workPtr = cicPtr->m_payloadBufferPtr + dataOffset;
                size_t bytesAvailable = min(cicPtr->m_payloadBufferSize, cicPtr->m_payloadByteCounter);
                size_t bytesDecoded = 0;
                SAC_SIZE_TYPE reportedSize = 0;
                measurementTypeEnum reportedType = MH_NO_MEASUREMENT;
                SAC_ERROR_TYPE reportedErrors = SAC_SUCCESS;
                SAC_FLAGS_TYPE valuesStated = 0x0;
                SAC_FLAGS_TYPE valuesInError = 0x0;
                SAC_FLAGS_TYPE valuesReceived = 0x0;
                const int arraySize = 8;
                SAC_CONVERTED_TYPE dataArray[arraySize];
                decodeDataArray(workPtr,
                                bytesAvailable,
                                bytesDecoded,
                                reportedSize,
                                reportedType,
                                reportedErrors,
                                valuesStated,
                                valuesInError,
                                valuesReceived,
                                arraySize,
                                dataArray);
                retVal += serialDevice.print(F("  bytes decoded                 "));
                retVal += serialDevice.println(bytesDecoded);
                serialDevice.flush();
                retVal += serialDevice.print(F("  reported size                 "));
                retVal += serialDevice.println(reportedSize);
                serialDevice.flush();
                retVal += serialDevice.print(F("  reported type                 "));
                retVal += printMeasurementTypeEnum(serialDevice, reportedType);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  reported errors               "));
                retVal += printSacErrorsType(serialDevice, reportedErrors);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  values stated                 "));
                retVal += printSensirionFlagsType(serialDevice, valuesStated);
                retVal += serialDevice.println();
                serialDevice.flush();
                retVal += serialDevice.print(F("  values received               "));
                retVal += printSensirionFlagsType(serialDevice, valuesReceived);
                retVal += serialDevice.println();
                serialDevice.flush();
                for (int i=0; i<arraySize; i++)
                {
                    retVal += serialDevice.print(F("  value["));
                    retVal += serialDevice.print(dataArray[i]);
                    retVal += serialDevice.println(F("]"));
                } // end for (i)
            } // close code block decode command
            break;
        default:
            retVal += serialDevice.print(F("  Unknown message type 0x"));
            retVal += serialDevice.println(cicPtr->m_receivedMessageType);
            break;
    } // end switch (cicPtr->m_receivedMessageType)
    retVal += printCicBuffer(serialDevice,
                             cicPtr);
    serialDevice.flush();
    return retVal;
} // end int printReceivedMessage(Stream &serialDevice,



#ifndef COMMANDINTERFACECLASS_H
#define COMMANDINTERFACECLASS_H

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_cic_commandInterfaceClass linux implementation incomplete"
#elif _WIN32
#error "mh_cic_commandInterfaceClass windows implementation incomplete"
#else
#error "mh_cic_commandInterfaceClass unknown compilation environment"
#endif // determine environment

#include <stdint.h>
#include <stddef.h>

#include <mh_versionNumberType.h>
#include <mh_arduinoSerialTypes.h>
#include <mh_cic_commandInterfaceTypes.h>
#include <mh_serialHandlerClass.h>
#include <mh_rs485WrapperClass.h>

/*! \brief commandInterfaceClass is a basic message handler to receive commands and send replies.
 *
 *  Message format:
 *    Sync                    uint8_t = 170 (binary 10101010)
 *    Source address          uint8_t
 *    Destination address     uint8_t
 *    Message number          uint8_t
 *    Message type            uint8_t
 *    Payload size            uint8_t
 *    Payload                 char[Payload size]
 *    CRC                 `   uint8_t
 *
 *  - readMessageData() reads data and accumulates it in the payload buffer.
 *    readMessageData() will indicate the availability of a complete message by the messageReady
 *    calling parameter.
 *  - resetForNewRead() must be called after receipt of each complete message
 *    and prior to calling readMessageData() to start a new message.
 *  - readMessageData() checks for timeout in transmission.  The timeout duration is readTimeoutByteIntervals byte periods
 *    multiplied by the byte transmission time calculated using getByteTxTime().
 *  - sendMessageData() waits for lineClearByteIntervals of no received data prior to transmission
 *    as a simple test for line clear
 *  - sendMessageData() blocks until transmission is complete.
 *
 *  WARNINGS:
 *  - if the parameter m_hardwareSerialMaxBytes is incorrect communication will be unreliable
 *    -- if m_hardwareSerialMaxBytes is too small, received bytes will be discarded prior to transmission
 *    -- if m_hardwareSerialMaxBytes is too large, a busy serial line may not be detected
 */

// Usage notes:
// - Constructors
//     commandInterfaceClass(void) does not set address or allocate the buffer used
//       to send and receive messages
//     commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress, ... ) allocates memory
//       for the buffer used to send and receive messages
// - respondAck(), respondNak(), and respondUnable() reply to the sending address as indicated
//   by m_receivedSourceAddress
// - checkPayloadPtrAndSize() is used prior to message transmission to verify buffer availability
//   and that the payload will fit within the maximum message size
// - getByteTxTime() calculates the time in microseconds to transmit one byte

// ACK payload
//  Received message number uint8_t

// NAK payload
//  Received message number uint8_t
//  Comm error type         uint8_t

// Type payload
//  Command interface class
//    software version      char[4]
//  Sensor type             uint8_t
//  Sensor controller
//    software version      char[4]
//  Number of sensors       uint8_t
//  Sensor flags            uint8_t

// Status payload
//  CIC status
//   Data length            uint8_t
//   CIC state              uint8_t
//   CIC errors             uint32_t
//   COMM errors            uint8_t
//  Device status
//  Variable by device type
//  Typical:
//   Data length            uint8_t
//   Device state           uint8_t
//   Sensor available flags uint8_t
//   Reader errors          uint32_t
//   Sensor error flags     uint8_t

// Capabilities payload
// Variable by device type
// Not currently implemented
// Typical
//  Payload length          uint8_t
//  Other payload data      [Remaining payload bytes]


// Command payload
//  CIC command             uint8_t
//  Command data            [Remaining payload bytes]
//   Vairable by device type
//   Typical
//    Sensor command        uint8_t
//    Sensor command data   uint8_t

// Data payload
//  Sensor type identifier  uint8_t
//  Data                    [Remaining payload bytes]
//   Variable by device type
//   Typical
//    Data length           uint8_t
//    Measurement type      uint8_t
//    Error flags           uint8_t
//    Value present flags   uint8_t
//    Values                float[]


// set the serial base type and default serial
// as appropriate for serial type
#ifdef ARDUINO
#define CIC_SERIAL_BASE_TYPE HardwareSerial
#define CIC_DEFAULT_SERIAL_DEVICE Serial
#elif __linux__
#error "mh_rs485WrapperClass linux serial base type not defined"
#elif _WIN32
#error "mh_rs485WrapperClass windows serial base type not defined"
#else
#error "mh_rs485WrapperClass serial base type not defined"
#endif // define serial base type

// uncomment one line below to select serial interface base class
// #define MH_CIC_USE_SERIALHANDLERCLASS
#define MH_CIC_USE_RS485WRAPPERCLASS

// implementation of serial base class selection
#ifdef MH_CIC_USE_SERIALHANDLERCLASS
#define MH_CIC_SERIAL_BASE_CLASS serialHandlerClass
#elif defined MH_CIC_USE_RS485WRAPPERCLASS
#define MH_CIC_SERIAL_BASE_CLASS rs485WrapperClass<HardwareSerial>
#else
#error "unknown CIC base class for serial interface"
#endif // serial base class selection


// uncomment below to include code for debug output on embedded systems
extern Stream& cicDebugStream;
// #define MH_CIC_DEBUG_READMESSAGE
// #define MH_CIC_DEBUG_DECODECOMMAND
// #define MH_CIC_DEBUG_ACK
// #define MH_CIC_DEBUG_NAK
// #define MH_CIC_DEBUG_RESPONDUNABLE
// #define MH_CIC_DEBUG_RESPONDTYPE
// #define MH_CIC_DEBUG_RESPONDSTATUS


namespace mh_temperatureSensing
{

class commandInterfaceClass:
    public MH_CIC_SERIAL_BASE_CLASS
{
public:
    commandInterfaceClass(void);
#ifdef MH_CIC_USE_SERIALHANDLERCLASS
    commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress,
                          const CIC_SIZE_TYPE bufferSize = defaultBufferSize,
                          const int dataRate = defaultDataRate,
                          const int dataBits = defaultDataBits,
                          const parityEnum parityOption = defaultParityOption,
                          const int stopBits = defaultStopBits);
#endif // MH_CIC_USE_SERIALHANDLERCLASS
#ifdef MH_CIC_USE_RS485WRAPPERCLASS
    commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress,
                          const CIC_SIZE_TYPE bufferSize = defaultBufferSize,
                          HardwareSerial &serialDevice = CIC_DEFAULT_SERIAL_DEVICE,
                          const int txEnablePin = defaultTxPin,
                          const int rxDisablePin = defaultRxPin,
                          const int dataRate = defaultDataRate,
                          const int dataBits = defaultDataBits,
                          const parityEnum parityOption = defaultParityOption,
                          const int stopBits = defaultStopBits);
#endif // MH_CIC_USE_RS485WRAPPERCLASS
    ~commandInterfaceClass(void);
    CIC_ADDRESS_TYPE getAddress(void);
    CIC_ADDRESS_TYPE setAddress(const CIC_ADDRESS_TYPE newAddress);
    CIC_ERROR_TYPE allocateBuffer(const size_t bufferSize);
    CIC_ERROR_TYPE setBufferByReference(const size_t newBufferSize, char* newBufferAddress);
    CIC_ERROR_TYPE setOutgoingMessageType(CIC_MESSAGE_TYPE &messageType,
                                          int &payloadOffset,
                                          int &maxPayloadBytes);
    CIC_ERROR_TYPE setOutgoingCommandValue(const CIC_COMMAND_TYPE commandValue,
                                           int &dataOffset,
                                           int &maxDataBytes);
    CIC_ERROR_TYPE setOutgoingSensorType(const cicSensorTypeEnum sensorEnumVal,
                                         int &dataOffset,
                                         int &maxDataBytes);
    CIC_ERROR_TYPE sendMessage(int &bytesWritten,
                               const CIC_ADDRESS_TYPE destinationAddress,
                               const CIC_MESSAGE_TYPE messageType);
    CIC_ERROR_TYPE sendMessage(Stream &serialDevice,
                               int &bytesWritten,
                               const CIC_ADDRESS_TYPE destinationAddress,
                               const CIC_MESSAGE_TYPE messageType);
    CIC_ERROR_TYPE sendMessage(int &bytesWritten,
                               const CIC_ADDRESS_TYPE destinationAddress,
                               const CIC_MESSAGE_TYPE messageType,
                               const size_t bufferSize,
                               const char* bufferPtr);
    CIC_ERROR_TYPE sendMessage(Stream &serialDevice,
                               int &bytesWritten,
                               const CIC_ADDRESS_TYPE destinationAddress,
                               const CIC_MESSAGE_TYPE messageType,
                               const size_t bufferSize,
                               const char* bufferPtr);
    CIC_ERROR_TYPE resetForNewRead(void);
    CIC_ERROR_TYPE readMessageData(boolean &messageReady,
                                   int &bytesThisRead);
    CIC_ERROR_TYPE readMessageData(Stream &serialDevice,
                                   boolean &messageReady,
                                   int &bytesThisRead);
    CIC_ERROR_TYPE readMessageData(Stream &serialDevice,
                                   boolean &messageReady,
                                   int &bytesThisRead,
                                   size_t &bufferSize,
                                   size_t &bufferCounter,
                                   char* bufferPtr);
    CIC_ERROR_TYPE readMessageBlocking(boolean &messageReady,
                                       int &bytesThisRead,
                                       const unsigned long timeoutMillis);
    CIC_ERROR_TYPE readMessageBlocking(Stream &serialDevice,
                                       boolean &messageReady,
                                       int &bytesThisRead,
                                       const unsigned long timeoutMillis);
    CIC_ERROR_TYPE respondAck(void);
    CIC_ERROR_TYPE respondAck(Stream &serialDevice);
    CIC_ERROR_TYPE respondNak(const COM_ERROR_TYPE commErrors);
    CIC_ERROR_TYPE respondNak(Stream &serialDevice,
                              const COM_ERROR_TYPE commErrors);
    CIC_ERROR_TYPE respondUnable(const COM_ERROR_TYPE commErrors);
    CIC_ERROR_TYPE respondUnable(Stream &serialDevice,
                                 const COM_ERROR_TYPE commErrors);
    CIC_ERROR_TYPE respondType(const CIC_SENSOR_TYPE sensorType,
                               const MH_VERSION_TYPE sensorVersion,
                               const CIC_SIZE_TYPE numSensors,
                               const CIC_FLAGS_TYPE sensorFlags);
    CIC_ERROR_TYPE respondType(Stream &serialDevice,
                               const CIC_SENSOR_TYPE sensorType,
                               const MH_VERSION_TYPE sensorVersion,
                               const CIC_SIZE_TYPE numSensors,
                               const CIC_FLAGS_TYPE sensorFlags);
    CIC_ERROR_TYPE respondCapabilities(const size_t payloadSize);
    CIC_ERROR_TYPE respondCapabilities(Stream &serialDevice,
                                       const size_t payloadSize);
    CIC_ERROR_TYPE respondStatus(const size_t payloadSize);
    CIC_ERROR_TYPE respondStatus(Stream &serialDevice,
                                 const size_t payloadSize);

    CIC_ERROR_TYPE clearErrors(void);
#ifdef MH_CIC_USE_SERIALHANDLERCLASS
    CIC_ERROR_TYPE initialize(const CIC_ADDRESS_TYPE cicAddress,
                              const CIC_SIZE_TYPE bufferSize = defaultBufferSize,
                              const int dataRate = defaultDataRate,
                              const int dataBits = defaultDataBits,
                              const parityEnum parityOption = defaultParityOption,
                              const int stopBits = defaultStopBits);
#endif // MH_CIC_USE_SERIALHANDLERCLASS
#ifdef MH_CIC_USE_RS485WRAPPERCLASS
    CIC_ERROR_TYPE initialize(const CIC_ADDRESS_TYPE cicAddress,
                              const CIC_SIZE_TYPE bufferSize = defaultBufferSize,
                              CIC_SERIAL_BASE_TYPE &serialDevice = CIC_DEFAULT_SERIAL_DEVICE,
                              const int txEnablePin = defaultTxPin,
                              const int rxDisablePin = defaultRxPin,
                              const int dataRate = defaultDataRate,
                              const int dataBits = defaultDataBits,
                              const parityEnum parityOption = defaultParityOption,
                              const int stopBits = defaultStopBits);
#endif // MH_CIC_USE_RS485WRAPPERCLASS
    CIC_ERROR_TYPE initializeLocal(const CIC_ADDRESS_TYPE cicAddress,
                                   const CIC_SIZE_TYPE bufferSize);
    void initializeLocalNoAlloc(const CIC_ADDRESS_TYPE cicAddress = 0);
    void initializeNoAlloc(const CIC_ADDRESS_TYPE cicAddress = 0);
protected:
    void freeMemory(void);
    CIC_ERROR_TYPE checkPayloadPtrAndSize(void);
    CIC_ERROR_TYPE checkPayloadPtrAndSize(const char* bufferPtr,
                                          const size_t bufferSize);
    int crcInit(CIC_CRC_TYPE &crcValue);
    int updateCrc(CIC_CRC_TYPE &crcValue,
                  const uint8_t* dataAddress,
                  const size_t numBytes);
    int completeCrc(CIC_CRC_TYPE &crcValue);
    CIC_ERROR_TYPE checkHeaderAvailable(void);
    CIC_ERROR_TYPE checkCommandAvailable(int &commandOffset,
                                         int &commandBytes);
    CIC_ERROR_TYPE checkDataAvailable(void);
    CIC_ERROR_TYPE encodeStatus(char *bufferPtr,
                                const size_t bufferSize,
                                size_t &bytesEncoded);
public:
    // functions to decode messages
    CIC_ERROR_TYPE decodeAck(CIC_MSG_NUM_TYPE &messageNumber);
    static CIC_ERROR_TYPE decodeAck(const char *bufferPtr,
                                    const size_t bufferSize,
                                    size_t &bytesUsed,
                                    CIC_MSG_NUM_TYPE &messageNumber);
    CIC_ERROR_TYPE decodeNak(CIC_MSG_NUM_TYPE &messageNumber,
                             COM_ERROR_TYPE &commErrors);
    static CIC_ERROR_TYPE decodeNak(const char *bufferPtr,
                                    const size_t bufferSize,
                                    size_t &bytesUsed,
                                    CIC_MSG_NUM_TYPE &messageNumber,
                                    COM_ERROR_TYPE &commErrors);
    CIC_ERROR_TYPE decodeType(MH_VERSION_TYPE &cicVersion,
                              CIC_SENSOR_TYPE &sensorType,
                              MH_VERSION_TYPE &sensorVersion,
                              CIC_SIZE_TYPE &numSensors,
                              CIC_FLAGS_TYPE &sensorFlags);
    static CIC_ERROR_TYPE decodeType(const char *bufferPtr,
                                     const size_t bufferSize,
                                     size_t &bytesUsed,
                                     MH_VERSION_TYPE &cicVersion,
                                     CIC_SENSOR_TYPE &sensorType,
                                     MH_VERSION_TYPE &sensorVersion,
                                     CIC_SIZE_TYPE &numSensors,
                                     CIC_FLAGS_TYPE &sensorFlags);
    CIC_ERROR_TYPE decodeCommand(CIC_COMMAND_TYPE &commandValue,
                                 int &dataOffset,
                                 int &dataBytes);
    static CIC_ERROR_TYPE decodeCommand(const char* commandPtr,
                                        const size_t commandBytes,
                                        CIC_COMMAND_TYPE &commandValue,
                                        int &dataOffset,
                                        int &dataBytes);
    CIC_ERROR_TYPE decodeData(cicSensorTypeEnum &sensorType,
                              int &dataOffset,
                              int &dataBytes);
    static CIC_ERROR_TYPE decodeData(const char* bufferPtr,
                                     const size_t bufferSize,
                                     cicSensorTypeEnum &sensorType,
                                     int &dataOffset,
                                     int &dataBytes);
    CIC_ERROR_TYPE decodeStatus(CIC_SIZE_TYPE &statedBytes,
                                CIC_STATE_TYPE &cicState,
                                CIC_ERROR_TYPE &cicErrors,
                                COM_ERROR_TYPE &commErrors);
    static CIC_ERROR_TYPE decodeStatus(const char *bufferPtr,
                                       const size_t bufferSize,
                                       size_t &bytesUsed,
                                       CIC_SIZE_TYPE &statedBytes,
                                       CIC_STATE_TYPE &cicState,
                                       CIC_ERROR_TYPE &cicErrors,
                                       COM_ERROR_TYPE &commErrors);
protected:
public:
    CIC_ADDRESS_TYPE m_myAddress = 0;
    boolean m_receivingMessage = false;
    COM_ERROR_TYPE m_readErrors = COMM_SUCCESS;
    CIC_ADDRESS_TYPE m_receivedSourceAddress = 0;
    CIC_ADDRESS_TYPE m_receivedDestinationAddress = 0;
    CIC_ADDRESS_TYPE m_outgoingDestinationAddress = 0;
    CIC_MSG_NUM_TYPE m_receivedMessageNumber = 0;
    CIC_MSG_NUM_TYPE m_outgoingMessageCounter = 0;
    CIC_MESSAGE_TYPE m_receivedMessageType = CIC_MSG_NONE;
    CIC_MESSAGE_TYPE m_outgoingMessageType = CIC_MSG_NONE;
    CIC_SIZE_TYPE m_receivedSizeStated = 0;
    size_t m_receivedBytesRead = 0;
    size_t m_payloadBufferSize = 0;
    size_t m_payloadByteCounter = 0;
    unsigned long m_lastByteReceivedMicros = 0;
    int m_unusedReadBytes = 0;
    bufferSourceEnum m_bufferSource = NO_BUFFER;
    char* m_payloadBufferPtr = NULL;
    cicStateEnum m_cicState = CIC_UNINIT;
    CIC_ERROR_TYPE m_errorFlags = CIC_SUCCESS;
public:
    static const MH_VERSION_TYPE versionNumber;
    static const int defaultBufferSize;
    static const size_t headerSize;
    static const size_t framingSize;
    static const size_t maxPayloadSize;
    // length of status message
    static const CIC_SIZE_TYPE statusMessageLength;
    static const CIC_SIZE_TYPE typeMessagePayloadSize;
    static const int hardwareSerialMaxBytes;
    static const int lineClearByteIntervals;
    static const int readTimeoutByteIntervals;
    // test methods
    // test sending and receiving messages
    friend int exerciseCICReadWrite(HardwareSerial &oneSerial,
                                    const int oneTxPin,
                                    const int oneRxPin,
                                    HardwareSerial &twoSerial,
                                    const int twoTxPin,
                                    const int twoRxPin,
                                    HardwareSerial &testStream);
    friend int exerciseCommandInterfaceClassCrc(Stream &serialDevice,
            commandInterfaceClass *cicPtr);
    friend int printCicMembers(Stream &serialDevice,
                               commandInterfaceClass *cicPtr);
    friend int printCicBuffer(Stream &serialDevice,
                              commandInterfaceClass *cicPtr);
    friend int printReceivedMessage(Stream &serialDevice,
                                    commandInterfaceClass *cicPtr);
};

// test methods
int printCicMembers(Stream &outStream,
                    commandInterfaceClass *cicPtr);
// test sending and receiving messages
int exerciseCICReadWrite(HardwareSerial &oneSerial,
                         const int oneTxPin,
                         const int oneRxPin,
                         HardwareSerial &twoSerial,
                         const int twoTxPin,
                         const int twoRxPin,
                         HardwareSerial &testStream);

// test loopback (communication only)
int cicTestLoopBack(HardwareSerial &outSerial,
                    const int outTxPin,
                    const int outRxPin,
                    HardwareSerial &inSerial,
                    const int inTxPin,
                    const int inRxPin,
                    HardwareSerial &testStream);

// test CRC computation
int exerciseCommandInterfaceClassCrc(Stream &serialDevice,
                                     commandInterfaceClass *cicPtr);

// print member variables
int printCicMembers(Stream &serialDevice,
                    commandInterfaceClass *cicPtr);

// print buffer
int printCicBuffer(Stream &serialDevice,
                   commandInterfaceClass *cicPtr);

// print received message
int printReceivedMessage(Stream &serialDevice,
                         commandInterfaceClass *cicPtr);

} // close namespace mh_temperatureSensing

#endif // COMMANDINTERFACECLASS_H


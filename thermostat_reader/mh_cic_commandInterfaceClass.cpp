#include <stdint.h>
#include <stddef.h>

#include <mh_versionNumberType.h>
#include <mh_arduinoSerialTypes.h>
#include <mh_serialHandlerClass.h>
#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


// Uncomment this line to include message type check in readMessageData().
// Recommendation - Verification of command value should be done in the using application.
//                  It is easily included as the default in a switch statement to handle
//                  known message types at the level of message processing.  Restriction of
//                  message type in this class may conflict with intended use.
// #define MH_CIC_RESTRICT_MESSAGE_TYPE

// uncomment as needed for debug output on embedded systems
#ifdef ARDUINO
Stream& cicDebugStream = Serial;
#elif __linux__
#error "linux debug output not defined"
#elif _WIN32
#error "windows debug output not defined"
#else
#error "unknown environment, debug output not defined"
#endif // determine environment


mh_temperatureSensing::commandInterfaceClass::
commandInterfaceClass(void)
{
    commandInterfaceClass::initializeNoAlloc();
} // end commandInterfaceClass(void)


#ifdef MH_CIC_USE_SERIALHANDLERCLASS
mh_temperatureSensing::commandInterfaceClass::
commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress,
                      const CIC_SIZE_TYPE bufferSize,
                      const int dataRate,
                      const int dataBits,
                      const parityEnum parityOption,
                      const int stopBits) :
    serialHandlerClass(dataRate, dataBits, parityOption, stopBits)
{
    initializeLocal(cicAddress, bufferSize);
} // end commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress, ... )
#endif // MH_CIC_USE_SERIALHANDLERCLASS


#ifdef MH_CIC_USE_RS485WRAPPERCLASS
mh_temperatureSensing::commandInterfaceClass::
commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress,
                      const CIC_SIZE_TYPE bufferSize,
                      CIC_SERIAL_BASE_TYPE &serialDevice,
                      const int txEnablePin,
                      const int rxDisablePin,
                      const int dataRate,
                      const int dataBits,
                      const parityEnum parityOption,
                      const int stopBits) :
    rs485WrapperClass<CIC_SERIAL_BASE_TYPE>(serialDevice, txEnablePin, rxDisablePin,
                                            dataRate, dataBits, parityOption, stopBits)
{
    initializeLocal(cicAddress, bufferSize);
} // end commandInterfaceClass(const CIC_ADDRESS_TYPE cicAddress, ... )
#endif // MH_CIC_USE_RS485WRAPPERCLASS


mh_temperatureSensing::commandInterfaceClass::
~commandInterfaceClass(void)
{
    commandInterfaceClass::freeMemory();
} // end ~commandInterfaceClass(void)


mh_temperatureSensing::CIC_ADDRESS_TYPE
mh_temperatureSensing::commandInterfaceClass::
getAddress(void)
{
    return m_myAddress;
} // end setAddress(const CIC_ADDRESS_TYPE newAddress)


mh_temperatureSensing::CIC_ADDRESS_TYPE
mh_temperatureSensing::commandInterfaceClass::
setAddress(const CIC_ADDRESS_TYPE newAddress)
{
    CIC_ADDRESS_TYPE retVal = m_myAddress;
    m_myAddress = newAddress;
    return retVal;
} // end setAddress(const CIC_ADDRESS_TYPE newAddress)


// clearErrors() returns previous error state
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
clearErrors(void)
{
    CIC_ERROR_TYPE previousErrors = m_errorFlags;
    m_errorFlags = CIC_SUCCESS;
    return previousErrors;
} // end CIC_ERROR_TYPE clearErrors()


#ifdef MC_CIC_USE_SERIALHANDLERCLASS
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
initialize(const CIC_ADDRESS_TYPE cicAddress,
           const CIC_SIZE_TYPE bufferSize,
           HardwareSerial &serialDevice,
           const int txEnablePin,
           const int rxDisablePin,
           const int dataRate,
           const int dataBits,
           const parityEnum parityOption,
           const int stopBits)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    initializeLocal(cicAddress, bufferSize);
    serialHandlerClass::initialize(dataRate,
                                   dataBits,
                                   parityOption,
                                   stopBits);
    return retVal;
} // end initialize(const CIC_ADDRESS_TYPE cicAddress, ... )
#endif // MC_CIC_USE_SERIALHANDLERCLASS


#ifdef MH_CIC_USE_RS485WRAPPERCLASS
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
initialize(const CIC_ADDRESS_TYPE cicAddress,
           const CIC_SIZE_TYPE bufferSize,
           CIC_SERIAL_BASE_TYPE &serialDevice,
           const int txEnablePin,
           const int rxDisablePin,
           const int dataRate,
           const int dataBits,
           const parityEnum parityOption,
           const int stopBits)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    initializeLocal(cicAddress, bufferSize);
    rs485WrapperClass<CIC_SERIAL_BASE_TYPE>::initialize(serialDevice,
                                                        defaultTxPin,
                                                        defaultRxPin,
                                                        dataRate,
                                                        dataBits,
                                                        parityOption,
                                                        stopBits);
    return retVal;
} // end initialize(const CIC_ADDRESS_TYPE cicAddress, ... )
#endif // MH_CIC_USE_RS485WRAPPERCLASS


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
initializeLocal(const CIC_ADDRESS_TYPE cicAddress,
                const CIC_SIZE_TYPE bufferSize)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // initializeNoAlloc called to reduce maintenance burden
    // all parameters are set to defaults in initializeNoAlloc
    // calling parameters (only) are updated here
    initializeLocalNoAlloc(cicAddress);
    retVal |= allocateBuffer(bufferSize);
    if (retVal == CIC_SUCCESS)
    {
        m_cicState = CIC_INITIALIZED;
    }
    else
    {
        m_errorFlags |= retVal;
    } // if (retVal == CIC_SUCCESS)
    return retVal;
} // end initializeLocal(const CIC_ADDRESS_TYPE cicAddress, ... )


void
mh_temperatureSensing::commandInterfaceClass::
initializeLocalNoAlloc(const CIC_ADDRESS_TYPE cicAddress)
{
    m_myAddress = cicAddress;
    m_receivingMessage = false;
    m_readErrors = COMM_SUCCESS;
    m_receivedSourceAddress = 0;
    m_outgoingDestinationAddress = 0;
    m_receivedMessageNumber = 0;
    m_outgoingMessageCounter = 0;
    m_receivedMessageType = CIC_MSG_NONE;
    m_outgoingMessageType = CIC_MSG_NONE;
    m_receivedSizeStated = 0;
    m_receivedBytesRead = 0;
    m_payloadBufferSize = 0;
    m_payloadByteCounter = 0;
    m_lastByteReceivedMicros = 0;
    m_unusedReadBytes = 0;
    m_bufferSource = NO_BUFFER;
    m_payloadBufferPtr = NULL;
    m_cicState = CIC_UNINIT;
    m_errorFlags = CIC_SUCCESS;
} // end int initializeLocalNoAlloc(void)


void
mh_temperatureSensing::commandInterfaceClass::
initializeNoAlloc(const CIC_ADDRESS_TYPE cicAddress)
{
    initializeLocalNoAlloc(cicAddress);
#ifdef MH_CIC_USE_RS485WRAPPERCLASS
    rs485WrapperClass::initializeNoAlloc();
#else
    serialHandlerClass::initializeNoAlloc();
#endif // MH_CIC_USE_RS485WRAPPERCLASS
} // end int initializeNoAlloc(void)


void
mh_temperatureSensing::commandInterfaceClass::
freeMemory(void)
{
    // verify buffer was allocated to prevent deletion of an array
    // shared by reference
    if ((m_payloadBufferPtr != NULL) && (m_bufferSource == ALLOCATED))
    {
        delete m_payloadBufferPtr;
    } // end if (m_payloadBufferPtr != NULL)
    m_payloadBufferPtr = NULL;
    m_payloadBufferSize = 0;
    m_bufferSource = NO_BUFFER;
} // end int freeMemory(void)




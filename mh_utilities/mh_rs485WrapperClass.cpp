#include <mh_rs485WrapperClass.h>
#include <mh_arduinoPinHandler.h>

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
static HardwareSerial& rs485WCDefaultSerial = Serial;
#elif __linux__
#error "linux default output not defined"
#elif _WIN32
#error "windows default output not defined"
#else
#error "unknown environment, default output not defined"
#endif // determine environment

// uncomment below as needed to debug methods
// rs485DebugStream must be defined for all debug blocks
#ifdef ARDUINO
static Stream& rs485DebugStream = Serial;
#elif __linux__
#error "linux debug output not defined"
#elif _WIN32
#error "windows debug output not defined"
#else
#error "unknown environment, debug output not defined"
#endif // determine environment
#define MH_RS485WC_INCLUDE_TEST

// uncomment below to include statements for debug output
// #define MH_RS485WC_DEBUG_START
// #define MH_RS485WC_DEBUG_STOP
// #define MH_RS485WC_DEBUG_AVAILABLE
// #define MH_RS485WC_DEBUG_READ
// #define MH_RS485WC_DEBUG_READ_DETAIL
// #define MH_RS485WC_DEBUG_PEEK
// #define MH_RS485WC_DEBUG_FLUSH
// #define MH_RS485WC_DEBUG_WRITE
// #define MH_RS485WC_DEBUG_WRITE_DETAIL
// #define MH_RS485WC_DEBUG_ENABLE
// #define MH_RS485WC_DEBUG_LISTEN

/// Default control pins are 2 for transmit control and 3 for receive control
template <class T_SerialBase>
const int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
defaultTxPin = 2;

template <class T_SerialBase>
const int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
defaultRxPin = 3;


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rs485WrapperClass(void)
{
    initialize(rs485WCDefaultSerial,
               defaultTxPin,
               defaultRxPin,
               defaultDataRate,
               defaultDataBits,
               defaultParityOption,
               defaultStopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase)


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rs485WrapperClass(T_SerialBase &serialBase)
{
    initialize(serialBase,
               defaultTxPin,
               defaultRxPin,
               defaultDataRate,
               defaultDataBits,
               defaultParityOption,
               defaultStopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase)


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rs485WrapperClass(T_SerialBase &serialBase,
                  const int txEnablePin,
                  const int rxDisablePin)
{
    initialize(serialBase,
               txEnablePin,
               rxDisablePin,
               defaultDataRate,
               defaultDataBits,
               defaultParityOption,
               defaultStopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase, ... )


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rs485WrapperClass(T_SerialBase &serialBase,
                  const long int dataRate,
                  const int dataBits,
                  const parityEnum parityOption,
                  const int stopBits)
{
    initialize(serialBase,
               defaultTxPin,
               defaultRxPin,
               dataRate,
               dataBits,
               parityOption,
               stopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase, ... )


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rs485WrapperClass(T_SerialBase &serialBase,
                  const int txEnablePin,
                  const int rxDisablePin,
                  const long int dataRate,
                  const int dataBits,
                  const parityEnum parityOption,
                  const int stopBits)
{
    initialize(serialBase,
               txEnablePin,
               rxDisablePin,
               dataRate,
               dataBits,
               parityOption,
               stopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase, ... )


// specialization required to correct problem linking in Arduino IDE
template <>
mh_temperatureSensing::rs485WrapperClass<HardwareSerial>::
rs485WrapperClass(HardwareSerial &serialBase,
                  const int txEnablePin,
                  const int rxDisablePin,
                  const long int dataRate,
                  const int dataBits,
                  const parityEnum parityOption,
                  const int stopBits)
{
    initialize(serialBase,
               txEnablePin,
               rxDisablePin,
               dataRate,
               dataBits,
               parityOption,
               stopBits);
} // end rs485WrapperClass(T_SerialBase &serialBase, ... )


template <class T_SerialBase>
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
~rs485WrapperClass()
{
    // set RS485 transceiver to receive mode
    // this prevents possible blocking or jamming of
    // the link by inadvertently leaving transmit enabled.
    rxEnable();
} // end ~rs485WrapperClass()


// specialization required to correct problem linking in Arduino IDE
template <>
mh_temperatureSensing::rs485WrapperClass<HardwareSerial>::
~rs485WrapperClass()
{
    // set RS485 transceiver to receive mode
    // this prevents possible blocking or jamming of
    // the link by inadvertently leaving transmit enabled.
    rxEnable();
} // end ~rs485WrapperClass()


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
start(void)
{
    int retVal = 0;
    // turn off transmitter to prevent jamming link
    rxEnable();
 #ifdef MH_RS485WC_DEBUG_START
    rs485DebugStream.flush();
    rs485DebugStream.print(F(" pins "));
    rs485DebugStream.print(m_txEnablePin);
    rs485DebugStream.print(F(" and "));
    rs485DebugStream.print(m_rxDisablePin);
    debugStream.println(F(" configured as output and set for read"));
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_START
    // start serial device
    if (m_serialBasePtr != NULL)
    {
#ifdef MH_RS485WC_DEBUG_START
        rs485DebugStream.flush();
        rs485DebugStream.print(F(" constructSerialParameters("));
        rs485DebugStream.print(dataBits);
        rs485DebugStream.print(F(" data bits, "));
        printParityEnumText(rs485DebugStream, parityOption);
        rs485DebugStream.print(F(", "));
        rs485DebugStream.print(stopBits);
        rs485DebugStream.println(F(" stop bits)"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_START
        conformDataRate(m_dataRate);
        uint8_t serialParameters = constructSerialParameters(m_dataBits,
                                   m_paritySetting,
                                   m_stopBits);
#ifdef MH_RS485WC_DEBUG_START
        rs485DebugStream.flush();
        rs485DebugStream.print(F("SHC end, begin "));
        rs485DebugStream.print(m_dataRate);
        rs485DebugStream.print(F(", 0x"));
        rs485DebugStream.println(serialParameters, HEX);
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_START
#ifdef ARDUINO
        m_serialBasePtr->end();
        m_serialBasePtr->begin(m_dataRate, serialParameters);
#elif __linux__
#error "rs485WrapperClass linux start implementation incomplete"
#elif _WIN32
#error "rs485WrapperClass windows start implementation incomplete"
#else
#error "rs485WrapperClass unknown compilation environment, start implementation incomplete"
#endif // ARDUINO ...
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_START
        rs485DebugStream.println(F("rs485WrapperClass::start() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_START
        retVal = -1;
    } // end if (m_serialBasePtr != NULL)
    return retVal;
} // end start()


template <class T_SerialBase>
void
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
stop(void)
{
#ifdef MH_RS485WC_DEBUG_STOP
        rs485DebugStream.println(F("rs485WrapperClass::stop()"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_STOP
    // turn off transmitter to prevent jamming link
    rxEnable();
    if (m_serialBasePtr != NULL)
    {
#ifdef ARDUINO
        m_serialBasePtr->end();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_STOP
        rs485DebugStream.println(F("rs485WrapperClass::stop() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_STOP
    } // end if (m_serialBasePtr != NULL)
} // end void stop()


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
available(void)
{
    int retVal = 0;
    if (m_serialBasePtr != NULL)
    {
#ifdef ARDUINO
        retVal = m_serialBasePtr->available();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_AVAILABLE
        rs485DebugStream.println(F("rs485WrapperClass::stop() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_AVAILABLE
        retVal = -1;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_AVAILABLE
    rs485DebugStream.print(F("rs485WrapperClass::available() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_AVAILABLE
    return retVal;
} // end int available(void)


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
read(void)
{
#ifdef MH_RS485WC_DEBUG_READ_DETAIL
    rs485DebugStream.print(F("rs485WrapperClass::read() m_txEnablePin "));
    rs485DebugStream.print(m_txEnablePin);
    rs485DebugStream.print(F(" set "));
    rs485DebugStream.print(digitalRead(m_txEnablePin));
    rs485DebugStream.print(F(", "));
    if (digitalRead(m_txEnablePin) == HIGH)
    {
        rs485DebugStream.println(F("HIGH"));
    }
    else
    {
        rs485DebugStream.println(F("LOW"));
    } // end if (digitalRead(m_txEnablePin) == HIGH)
    rs485DebugStream.flush();
    rs485DebugStream.print(F("rs485WrapperClass::read() m_rxDisablePin"));
    rs485DebugStream.print(m_rxDisablePin);
    rs485DebugStream.print(F(" set "));
    rs485DebugStream.print(digitalRead(m_rxDisablePin));
    rs485DebugStream.print(F(", "));
    if (digitalRead(m_rxDisablePin) == HIGH)
    {
        rs485DebugStream.println(F("HIGH"));
    }
    else
    {
        rs485DebugStream.println(F("LOW"));
    } // end if (digitalRead(m_rxDisablePin) == HIGH)
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_READ_DETAIL
    int retVal = 0;
    if (m_serialBasePtr != NULL)
    {
#ifdef ARDUINO
        retVal = m_serialBasePtr->read();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
#ifdef MH_RS485WC_DEBUG_READ
        rs485DebugStream.print(F("rs485WrapperClass::read() received "));
        rs485DebugStream.print((unsigned char) retVal);
        rs485DebugStream.print(F(", 0x"));
        rs485DebugStream.println(retVal, HEX);
#endif // MH_RS485WC_DEBUG_READ
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_READ
        rs485DebugStream.println(F("rs485WrapperClass::read() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_READ
        retVal = -1;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_READ
    rs485DebugStream.print(F("rs485WrapperClass::read() returning '"));
    rs485DebugStream.print(retVal);
    rs485DebugStream.print(F("', 0x"));
    rs485DebugStream.print(retVal, HEX);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_READ
    return retVal;
} // end int read(void)


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
peek(void)
{
    int retVal = 0;
    if (m_serialBasePtr != NULL)
    {
#ifdef ARDUINO
        retVal = m_serialBasePtr->peek();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_PEEK
        rs485DebugStream.println(F("rs485WrapperClass::peek() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_PEEK
        retVal = -1;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_PEEK
    rs485DebugStream.print(F("rs485WrapperClass::peek() returning '"));
    rs485DebugStream.print(retVal);
    rs485DebugStream.print(F("', 0x"));
    rs485DebugStream.print(retVal, HEX);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_PEEK
    return retVal;
} // end int peek(void)


template <class T_SerialBase>
void
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
flush(void)
{
#ifdef MH_RS485WC_DEBUG_FLUSH
    rs485DebugStream.println(F("rs485WrapperClass::flush()"));
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_FLUSH
    if (m_serialBasePtr != NULL)
    {
#ifdef ARDUINO
        m_serialBasePtr->flush();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_FLUSH
        rs485DebugStream.println(F("rs485WrapperClass::flush() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_FLUSH
    } // end if (m_serialBasePtr != NULL)
} // end int flush(void)


template <class T_SerialBase>
size_t
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
write(uint8_t outByte)
{
#ifdef MH_RS485WC_DEBUG_WRITE_DETAIL
    rs485DebugStream.print(F("rs485WrapperClass::write() m_txEnablePin "));
    rs485DebugStream.print(m_txEnablePin);
    rs485DebugStream.print(F(" set "));
    rs485DebugStream.print(digitalRead(m_txEnablePin));
    rs485DebugStream.print(F(", "));
    if (digitalRead(m_txEnablePin) == HIGH)
    {
        rs485DebugStream.println(F("HIGH"));
    }
    else
    {
        rs485DebugStream.println(F("LOW"));
    } // end if (digitalRead(m_txEnablePin) == HIGH)
    rs485DebugStream.flush();
    rs485DebugStream.print(F("rs485WrapperClass::write() m_rxDisablePin"));
    rs485DebugStream.print(m_rxDisablePin);
    rs485DebugStream.print(F(" set "));
    rs485DebugStream.print(digitalRead(m_rxDisablePin));
    rs485DebugStream.print(F(", "));
    if (digitalRead(m_rxDisablePin) == HIGH)
    {
        rs485DebugStream.println(F("HIGH"));
    }
    else
    {
        rs485DebugStream.println(F("LOW"));
    } // end if (digitalRead(m_rxDisablePin) == HIGH)
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_WRITE_DETAIL
    size_t retVal = 0;
    if (m_serialBasePtr != NULL)
    {
#ifdef MH_RS485WC_DEBUG_WRITE
        rs485DebugStream.print(F("rs485WrapperClass::write() printing "));
        rs485DebugStream.print((unsigned char) outByte);
        rs485DebugStream.print(F(", 0x"));
        rs485DebugStream.println(outByte, HEX);
#endif // MH_RS485WC_DEBUG_WRITE_DETAIL
#ifdef ARDUINO
        retVal = m_serialBasePtr->write(outByte);
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_WRITE
        rs485DebugStream.println(F("rs485WrapperClass::write() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_WRITE
        retVal = -1;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_WRITE
    rs485DebugStream.print(F("rs485WrapperClass::write() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_WRITE
    return retVal;
} // end int write()


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
txEnable(void)
{
    int retVal = 0;
    retVal += setPinHigh(m_rxDisablePin);
    retVal += setPinHigh(m_txEnablePin);
    m_transmitterIsOn = true;
#ifdef MH_RS485WC_DEBUG_ENABLE
    rs485DebugStream.print(F("rs485WrapperClass::txEnable() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_ENABLE
    return retVal;
} // end int txEnable(void)


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
txDisable(void)
{
    int retVal = 0;
    retVal += setPinLow(m_txEnablePin);
    m_transmitterIsOn = false;
#ifdef MH_RS485WC_DEBUG_ENABLE
    rs485DebugStream.print(F("rs485WrapperClass::txDisable() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_ENABLE
    return retVal;
} // end int txDisable(void)


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rxEnable(void)
{
    int retVal = 0;
    retVal += setPinLow(m_txEnablePin);
    retVal += setPinLow(m_rxDisablePin);
    m_transmitterIsOn = false;
#ifdef MH_RS485WC_DEBUG_ENABLE
    rs485DebugStream.print(F("rs485WrapperClass::rxEnable() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_ENABLE
    return retVal;
} // end int rxEnable(void)


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
rxDisable(void)
{
    int retVal = 0;
    retVal += setPinHigh(m_rxDisablePin);
#ifdef MH_RS485WC_DEBUG_ENABLE
    rs485DebugStream.print(F("rs485WrapperClass::rxDisable() returning "));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_ENABLE
    return retVal;
} // end int rxDisable(void)


// general method for use with base classes having no listen()
// or equivalent method
template <class T_SerialBase>
bool
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
listen(void)
{
    bool retVal = true;
    if (m_serialBasePtr != NULL)
    {
        rxEnable();
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_LISTEN
        rs485DebugStream.println(F("rs485WrapperClass::listen() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
        retVal = false;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_LISTEN
    rs485DebugStream.print(F("rs485WrapperClass::listen() returning '"));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
    return retVal;
} // end bool listen(void)


// general method for use with base classes having no isListening()
// or equivalent method
template <class T_SerialBase>
bool
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
isListening(void)
{
    bool retVal = true;
    if (m_serialBasePtr != NULL)
    {
        pinLevelEnum txPinVal = getPinValue(m_txEnablePin);
        pinLevelEnum rxPinVal = getPinValue(m_rxDisablePin);
        if (txPinVal == PIN_VALUE_HIGH)
        {
            retVal = false;
        } // end if (txPinVal == PIN_VALUE_HIGH)
        if (rxPinVal == PIN_VALUE_HIGH)
        {
            retVal = false;
        } // end if (rxPinVal == PIN_VALUE_HIGH)
        if (m_transmitterIsOn == true)
        {
            retVal = false;
        } // end if (m_transmitterIsOn == true)
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_LISTEN
        rs485DebugStream.println(F("rs485WrapperClass::isListening() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
        retVal = false;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_LISTEN
    rs485DebugStream.print(F("rs485WrapperClass::isListening() returning '"));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
    return retVal;
} // end bool isListening(void)


// class-specific listen () for Arduino SoftwareSerial
#ifdef ARDIUNO
template<>
int
mh_temperatureSensing::rs485WrapperClass<SoftwareSerial>::
listen(void)
{
    bool retVal = true;
    if (m_serialBasePtr != NULL)
    {
        rxEnable();
        retVal = m_serialBasePtr->listen();
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_LISTEN
        rs485DebugStream.println(F("rs485WrapperClass<SoftwareSerial>::listen() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
        retVal = false;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_LISTEN
    rs485DebugStream.print(F("rs485WrapperClass<SoftwareSerial>::listen() returning '"));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
    return retVal;
} // end listen(void)
#endif // ARDUINO


// class-specific isListening for Arduino SoftwareSerial
#ifdef ARDIUNO
template<>
int
mh_temperatureSensing::rs485WrapperClass<SoftwareSerial>::
isListening(void)
{
    bool retVal = false;
    if (m_serialBasePtr != NULL)
    {
        pinLevelEnum txPinVal = getPinValue(m_txEnablePin);
        pinLevelEnum rxPinVal = getPinValue(m_rxDisablePin);
        if (txPinVal == PIN_VALUE_HIGH)
        {
            retVal = false;
#ifdef MH_RS485WC_DEBUG_LISTEN
        rs485DebugStream.println(F("rs485WrapperClass<SoftwareSerial>::listen() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
        } // end if (txPinVal == PIN_VALUE_HIGH)
        if (rxPinVal == PIN_VALUE_HIGH)
        {
            retVal = false;
        } // end if (rxPinVal == PIN_VALUE_HIGH)
        if (m_transmitterIsOn == true)
        {
            retVal = false;
        } // end if (m_transmitterIsOn == true)
        bool baseIsListening = m_serialBasePtr->isListening();
        if (baseIsListening != true)
        {
            retVal = false;
        } // end if (baseIsListening != true)
        retVal = m_serialBasePtr->isListening();
    }
    else
    {
#ifdef MH_RS485WC_DEBUG_LISTEN
        rs485DebugStream.println(F("rs485WrapperClass<SoftwareSerial>::isListening() null pointer"));
        rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
        retVal = false;
    } // end if (m_serialBasePtr != NULL)
#ifdef MH_RS485WC_DEBUG_LISTEN
    rs485DebugStream.print(F("rs485WrapperClass<SoftwareSerial>::isListening() returning '"));
    rs485DebugStream.println(retVal);
    rs485DebugStream.flush();
#endif // MH_RS485WC_DEBUG_LISTEN
    return retVal;
} // end listen(void)
#endif // ARDUINO


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
initialize(T_SerialBase &serialDevice,
           const int txEnablePin,
           const int rxDisablePin,
           const long int dataRate,
           const int dataBits,
           const parityEnum parityOption,
           const int stopBits)
{
    int retVal = 0;
    retVal += initializeLocal(serialDevice,
                              txEnablePin,
                              rxDisablePin);
    retVal += serialHandlerClass::initialize(dataRate,
                                             dataBits,
                                             parityOption,
                                             stopBits);
    // set receive and transmit control pins
    // for output
    configurePinOutput(m_txEnablePin);
    configurePinOutput(m_rxDisablePin);
    // set device receive
    // prevents jamming of line by unintended transmitter active
    rxEnable();
    return retVal;
} // end void initialize()


template <class T_SerialBase>
int
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
initializeLocal(T_SerialBase &serialDevice,
                const int txEnablePin,
                const int rxDisablePin)
{
    int retVal = 0;
    m_serialBasePtr = &serialDevice;
    m_txEnablePin = txEnablePin;
    m_rxDisablePin = rxDisablePin;
    m_transmitterIsOn = false;
    return retVal;
} // end void initialize()


template <class T_SerialBase>
void
mh_temperatureSensing::rs485WrapperClass<T_SerialBase>::
initializeNoAlloc(void)
{
    m_serialBasePtr = &rs485WCDefaultSerial;
    m_txEnablePin = defaultTxPin;
    m_rxDisablePin = defaultRxPin;
    m_transmitterIsOn = false;
    serialHandlerClass::initializeNoAlloc();
    // NOTE - m_serialBasePtr is not set to null
    //        this implementation requires m_serialBasePtr
    //        to be set at construction
} // end void initializeNoAlloc(void)



#ifdef MH_RS485WC_INCLUDE_TEST

// test code
int
mh_temperatureSensing::
rs485WCTestLoopBack(HardwareSerial &outSerial,
                    const int outTxPin,
                    const int outRxPin,
                    HardwareSerial &inSerial,
                    const int inTxPin,
                    const int inRxPin,
                    HardwareSerial &testStream)
{
    int retVal = 0;
    rs485WrapperClass<HardwareSerial> rsIn(inSerial, inTxPin, inRxPin, 9600, 8, PARITY_NONE, 1);
    rs485WrapperClass<HardwareSerial> rsOut(outSerial, outTxPin, outRxPin, 9600, 8, PARITY_NONE, 1);
    rsIn.start();
    rsIn.rxEnable();
    rsOut.start();
    rsOut.txEnable();
    testStream.println("serial devices started, delay 5 seconds");
    delay(5);
    testStream.println("begin rs485WrapperClass loopback test");
    for (int i=0; i<10; i++)
    {
        char writeByte = 'a' + i;
        char readByte = 0;
        testStream.print("sending -");
        testStream.print((char) writeByte);
        testStream.print("-   ");
        rsOut.write(writeByte);
        delay(2);
        readByte = rsIn.read();
        testStream.print("received -");
        testStream.print((char) readByte);
        testStream.print("-, 0x");
        testStream.println((unsigned char) readByte, HEX);
        if (readByte != writeByte)
        {
            retVal--;
        }
    } // end for (i)
    if (retVal != 0)
    {
        testStream.print("ERROR - input did not match output ");
        testStream.print(-retVal);
        testStream.println(" times");
    } // end if (retVal != 0)
    testStream.println("end of mh_serialHandlerClass loopback test");
    return retVal;
} // end int testLoopBack()

// sample call for loop back test on Arduino Mega 2650
// int commRet = mh_temperatureSensing::rs485WCTestLoopBack(Serial1, 22, 23, Serial2, 24, 25, Serial);

#endif // #ifdef MH_RS485WC_INCLUDE_TEST



/*

// candidate loop test program for Arduino Mega 2650

#include <Arduino.h>
#include <mh_arduinoPinHandler.h>
#include <mh_rs485WrapperClass.h>

using namespace mh_temperatureSensing;

#define _MH_PROGRAM_NAME_ "rs485 loopback"
#define _MH_VERSION_NUMBER_ "v0.001"

HardwareSerial& consoleSerial = Serial;
const long consoleDataRate = 115200L;

void setup() {
  // start console serial
  consoleSerial.begin(consoleDataRate);
  delay(100);
  // report version number
  consoleSerial.println(_MH_PROGRAM_NAME_);
  consoleSerial.println(_MH_VERSION_NUMBER_);
  consoleSerial.println("initialization complete");
  consoleSerial.println("delaying 5 seconds before start");
  delay(5000);
  int commRet = mh_temperatureSensing::rs485WCTestLoopBack(Serial1, 22, 23, Serial2, 24, 25, Serial);
  consoleSerial.print("test complete, return value = ");
  consoleSerial.println(commRet);
} //end setup()


void loop() {

  delay(1000);

} // end loop()


*/


/*

// RS-485 listener
// Note - verbose byte-by-byte output.  intended only for low data
//        arrival rates

#include <Arduino.h>
#include <mh_arduinoPinHandler.h>
#include <mh_arduinoSerialTypes.h>
#include <mh_rs485WrapperClass.h>

using namespace mh_temperatureSensing;

#define _MH_PROGRAM_NAME_ "rs485 monitor"
#define _MH_VERSION_NUMBER_ "v0.001"

HardwareSerial& consoleSerial = Serial;
const long consoleDataRate = 115200L;

mh_temperatureSensing::
rs485WrapperClass<HardwareSerial> listener(Serial1,
                                           22,
                                           23,
                                           9600,
                                           8,
                                           PARITY_NONE,
                                           1);
unsigned long byteTxTime = 0;

void setup() {
  // start console serial
  consoleSerial.begin(consoleDataRate);
  delay(100);
  // report version number
  consoleSerial.println(_MH_PROGRAM_NAME_);
  consoleSerial.println(_MH_VERSION_NUMBER_);
  // configure for listening
  listener.rxEnable();
  // set delay to byte transmission time
  byteTxTime = listener.getByteTxTime();
  consoleSerial.println("initialization complete");
  consoleSerial.println("delaying 5 seconds before start");
  delay(5000);
} //end setup()


void loop() {

  // while data is available, read and output data
  while (listener.available() > 0)
  {
      int readByte = listener.read();
      if (readByte > 0)
      {
        unsigned char readChar = readByte;
        consoleSerial.print("received -");
        consoleSerial.print((char) readChar);
        consoleSerial.print("-, 0x");
        consoleSerial.println((unsigned char) readByte, HEX);
      } // end if (readByte > 0)
  } // end while (listener.available() > 0)

  delay(byteTxTime);

} // end loop()


*/

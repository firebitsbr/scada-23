#ifndef MH_RS485WRAPPERCLASS_H
#define MH_RS485WRAPPERCLASS_H

/*! \brief rs485WrapperClass is a wrapper class for implementation of
 *  RS-485 serial interfacees.  It is intended to allow common development
 *  of code for use across microcontrollers and supervisory systems,
 *  including servers and desktops.
 *
 *  The rs485WrapperClass template has been implemented for Arduino HardwareSerial.
 */
// Notes
// - By design, there is no constructor without arguments
//   pointer operations and intended actions require
//   association with an underlying serial type
/// - Neither the constructor nor initialize() configure
///   the transmit and receive pins or begin control of
///   the underlying RS-485 device.  start() must be called
///   to begin control of the RS-485 device.
/// - listen() and isListening() modeled after the arduino
///   functions of the same names.  They do not function like
///   the similarly named socket functions
///   listen() calls rxEnable() to put the RS-485 transceiver in
///   listening mode.
///   isListening() checks the pin levels for m_txEnablePin and
///   m_rxDisablePin, checks the value of m_transmitterIsOn,
///   and, if appropriate, checks the return of the base class
///   isListening() function.
/// - txEnable() and rxEnable() set the level of both pins,
///   m_txEnablePin and m_rxDisablePin.
///   txDisable() sets only the value of m_txEnablePin
///   rxDisable() sets only the value of m_rxEnablePin
///   The provision of these additional functions are provided
///   to support chips that allow turning off both transmit and
///   receive functions (e.g. a power saving mode)

// TODO: implementation for additional types (e.g. SoftwareSerial) will
//       require modification of default output implementation.  A possible
//       implementation is as a static member of the template class; however,
//       allocation of the default output wastes memory if not used.  Conversion
//       from a pointer to a reference (from *m_serialBasePtr to &m_serialBaseRef)
//       would eliminate the need for a default output.

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_rs485WrapperClass linux implementation incomplete"
#elif _WIN32
#error "mh_rs485WrapperClass windows implementation incomplete"
#else
#error "mh_rs485WrapperClass unknown compilation environment"
#endif // determine environment

#include <mh_arduinoSerialTypes.h>
#include <mh_serialHandlerClass.h>

// uncomment below to include test functions
#define MH_RS485WC_INCLUDE_TEST

namespace mh_temperatureSensing
{

template <class T_SerialBase>
class rs485WrapperClass:
    public serialHandlerClass
{
public:
    rs485WrapperClass(void);
    rs485WrapperClass(T_SerialBase &serialBase);
    rs485WrapperClass(T_SerialBase &serialBase,
                      const int txEnablePin,
                      const int rxDisablePin);
    rs485WrapperClass(T_SerialBase &serialBase,
                      const long int dataRate,
                      const int dataBits,
                      const parityEnum parityOption,
                      const int stopBits);
    rs485WrapperClass(T_SerialBase &serialBase,
                      const int txEnablePin,
                      const int rxDisablePin,
                      const long int dataRate,
                      const int dataBits,
                      const parityEnum parityOption,
                      const int stopBits);
    ~rs485WrapperClass();
    virtual int start(void);
    virtual void stop(void);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    virtual size_t write(uint8_t outByte);
    virtual int txEnable(void);
    int txDisable(void);
    virtual int rxEnable(void);
    int rxDisable(void);
    virtual bool listen(void);
    virtual bool isListening(void);
    int initialize(T_SerialBase &serialDevice,
                   const int txEnablePin = defaultTxPin,
                   const int rxDisablePin = defaultRxPin,
                   const long int dataRate = defaultDataRate,
                   const int dataBits = defaultDataBits,
                   const parityEnum parityOption = defaultParityOption,
                   const int stopBits = defaultStopBits);
    int initializeLocal(T_SerialBase &serialDevice,
                        const int txEnablePin,
                        const int rxDisablePin);
    void initializeNoAlloc(void);
protected:
    // no methods
protected:
    static const int defaultTxPin;
    static const int defaultRxPin;
    int m_txEnablePin;
    int m_rxDisablePin;
    boolean m_transmitterIsOn;
    T_SerialBase *m_serialBasePtr;
};


template class rs485WrapperClass<HardwareSerial>;

// specialization required to correct problem linking
// in Arduino IDE
template <> mh_temperatureSensing::
            rs485WrapperClass<HardwareSerial>::
            rs485WrapperClass(HardwareSerial &serialBase,
                              const int txEnablePin,
                              const int rxDisablePin,
                              const long int dataRate,
                              const int dataBits,
                              const parityEnum parityOption,
                              const int stopBits);

// specialization required to correct problem linking
// in Arduino IDE
template <> mh_temperatureSensing::
            rs485WrapperClass<HardwareSerial>::
            ~rs485WrapperClass();

// test code
#ifdef MH_RS485WC_INCLUDE_TEST
int  rs485WCTestLoopBack(HardwareSerial &outSerial,
                         const int outTxPin,
                         const int outRxPin,
                         HardwareSerial &inSerial,
                         const int inTxPin,
                         const int inRxPin,
                         HardwareSerial &testStream);
#endif // MH_RS485WC_INCLUDE_TEST

} // close namespace mh_temperatureSensing


#endif // MH_RS485WRAPPERCLASS_H

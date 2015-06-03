#ifndef SERIALWRAPPERCLASS_H
#define SERIALWRAPPERCLASS_H

/*! \brief serialWrapperClass is a template class intended to support
 *  implementation of wrapper classes such as hardwareSerialWrapperClass,
 * softwareSerialWrapperClass, rs485WrapperClass, with the objective
 * of accumulating classes for cross-platform development.
 *
 * serialWrapperClass is intended to allow common development of
 * code for use across microcontrollers and supervisory systems,
 * including servers and desktops.
 */
// static methods are provided where appropriate to allow use of class
// methods without a class instance
//
// Test code is included to generate debug output from microcontrollers.
//
// Why another serial wrapper?
// The purpose of this wrapper is to provide an abstraction that is only
// a small change from the native microcontroller types.  The more diverse,
// more capable and widely varied wrappers available for other platforms
// can be mapped easily into this wrapper, allowing code developed for
// execution on the microcontroller to be executed on larger, more capable
// systems.  The intended benefit is to allow a large part of any supervisory
// interface to be a natural biproduct of microcontroller code development.
//

// Notes:
// - swcDefaultSerial is defined to allow use of the base class without
//   error and without additional programming.  The reference shcDefaultStream
//   can be changed to any stream.
/// - Serial control functions (setDataRate(), setDataBits(), etc.) set member variables only.
///   To change serial operating parameters start() must be called changing member variables.
/// - Serial parameters are checked in conformDataRate() and constructSerialParameters() as part of the
///   start() call.  The calling value for data rate may be modified.
///   -- Default communication parameters are 9600-N-8-1.  Invalid calling parameters
///      result in substitution of the default value for that parameter.
///   -- start() will modify dataRate (or m_dataRate) to conform to permitted values
// - initialize return values:
//      -1 = error setting data rate
//      -2 = error setting data bits
//      -4 = error setting parity
//      -8 = error setting stop bits
//      error value is cumulative of all errors.
/// - This class has not been made thread safe.  Mutexes or other protections should be added
///   to using applications or wrapper classes if multithreaded use is anticipated.

// TODO: consider changing m_serialDevicePtr from a pointer to a reference.

// determine environment
#ifdef ARDUINO
    #include <Arduino.h>
    #include <mh_arduinoSerialTypes.h>
#elif __linux__
    #error "mh_serialWrapperClass linux implementation incomplete"
#elif _WIN32
    #error "mh_serialWrapperClass windows implementation incomplete"
#else
    #error "mh_serialWrapperClass unknown compilation environment"
#endif // determine environment

#include <mh_serialHandlerClass.h>


// uncomment below to include test code
// #define MH_SWC_INCLUDE_TEST


namespace mh_temperatureSensing
{

template <class T_SerialBase>
class serialWrapperClass:
    public serialHandlerClass
{
public:
    serialWrapperClass(T_SerialBase &serialDevice = swcDefaultSerial,
                       const long int dataRate = defaultDataRate,
                       const int dataBits = defaultDataBits,
                       const parityEnum parityOption = defaultParityOption,
                       const int stopBits = defaultStopBits);
    ~serialWrapperClass(void);
    virtual int start(void);
    virtual void stop(void);
    // methods required by Stream base class
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    virtual size_t write(uint8_t outByte);
    // methods required to wrap Arduino SoftwareSerial
    virtual bool listen(void);
    virtual bool isListening(void);
    // methods to implement other types of serial
    virtual int txEnable(void);
    virtual int rxEnable(void);
    // static methods for use without class instance
    static int start(T_SerialBase &serialDevice,
                     const long int dataRate = defaultDataRate,
                     const int dataBits = defaultDataBits,
                     const parityEnum parityOption = defaultParityOption,
                     const int stopBits = defaultStopBits);
    static void stop(T_SerialBase &serialDevice);
    // initialize
    int initialize(T_SerialBase &serialDevice = swcDefaultSerial,
                   const long int dataRate = defaultDataRate,
                   const int dataBits = defaultDataBits,
                   const parityEnum parityOption = defaultParityOption,
                   const int stopBits = defaultStopBits);
protected:
    // no methods
public:
    static const T_SerialBase& swcDefaultSerial;
    static const long int defaultDataRate;
    static const int defaultDataBits;
    static const parityEnum defaultParityOption;
    static const int defaultStopBits;
protected:
    T_SerialBase m_serialDevicePtr = &swcDefaultSerial;
}; // end class serialWrapperClass

#ifdef MH_SWC_INCLUDE_TEST
// test functions
template <class T_SerialBase>
int testLoopBack(unsigned long consoleRate,
                 T_SerialBase &outSerial,
                 T_SerialBase &inSerial);
#endif // MH_SWC_INCLUDE_TEST

} // close namespace mh_temperatureSensing



/*

// Recommended derived class methods
//
// Derived classes should support all methods required for
// operation of all intended serial types.  Exceptions for
// methods not implemented because they are not supported
// by all underlying implementations should be noted.

  // Supported methods:
  //
  // methods required to use HardwareSerial and SoftwareSerial classes
  //   void begin(unsigned long) and void begin(unsigned long, uint8_t) are
  //     addressed by start() and stop()
  // methods to use RS-485 and other 2-wire protocols
  //   virtual int txEnable(void) = 0;
  //   virtual int txDisable(void) = 0;
  //   virtual bool txIsEnabled(void) = 0;
  //   virtual int rxEnable(void) = 0;
  //   virtual int rxDisable(void) = 0;
  //   virtual bool rxIsEnabled(void) = 0;
  // methods to use SoftwareSerial
  //   bool listen(void) is mapped to rxEnable();
  //   isListening(void) is mapped to rxIsEnabled();

  // Unsupported methods
  // (not available using derived wrapper pointer)
  //
  // methods used by SoftwareSerial
  //   virtual bool overflow(void) = 0;

  //  derived class candidates
  //   stream wrappers
  //     - streambuff
  //     - http://libserial.sourceforge.net/
  //   fd wrappers
  //     - canonicalarduinoread.c
  //   general-purpose wrappers
  //     - boost asio

 */


#endif // SERIALWRAPPERCLASS_H

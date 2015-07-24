#ifndef MH_SERIALHANDLERCLASS_H
#define MH_SERIALHANDLERCLASS_H

/*! \brief serialHandlerClass is a base class intended for use in
 * wrapper classes such as hardwareSerialWrapperClass,
 * softwareSerialWrapperClass, rs485WrapperClass, with the objective
 * of accumulating classes for cross-platform development.
 *
 * serialHandlerClass is intended to allow common development of
 * code for use across microcontrollers and supervisory systems,
 * including servers and desktops.
 *
 * Static methods are provided where appropriate to allow use of class
 * methods without a class instance
 */

// shcDefaultStream allows direct use of the base class without extension
// NOTE - without extension the base class supports only a single,
//        effectively static, interface
//
// Test code is included to generate debug output from microcontrollers.
//
// Why another serial wrapper?
// The purpose of this handler and any supported wrappers is to provide
// an abstraction that is only a small change from the native microcontroller
// types.  The more diverse, more capable and widely varied wrappers available
// for other platforms can be mapped easily into this wrapper, allowing code
// developed for execution on the microcontroller to be executed on larger,
// more capable systems.  The intended benefit is to allow a large part of any
// supervisory interface to be a natural biproduct of microcontroller code
// development.
//

/// Usage notes:
// - shcDefaultStream is defined to allow use of the base class without
//   error and without additional programming.  The reference shcDefaultStream
//   can be changed to any stream.
/// - Serial control functions (setDataRate(), setDataBits(), etc.) set member variables only.
///   To change serial operating parameters start() must be called.
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

// determine environment
#ifdef ARDUINO
    #include <Arduino.h>
    #include "mh_arduinoSerialTypes.h"
#elif __linux__
    #error "mh_serialHandlerClass linux implementation incomplete"
#elif _WIN32
    #error "mh_serialHandlerClass windows implementation incomplete"
#else
    #error "mh_serialHandlerClass unknown compilation environment"
#endif // determine environment


// uncomment below to include test code
// #define MH_SHC_INCLUDE_TEST


namespace mh_temperatureSensing
{

// define shcDefaultStream for output when using the base class without extension
#ifdef ARDUINO
typedef HardwareSerial shcDefaultSerialType;
#elif __linux__
#error "linux default output not defined"
#elif _WIN32
#error "windows default output not defined"
#else
#error "unknown environment, default output not defined"
#endif // determine environment

class serialHandlerClass:
    public Stream
{
public:
    serialHandlerClass(void);
    serialHandlerClass(const long int dataRate,
                       const int dataBits,
                       const parityEnum parityOption,
                       const int stopBits);
    ~serialHandlerClass(void);
    long int setDataRate(const long int dataRate);
    int setDataBits(const int dataBits);
    parityEnum setParityOption(const parityEnum parityOption);
    int setStopBits(const int stopBits);
    virtual int start(void);
    virtual void stop(void);
    unsigned long getByteTxTime(void);
    // static methods for use without a class instance
    static int start(shcDefaultSerialType &serialDevice,
                     long int &dataRate,
                     const int dataBits,
                     const parityEnum parityOption,
                     const int stopBits);
    static void stop(shcDefaultSerialType &serialDevice);
    static unsigned long getByteTxTime(const long int dataRate,
                                       const int dataBits,
                                       const parityEnum parityOption,
                                       const int stopBits);
    // other methods for convenience in use
    int start(shcDefaultSerialType &serialDevice);
    // methods required by Stream base class
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    virtual size_t write(uint8_t outByte);
    int initialize(const long int dataRate,
                   const int dataBits,
                   const parityEnum parityOption,
                   const int stopBits);
    void initializeNoAlloc(void);
protected:
    // no methods
public:
    static const long int defaultDataRate;
    static const int defaultDataBits;
    static const parityEnum defaultParityOption;
    static const int defaultStopBits;
protected:
    long int m_dataRate;
    int m_dataBits;
    parityEnum m_paritySetting;
    int m_stopBits;

}; // end class serialHandlerClass

#ifdef MH_SHC_INCLUDE_TEST
// test functions
int testLoopBack(unsigned long consoleRate,
                 shcDefaultSerialType &outSerial,
                 shcDefaultSerialType &inSerial);
#endif // MH_SHC_INCLUDE_TEST

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



#endif // MH_SERIALHANDLERCLASS_H

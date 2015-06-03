// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_serialWrapperClass linux implementation incomplete"
#elif _WIN32
#error "mh_serialWrapperClass windows implementation incomplete"
#else
#error "mh_serialWrapperClass unknown compilation environment"
#endif // determine environment


#include "mh_arduinoSerialTypes.h"
#include "mh_serialWrapperClass.h"

// uncomment below as needed to debug methods
// debugStream must be defined for all debug blocks
// static Stream& debugStream = Serial;
// #define MH_SWC_DEBUG_DATARATE
// #define MH_SWC_DEBUG_DATABITS
// #define MH_SWC_DEBUG_PARITY
// #define MH_SWC_DEBUG_STOPBITS
// #define MH_SWC_DEBUG_START

// define default streams for each serial type
#ifdef ARDUINO
template <>
const HardwareSerial&
mh_temperatureSensing::serialWrapperClass<HardwareSerial>::
swcDefaultSerial = Serial;
#elif __linux__
// linux stream types here
#elif _WIN32
// windows stream types here
#else
// conditional statements for other environment
// other stream types here
#endif // define default streams


// define testStream for output of test data
#ifdef MH_SWC_INCLUDE_TEST
#ifdef ARDUINO
static HardwareSerial& testStream = Serial;
#elif __linux__
#error "linux test output not defined"
#elif _WIN32
#error "windows test output not defined"
#else
#error "unknown environment, test output not defined"
#endif // determine environment
#endif // MH_SWC_INCLUDE_TEST

template <class T_SerialBase>
const long int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
defaultDataRate = 9600L;

template <class T_SerialBase>
const int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
defaultDataBits = 8;

template <class T_SerialBase>
const mh_temperatureSensing::parityEnum
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
defaultParityOption = PARITY_NONE;

template <class T_SerialBase>
const int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
defaultStopBits = 1;


template <class T_SerialBase>
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
serialWrapperClass(T_SerialBase &serialDevice,
                   const long int dataRate,
                   const int dataBits,
                   const parityEnum parityOption,
                   const int stopBits)
{
    initialize(serialDevice,
               dataRate,
               dataBits,
               parityOption,
               stopBits);
} // end serialWrapperClass(const int dataRate, ... )


template <class T_SerialBase>
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
~serialWrapperClass(void)
{
    // no action in base class
} // end ~serialWrapperClass()


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
start(void)
{
    if (m_serialDevicePtr != NULL)
    {
        return start(*m_serialDevicePtr,
                     m_dataRate,
                     m_dataBits,
                     m_paritySetting,
                     m_stopBits);
    }
    else
    {
        return start(swcDefaultSerial,
                     m_dataRate,
                     m_dataBits,
                     m_paritySetting,
                     m_stopBits);
    } // end if (m_serialDevicePtr != NULL)
} // end int start(void)


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
start(T_SerialBase &serialDevice,
      const long int dataRate,
      const int dataBits,
      const parityEnum parityOption,
      const int stopBits)
{
    int retVal = 0;
#ifdef MH_SWC_DEBUG_START
    debugStream.flush();
    debugStream.print(F(" constructSerialParameters("));
    debugStream.print(dataBits);
    debugStream.print(F(" data bits, "));
    printParityEnumText(debugStream, parityOption);
    debugStream.print(F(", "));
    debugStream.print(stopBits);
    debugStream.println(F(" stop bits)"));
    debugStream.flush();
#endif // MH_SWC_DEBUG_START
    long int rateWorkVar = dataRate;
    conformDataRate(rateWorkVar);
    uint8_t serialParameters = constructSerialParameters(dataBits,
                               parityOption,
                               stopBits);
#ifdef MH_SWC_DEBUG_START
    debugStream.flush();
    debugStream.print(F("SWC end, begin "));
    debugStream.print(rateWorkVar);
    debugStream.print(F(", 0x"));
    debugStream.println(serialParameters, HEX);
    debugStream.flush();
#endif // MH_SWC_DEBUG_START
#ifdef ARDUINO
    serialDevice.end();
    serialDevice.begin(rateWorkVar, serialParameters);
#elif __linux__
#error "mh_serialWrapperClass linux start implementation incomplete"
#elif _WIN32
#error "mh_serialWrapperClass windows start implementation incomplete"
#else
#error "mh_serialWrapperClass unknown compilation environment, start implementation incomplete"
#endif // determine environment
    return retVal;
} // end int start(HardwareSerial &serialDevice, ... )


template <class T_SerialBase>
void
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
stop(void)
{
    if (m_serialDevicePtr != NULL)
    {
        stop(*m_serialDevicePtr);
    }
    else
    {
        stop(swcDefaultSerial);
    } // end if (m_serialDevicePtr != NULL)
} // end void stop(void)


template <class T_SerialBase>
void
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
stop(T_SerialBase &serialDevice)
{
#ifdef ARDUINO
    serialDevice.end();
#elif __linux__
#error "mh_serialWrapperClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialWrapperClass windows stop implementation incomplete"
#else
#error "mh_serialWrapperClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
} // end void stop(T_SerialBase)


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
available(void)
{
    if (m_serialDevicePtr != NULL)
    {
        return m_serialDevicePtr->available();
    }
    else
    {
        return swcDefaultSerial.available();
    } // end if (m_serialDevicePtr != NULL)
} // end available()


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
read(void)
{
    if (m_serialDevicePtr != NULL)
    {
        return m_serialDevicePtr->read();
    }
    else
    {
        return swcDefaultSerial.read();
    } // end if (m_serialDevicePtr != NULL)
} // end read()


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
peek(void)
{
    if (m_serialDevicePtr != NULL)
    {
        return m_serialDevicePtr->peek();
    }
    else
    {
        return swcDefaultSerial.peek();
    } // end if (m_serialDevicePtr != NULL)
} // end peek()


template <class T_SerialBase>
void
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
flush(void)
{
    if (m_serialDevicePtr != NULL)
    {
        return m_serialDevicePtr->flush();
    }
    else
    {
        return swcDefaultSerial.flush();
    } // end if (m_serialDevicePtr != NULL)
} // end flush()


template <class T_SerialBase>
size_t
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
write(uint8_t outByte)
{
    if (m_serialDevicePtr != NULL)
    {
        return m_serialDevicePtr->write(outByte);
    }
    else
    {
        return swcDefaultSerial.write(outByte);
    } // end if (m_serialDevicePtr != NULL)
} // end write()


template <class T_SerialBase>
int
mh_temperatureSensing::serialWrapperClass<T_SerialBase>::
initialize(T_SerialBase &serialDevice,
           const long int dataRate,
           const int dataBits,
           const parityEnum parityOption,
           const int stopBits)
{
    int retVal = 0;
    m_serialDevicePtr = &serialDevice;
    setDataRate(dataRate);
    setDataBits(dataBits);
    setParityOption(parityOption);
    setStopBits(stopBits);
    if (m_dataRate != dataRate)
    {
        retVal += -1;
    }
    if (m_dataBits != dataBits)
    {
        retVal += -2;
    }
    if (m_paritySetting != parityOption)
    {
        retVal += -4;
    }
    if (m_stopBits != stopBits)
    {
        retVal += -8;
    }
    return retVal;
} // end initialize(const int dataRate, ... )



#ifdef MH_SWC_INCLUDE_TEST

// test code
template <class T_SerialBase>
int
mh_temperatureSensing<T_SerialBase>::
testLoopBack(unsigned long consoleRate,
             T_SerialBase &outSerial,
             T_SerialBase &inSerial)
{
    int retVal = 0;
    serialWrapperClass srOne(19200, 8, PARITY_NONE, 1);
    serialWrapperClass srTwo(19200, 8, PARITY_NONE, 1);
    testStream.begin(consoleRate);
    srOne.start(outSerial);
    srTwo.start(inSerial);
    delay(50);
    testStream.println("begin mh_serialWrapperClass loopback test");
    for (int i=0; i<10; i++)
    {
        char writeByte = 'a' + i;
        char readByte = 0;
        testStream.print("sending -");
        testStream.print((char) writeByte);
        testStream.print("-   ");
        outSerial.write(writeByte);
        delay(2);
        readByte = inSerial.read();
        testStream.print("received -");
        testStream.print((char) readByte);
        testStream.println("-");
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
    testStream.println("end of mh_serialWrapperClass loopback test");
    return retVal;
} // end int testLoopBack()

// sample call for test code
// int commRet = mh_temperatureSensing::exerciseCommandInterfaceClassReadWrite(Serial1, Serial2);

#endif // MH_SWC_INCLUDE_TEST



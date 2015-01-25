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


#include "mh_serialHandlerClass.h"


// uncomment below as needed to debug methods
// debugStream must be defined for all debug blocks
// static Stream& debugStream = Serial;
// #define MH_SHC_DEBUG_DATARATE
// #define MH_SHC_DEBUG_DATABITS
// #define MH_SHC_DEBUG_PARITY
// #define MH_SHC_DEBUG_STOPBITS
// #define MH_SHC_DEBUG_START

mh_temperatureSensing::shcDefaultSerialType& shcDefaultStream = Serial;

// define testStream for output of test data
#ifdef MH_SHC_INCLUDE_TEST
#ifdef ARDUINO
static HardwareSerial& testStream = Serial;
#elif __linux__
#error "linux test output not defined"
#elif _WIN32
#error "windows test output not defined"
#else
#error "unknown environment, test output not defined"
#endif // determine environment
#endif // MH_SHC_INCLUDE_TEST

const long int
mh_temperatureSensing::serialHandlerClass::
defaultDataRate = 9600L;

const int
mh_temperatureSensing::serialHandlerClass::
defaultDataBits = 8;

const mh_temperatureSensing::parityEnum
mh_temperatureSensing::serialHandlerClass::
defaultParityOption = PARITY_NONE;

const int
mh_temperatureSensing::serialHandlerClass::
defaultStopBits = 1;


mh_temperatureSensing::serialHandlerClass::
serialHandlerClass(void)
{
    initialize(defaultDataRate,
               defaultDataBits,
               defaultParityOption,
               defaultStopBits);
} // end serialHandlerClass(void)


mh_temperatureSensing::serialHandlerClass::
serialHandlerClass(const long int dataRate,
                   const int dataBits,
                   const parityEnum parityOption,
                   const int stopBits)
{
    initialize(dataRate,
               dataBits,
               parityOption,
               stopBits);
} // end serialHandlerClass(const int dataRate, ... )


mh_temperatureSensing::serialHandlerClass::
~serialHandlerClass(void)
{
    // no action in base class
} // end ~serialHandlerClass()


long int
mh_temperatureSensing::serialHandlerClass::
setDataRate(const long int dataRate)
{
    long int retVal = m_dataRate;
    m_dataRate = dataRate;
#ifdef MH_SHC_DEBUG_DATARATE
    debugStream.flush();
    debugStream.print(F("SHC at "));
    debugStream.print((long int) this);
    debugStream.print(F(" m_dataRate set to "));
    debugStream.print(m_dataRate);
    debugStream.print(F(", was "));
    debugStream.println(retVal);
    debugStream.flush();
#endif // MH_SHC_DEBUG_DATARATE
    return retVal;
} // end int setDataRate(const int dataRate)


int
mh_temperatureSensing::serialHandlerClass::
setDataBits(const int dataBits)
{
    int retVal = m_dataBits;
    m_dataBits = dataBits;
#ifdef MH_SHC_DEBUG_DATABITS
    debugStream.flush();
    debugStream.print(F("SHC at "));
    debugStream.print((long int) this);
    debugStream.print(F(" m_dataBits set to "));
    debugStream.print(m_dataBits);
    debugStream.print(F(", was "));
    debugStream.println(retVal);
    debugStream.flush();
#endif // MH_SHC_DEBUG_DATABITS
    return retVal;
} // end int setdataBits(const int dataBits)


mh_temperatureSensing::parityEnum
mh_temperatureSensing::serialHandlerClass::
setParityOption(const parityEnum parityOption)
{
    parityEnum retVal = m_paritySetting;
    m_paritySetting = parityOption;
#ifdef MH_SHC_DEBUG_PARITY
    debugStream.flush();
    debugStream.print(F("SHC at "));
    debugStream.print((long int) this);
    debugStream.print(F(" m_paritySetting set to "));
    printParityEnumText(debugStream, m_paritySetting);
    debugStream.print(F(", was "));
    printParityEnumText(debugStream, retVal);
    debugStream.println();
    debugStream.flush();
#endif // MH_SHC_DEBUG_PARITY
    return retVal;
} // end int setParityOption(const parityEnum parityOption)


int
mh_temperatureSensing::serialHandlerClass::
setStopBits(const int stopBits)
{
    int retVal = m_stopBits;
    m_stopBits = stopBits;
#ifdef MH_SHC_DEBUG_STOPBITS
    debugStream.flush();
    debugStream.print(F("SHC at "));
    debugStream.print((long int) this);
    debugStream.print(F(" m_stopBits set to "));
    debugStream.print(m_stopBits);
    debugStream.print(F(", was "));
    debugStream.println(retVal);
    debugStream.flush();
#endif // MH_SHC_DEBUG_STOPBITS
    return retVal;
} // end int setStopBits(const int stopBits)


int
mh_temperatureSensing::serialHandlerClass::
start(void)
{
    return start(shcDefaultStream);
} // end int start(void)


int
mh_temperatureSensing::serialHandlerClass::
start(shcDefaultSerialType &serialDevice)
{
#ifdef MH_SHC_DEBUG_START
    debugStream.flush();
    debugStream.print(F("SHC at "));
    debugStream.print((long int) this);
#endif // MH_SHC_DEBUG_START
    return start(serialDevice,
                 m_dataRate,
                 m_dataBits,
                 m_paritySetting,
                 m_stopBits);
} // end int start(HardwareSerial &serialDevice)


int
mh_temperatureSensing::serialHandlerClass::
start(shcDefaultSerialType &serialDevice,
      long int &dataRate,
      const int dataBits,
      const parityEnum parityOption,
      const int stopBits)
{
    int retVal = 0;
#ifdef MH_SHC_DEBUG_START
    debugStream.flush();
    debugStream.print(F(" constructSerialParameters("));
    debugStream.print(dataBits);
    debugStream.print(F(" data bits, "));
    printParityEnumText(debugStream, parityOption);
    debugStream.print(F(", "));
    debugStream.print(stopBits);
    debugStream.println(F(" stop bits)"));
    debugStream.flush();
#endif // MH_SHC_DEBUG_START
    conformDataRate(dataRate);
    uint8_t serialParameters = constructSerialParameters(dataBits,
                               parityOption,
                               stopBits);
#ifdef MH_SHC_DEBUG_START
    debugStream.flush();
    debugStream.print(F("SHC end, begin "));
    debugStream.print(dataRate);
    debugStream.print(F(", 0x"));
    debugStream.println(serialParameters, HEX);
    debugStream.flush();
#endif // MH_SHC_DEBUG_START
#ifdef ARDUINO
    serialDevice.end();
    serialDevice.begin(dataRate, serialParameters);
#elif __linux__
#error "mh_serialHandlerClass linux start implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows start implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, start implementation incomplete"
#endif // determine environment
    return retVal;
} // end int start(HardwareSerial &serialDevice, ... )


void
mh_temperatureSensing::serialHandlerClass::
stop(void)
{
    stop(shcDefaultStream);
} // end void stop(void)


void
mh_temperatureSensing::serialHandlerClass::
stop(shcDefaultSerialType &serialDevice)
{
#ifdef ARDUINO
    serialDevice.end();
#elif __linux__
#error "mh_serialHandlerClass linux stop implementation incomplete"
#elif _WIN32
#error "mh_serialHandlerClass windows stop implementation incomplete"
#else
#error "mh_serialHandlerClass unknown compilation environment, stop implementation incomplete"
#endif // determine environment
} // end void stop(shcDefaultSerialType)


unsigned long
mh_temperatureSensing::serialHandlerClass::
getByteTxTime(void)
{
    return getByteTxTime(m_dataRate,
                         m_dataBits,
                         m_paritySetting,
                         m_stopBits);
} // end unsigned long getByteTxTime(void)


unsigned long
mh_temperatureSensing::serialHandlerClass::
getByteTxTime(const long int dataRate,
              const int dataBits,
              const parityEnum parityOption,
              const int stopBits)
{
    // calculate time to transmit one byte
    uint64_t bitsPerSerialByte = dataBits + stopBits;
    if (parityOption != PARITY_NONE)
    {
        bitsPerSerialByte++;
    } // end if (paritySetting != PARITY_NONE)
    unsigned long retVal = (unsigned long) ( ((uint64_t) 1000000ull * bitsPerSerialByte) /
                           ((uint64_t) dataRate) );
    return retVal;
} // end unsigned long getByteTxTime(const long int dataRate, ... )


int
mh_temperatureSensing::serialHandlerClass::
available(void)
{
    return shcDefaultStream.available();
} // end available()


int
mh_temperatureSensing::serialHandlerClass::
read(void)
{
    return shcDefaultStream.read();
} // end read()


int
mh_temperatureSensing::serialHandlerClass::
peek(void)
{
    return shcDefaultStream.peek();
} // end peek()


void
mh_temperatureSensing::serialHandlerClass::
flush(void)
{
    shcDefaultStream.flush();
} // end flush()


size_t
mh_temperatureSensing::serialHandlerClass::
write(uint8_t outByte)
{
    return shcDefaultStream.write(outByte);
} // end write()


int
mh_temperatureSensing::serialHandlerClass::
initialize(const long int dataRate,
           const int dataBits,
           const parityEnum parityOption,
           const int stopBits)
{
    int retVal = 0;
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


void
mh_temperatureSensing::serialHandlerClass::
initializeNoAlloc(void)
{
    m_dataRate = defaultDataRate;
    m_dataBits = defaultDataBits;
    m_paritySetting = defaultParityOption;
    m_stopBits = defaultStopBits;
} // end


#ifdef MH_SHC_INCLUDE_TEST

// test code
int
mh_temperatureSensing::
testLoopBack(unsigned long consoleRate,
             HardwareSerial &outSerial,
             HardwareSerial &inSerial)
{
    int retVal = 0;
    serialHandlerClass srOne(19200, 8, PARITY_NONE, 1);
    serialHandlerClass srTwo(19200, 8, PARITY_NONE, 1);
    testStream.begin(consoleRate);
    srOne.start(outSerial);
    srTwo.start(inSerial);
    delay(50);
    testStream.println("begin mh_serialHandlerClass loopback test");
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
    testStream.println("end of mh_serialHandlerClass loopback test");
    return retVal;
} // end int testLoopBack()

// sample call for test code
// int commRet = mh_temperatureSensing::exerciseCommandInterfaceClassReadWrite(Serial1, Serial2);

#endif // MH_SHC_INCLUDE_TEST



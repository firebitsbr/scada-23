#include <stdint.h>
#include <stddef.h>

#include <mh_versionNumberType.h>
#include <mh_arduinoTimeFunctions.h>
#include <mh_arduinoPinHandler.h>
#include <mh_arduinoSerialTypes.h>
#include <mh_serialHandlerClass.h>
#include <mh_sarc_encodeData.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>
#include <mh_sac_sensirionArrayTypes.h>


using mh_timeFunctions::microsElapsed;

// test code

// simple loop-back test - checks for connectivity
int
mh_temperatureSensing::
cicTestLoopBack(HardwareSerial &outSerial,
                const int outTxPin,
                const int outRxPin,
                HardwareSerial &inSerial,
                const int inTxPin,
                const int inRxPin,
                HardwareSerial &testStream)
{
    int retVal = 0;
    commandInterfaceClass cicIn(1, 64,
                                inSerial, inTxPin, inRxPin,
                                9600, 8, PARITY_NONE, 1);
    commandInterfaceClass cicOut(2, 64,
                                 outSerial, outTxPin, outRxPin,
                                 9600, 8, PARITY_NONE, 1);
    cicIn.start();
    cicIn.rxEnable();
    cicOut.start();
    cicOut.txEnable();
    testStream.println("CIC devices started, delay 5 seconds");
    delay(5000);
    testStream.println("begin CIC loopback test");
    for (int i=0; i<10; i++)
    {
        char writeByte = 'a' + i;
        char readByte = 0;
        testStream.print("sending -");
        testStream.print((char) writeByte);
        testStream.print("-   ");
        cicOut.write(writeByte);
        delay(2);
        readByte = cicIn.read();
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
    testStream.println("end of commandInterfaceClass loopback test");
    return retVal;
} // end int testLoopBack()


// test sending and receiving messages
// intended for use on Mega 2650 or similar
int
mh_temperatureSensing::
exerciseCICReadWrite(HardwareSerial &oneSerial,
                     const int oneTxPin,
                     const int oneRxPin,
                     HardwareSerial &twoSerial,
                     const int twoTxPin,
                     const int twoRxPin,
                     HardwareSerial &testStream)
{
    int retVal = 0;
    // Class data
    testStream.println(F("Class data:"));
    testStream.flush();
    testStream.print(F("  Sync value              0x"));
    testStream.println(syncValue, HEX);
    testStream.flush();
    testStream.print(F("  Max buffer size         "));
    testStream.println(cicSizeTypeMax);
    testStream.flush();
    testStream.print(F("  Header size             "));
    testStream.println(commandInterfaceClass::headerSize);
    testStream.flush();
    testStream.print(F("  Framing size            "));
    testStream.println(commandInterfaceClass::framingSize);
    testStream.flush();
    testStream.print(F("  Max payload size        "));
    testStream.println(commandInterfaceClass::maxPayloadSize);
    testStream.flush();
    testStream.print(F("  Default data rate       "));
    testStream.println(commandInterfaceClass::defaultDataRate);
    testStream.flush();
    testStream.print(F("  Default data bits       "));
    testStream.println(commandInterfaceClass::defaultDataBits);
    testStream.flush();
    testStream.print(F("  Default parity bits     "));
    retVal += printParityEnumText(testStream, commandInterfaceClass::defaultParityOption);
    retVal += testStream.println();
    testStream.print(F("  Default stop bits       "));
    testStream.println(commandInterfaceClass::defaultStopBits);
    testStream.flush();
    testStream.print(F("  Max RX bytes waiting    "));
    testStream.println(commandInterfaceClass::hardwareSerialMaxBytes);
    testStream.flush();
    testStream.print(F("  RX line clear intervals "));
    testStream.println(commandInterfaceClass::lineClearByteIntervals);
    testStream.flush();
    testStream.print(F("  RX timeout intervals    "));
    testStream.println(commandInterfaceClass::readTimeoutByteIntervals);
    testStream.flush();
    // allocation and related assignment
    const CIC_ADDRESS_TYPE addressOne = 1;
    const CIC_ADDRESS_TYPE addressTwo = 2;
    const CIC_SIZE_TYPE bufferSize = 64;
    commandInterfaceClass cicOne(addressOne, bufferSize,
                                 oneSerial, oneTxPin, oneRxPin,
                                 9600, 8, PARITY_NONE, 1);
    commandInterfaceClass cicTwo(addressTwo, bufferSize,
                                 twoSerial, twoTxPin, twoRxPin,
                                 9600, 8, PARITY_NONE, 1);
    testStream.print("cicOne address ");
    testStream.println((long int) &cicOne);
    testStream.print("m_dataBits = ");
    testStream.println(cicOne.m_dataBits);
    testStream.flush();
    testStream.print("cicTwo address ");
    testStream.println((long int) &cicTwo);
    testStream.print("m_dataBits = ");
    testStream.println(cicTwo.m_dataBits);
    testStream.flush();
    testStream.print(F("  TX micros per byte   "));
    testStream.println(cicOne.getByteTxTime());
    cicOne.setAddress(addressOne);
    cicOne.allocateBuffer(bufferSize);
    if (cicOne.getAddress() != addressOne)
    {
        testStream.print(F("Error - Incorrect address cicOne "));
        testStream.print(cicOne.getAddress());
        testStream.print(F(", should be "));
        testStream.println(addressOne);
        retVal |= 1;
    }
    if (cicTwo.getAddress() != addressTwo)
    {
        testStream.print(F("Error - Incorrect address cicTwo "));
        testStream.print(cicTwo.getAddress());
        testStream.print(F(", should be "));
        testStream.println(addressTwo);
        retVal |= 2;
    }
    if (cicOne.m_payloadBufferPtr == NULL)
    {
        testStream.println(F("Error - cicOne buffer pointer is null"));
        retVal |= 4;
    }
    if (cicOne.m_bufferSource != ALLOCATED)
    {
        testStream.print(F("Error - cicOne buffer source is "));
        printBufferSourceText(testStream, cicOne.m_bufferSource);
        testStream.print(F(", should be "));
        printBufferSourceText(testStream, ALLOCATED);
        testStream.println();
        retVal |= 8;
    }
    if (cicTwo.m_payloadBufferPtr == NULL)
    {
        testStream.println(F("Error - cicTwo buffer pointer is null"));
        retVal |= 16;
    }
    if (cicTwo.m_bufferSource != ALLOCATED)
    {
        testStream.print(F("Error - cicTwo buffer source is "));
        printBufferSourceText(testStream, cicTwo.m_bufferSource);
        testStream.print(F(", should be "));
        printBufferSourceText(testStream, ALLOCATED);
        testStream.println();
        retVal |= 32;
    }
    char newBuffer[bufferSize];
    cicTwo.setBufferByReference(bufferSize, newBuffer);
    uint32_t ptrVal = 0;
    if (cicTwo.m_payloadBufferPtr != newBuffer)
    {
        testStream.print(F("Error - cicTwo buffer pointer is 0x"));
        ptrVal = (uint32_t) cicTwo.m_payloadBufferPtr;
        testStream.print(ptrVal, HEX);
        testStream.print(F(", should be 0x"));
        ptrVal = (uint32_t) &(newBuffer[0]);
        testStream.println(ptrVal);
        retVal |= 64;
    }
    if (cicTwo.m_bufferSource != BY_REFERENCE)
    {
        testStream.print(F("Error - cicTwo buffer source is "));
        printBufferSourceText(testStream, cicTwo.m_bufferSource);
        testStream.print(F(", should be "));
        printBufferSourceText(testStream, BY_REFERENCE);
        testStream.println();
        retVal |= 128;
    }
    testStream.println(F("cicOne:"));
    printCicMembers(testStream, &cicOne);
    testStream.println(F("cicTwo:"));
    printCicMembers(testStream, &cicTwo);
    // set data rates and start serial interfaces
    cicOne.setDataRate(19200);
    cicTwo.setDataRate(19200);
    cicOne.start();
    cicTwo.start();
    delay(100);
    if (cicOne.m_payloadBufferPtr != NULL)
    {
        const size_t testStringLength = 17;
        cicOne.m_outgoingMessageType = CIC_MSG_DATA;
        char testString[testStringLength] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 0 };
        memcpy((void *) (cicOne.m_payloadBufferPtr), (void *) testString, testStringLength);
        cicOne.m_payloadByteCounter = testStringLength - 1;
    }
    else
    {
        testStream.println("Error - crcOne->m_payloadBufferPtr is NULL");
    } // end if (crcOne->m_payloadBufferPtr != NULL)


    testStream.println();
    testStream.println("Testing sendMessage()");
    int bytesWritten = 0;
    CIC_ERROR_TYPE commReturn = cicOne.sendMessage(bytesWritten,
                                addressTwo,
                                CIC_MSG_DATA);
    testStream.print("cicOne wrote ");
    testStream.print(bytesWritten);
    testStream.println(" bytes");
    testStream.print("return value ");
    printCicErrorTypeText(testStream, commReturn);
    testStream.println();
    testStream.println();
    delay(100);

    testStream.println("Testing readMessageData()");
    boolean messageReady = false;
    int bytesThisRead = 0;
    cicTwo.resetForNewRead();
    cicTwo.readMessageData(messageReady,
                           bytesThisRead);
    testStream.println(F("cicTwo:"));
    printCicMembers(testStream, &cicOne);
    printCicBuffer(testStream, &cicOne);
    testStream.println();
    testStream.println();
    testStream.println(F("cicTwo:"));
    printCicMembers(testStream, &cicTwo);
    printCicBuffer(testStream, &cicTwo);
    testStream.println();
    testStream.println();
    return 0;
} // end exerciseCommandInterfaceClassReadWrite(Stream &serialDevice, ... )


// test crc computation
// based on open source crc implementation from
// http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
int
mh_temperatureSensing::
exerciseCommandInterfaceClassCrc(Stream &serialDevice,
                                 commandInterfaceClass *cicPtr)
{
    int retVal = 0;
    const int testStringLength = 10;
    char testString[testStringLength] = "123456789";
    retVal += serialDevice.println("Exercise CRC computation");
    retVal += serialDevice.print("Polynomial is ");
    retVal += serialDevice.println(crcPolynomial, HEX);
    retVal += serialDevice.print("CRC of \"");
    retVal += serialDevice.print(testString);
    retVal += serialDevice.println("\"");
    CIC_CRC_TYPE crcValue = crcInitialValue;
    int crcReturn = cicPtr->crcInit(crcValue);
    retVal += serialDevice.print("crcInit() returned ");
    retVal += serialDevice.println(crcReturn);
    crcReturn = cicPtr->updateCrc(crcValue, (const uint8_t*) testString, (testStringLength - 1));
    retVal += serialDevice.print("crcUpdate() returned ");
    retVal += serialDevice.println(crcReturn);
    crcReturn = cicPtr->completeCrc(crcValue);
    retVal += serialDevice.print("crcComplete() returned ");
    retVal += serialDevice.println(crcReturn);
    retVal += serialDevice.print("CRC value is ");
    retVal += serialDevice.println(crcValue, HEX);
    return retVal;
} // end exerciseCommandInterfaceClassCrc(void)


/*

// simple test driver for CIC loopback test and read/write check

#include <Arduino.h>
#include <Sensirion.h>
#include <MemoryFree.h>
#include <mh_arduinoPinHandler.h>
#include <mh_rs485WrapperClass.h>
#include <commandInterfaceClass.h>

using namespace mh_temperatureSensing;

#define _MH_PROGRAM_NAME_ "CIC loopback"
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
  int commRet = mh_temperatureSensing::cicTestLoopBack(Serial1, 22, 23, Serial2, 24, 25, Serial);
  consoleSerial.print("loopback test complete, return value = ");
  consoleSerial.println("delaying 2 seconds");
  delay(2000);
  commRet = mh_temperatureSensing::exerciseCICReadWrite(Serial1, 22, 23, Serial2, 24, 25, Serial);
  consoleSerial.print("CIC read/write check complete, return value = ");
  consoleSerial.println(commRet);
} //end setup()


void loop() {

  delay(1000);

} // end loop()


*/

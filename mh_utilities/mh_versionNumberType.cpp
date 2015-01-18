#include "mh_versionNumberType.h"

// implementation uses AVR libc void* memcpy_PF(void *dest, uint_farptr_t src, size_t len)

void
mh_temperatureSensing::
decodeVersionNumber(const MH_VERSION_TYPE version,
                    int &majorVersion,
                    int &minorVersion,
                    int &revision,
                    char &build)
{
    majorVersion = (int) version[0];
    minorVersion = (int) version[1];
    revision = (int) version[2];
    build = (char) version[3];
}


int
mh_temperatureSensing::
encodeVersionNumber(char* bufferPtr,
                    const size_t bufferSize,
                    size_t &bytesUsed,
                    const MH_VERSION_TYPE version)
{
    int retVal = 0;
    // set initial values for parameters passed by reference
    bytesUsed = 0;
    // check input parameters
    if (bufferPtr == NULL)
    {
        retVal += -1;
    }
    if (bufferSize < MH_VERSION_TYPE_SIZE)
    {
        retVal += -2;
    }
    // end check inupt values
    if (retVal == 0)
    {
        // AVR libc void* memcpy_PF(void *dest, uint_farptr_t src, size_t len)
        // an alternative implementation could use pgm_read_byte() in a loop
        for (size_t i=0; i<MH_VERSION_TYPE_SIZE; i++)
        {
            bufferPtr[i] = version[i];
        } // end for (i)
        // memcpy_PF((void*) bufferPtr, version, MH_VERSION_TYPE_SIZE);
        bytesUsed = MH_VERSION_TYPE_SIZE;
    } // end if
    return retVal;
} // end encodeVersionNumber(char* bufferPtr, ... )


void
mh_temperatureSensing::
setVersionNumber(MH_VERSION_TYPE &version,
                 const int majorVersion,
                 const int minorVersion,
                 const int revision,
                 const char build)
{
    version[0] = (char) majorVersion;
    version[1] = (char) minorVersion;
    version[2] = (char) revision;
    version[3] = build;
} // end void encodeVersionNumber(MH_VERSION_TYPE &version, ... )


int
mh_temperatureSensing::
outputVersionNumber(Stream &serialDevice,
                    const MH_VERSION_TYPE version)
{
    int retVal = 0;
    retVal += serialDevice.print( (int) version[0] );
    retVal += serialDevice.print(".");
    retVal += serialDevice.print( (int) version[1] );
    retVal += serialDevice.print(".");
    retVal += serialDevice.print( (int) version[2] );
    retVal += serialDevice.print( (char) version[3] );
    return retVal;
} // end int outputVersionNumber(Stream &serialDevice, ... )

// test methods
#ifdef MH_TEST_VNT

int mh_temperatureSensing::
testVersionNumber(Stream &serialDevice)
{
    int retVal = 0;
    serialDevice.println("begin test versionNumberType");
    // allocate
    MH_VERSION_TYPE version = { 1, 2, 3, 'a' };
    serialDevice.println("version number allocated as 1.2.3a");
    if ( (version[0] != 1) ||
         (version[1] != 2) ||
         (version[2] != 3) ||
         (version[3] != 'a')    )
    {
        serialDevice.println("ERROR - allocation error");
        retVal -= 1;
    }
    // print
    serialDevice.print("version prints as ");
    int bytesWritten = outputVersionNumber(serialDevice, version);
    serialDevice.print(", ");
    serialDevice.print(bytesWritten);
    serialDevice.println(" bytes written");
    // encode
    setVersionNumber(version, 4, 5, 6, 'b');
    serialDevice.println("version number set to 4.5.6b");
    if ( (version[0] != 4) ||
         (version[1] != 5) ||
         (version[2] != 6) ||
         (version[3] != 'b')    )
    {
        serialDevice.println("ERROR - set error");
        retVal -= 2;
    }
    serialDevice.print("version prints as ");
    outputVersionNumber(serialDevice, version);
    serialDevice.println();
    // decode
    int majorVersion = 0;
    int minorVersion = 0;
    int revision = 0;
    char build = ' ';
    decodeVersionNumber(version,
                        majorVersion,
                        minorVersion,
                        revision,
                        build);
    if ( (majorVersion != 4) ||
         (minorVersion != 5) ||
         (revision != 6) ||
         (build != 'b')    )
    {
        serialDevice.println("ERROR - decoding error");
        retVal -= 4;
    }
    serialDevice.print("version number decodes as ");
    serialDevice.print(majorVersion);
    serialDevice.print(".");
    serialDevice.print(minorVersion);
    serialDevice.print(".");
    serialDevice.print(revision);
    serialDevice.print(build);
    serialDevice.println();
    char versionArray[MH_VERSION_TYPE_SIZE] = { 0 };
    size_t bytesEncoded = 0;
    int encodeRet = encodeVersionNumber(versionArray,
                                        MH_VERSION_TYPE_SIZE,
                                        bytesEncoded,
                                        version);
    if (encodeRet != 0)
    {
        serialDevice.print("ERROR - encodeVersionNumber() returned ");
        serialDevice.println(encodeRet);
    }
    serialDevice.print("version number encoded as ");
    for (size_t i=0; i<MH_VERSION_TYPE_SIZE; i++)
    {
        serialDevice.print("0x");
        serialDevice.print(versionArray[i], HEX);
        if (i < (MH_VERSION_TYPE_SIZE - 1))
        {
            serialDevice.print(", ");
        } // end if ()
    } // end for (i)
    serialDevice.println();
    serialDevice.println("end test versionNumberType");
    return retVal;
} // end int testVersionNumber()


// recommended test code
// for use in calling test method
/*
  int retVal = mh_temperatureSensing::testVersionNumber(Serial);
  if (retVal != 0)
  {
    Serial.print("ERROR - ");
  }
  Serial.print("version number test returned ");
  Serial.print(retVal);
  Serial.println();
*/

#endif // MH_TEST_VNT

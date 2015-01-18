#ifndef VERSIONNUMBERTYPE_H_INCLUDED
#define VERSIONNUMBERTYPE_H_INCLUDED

#include <Arduino.h>
#include <avr/pgmspace.h>

// MH_VERSION_TYPE is a 4-byte string in program memory
// recommended use is major version, minor version, revision
// and revision letter, e.g. 2.0.1a encoded as {0x02, 0x00, 0x01, 'a'}
//
// decodeVersionNumber() places the four bytes of version in variables for
//   majorVersion, minorVersion, revision and build
//
// encodeVersionNumber() copies version to a 4-byte array at bufferPtr
//   calling variable bytesUsed records the number of bytes assigned by
//   the operation.  Return values are 0 = success, -1 = null pointer,
//   -2 insufficient buffer size, -3 both errors.
//
// outputVersionNumber() prints version in '.' delimited format,
//   e.g. 2.0.1a.  Returns the number of bytes printed.
//
// setVersionNumber() is provided in the event that member initialization
//   cannot be accomplished by assigment at declaration
//   e.g. MH_VERSION_TYPE version = { 1, 2, 3, 'a' };
//   (not anticipated and verified in testing, but covered as a possibility)
//

// class Stream required methods are:
// Stream.print(char)
// Stream.print(int)

// implementation uses AVR libc void* memcpy_PF(void *dest, uint_farptr_t src, size_t len)

// uncomment this line to include test methods
#define MH_TEST_VNT

namespace mh_temperatureSensing
{

// sensor version
const size_t MH_VERSION_TYPE_SIZE = 4;
typedef char PROGMEM MH_VERSION_TYPE[MH_VERSION_TYPE_SIZE];
// recommended use is major version, minor version, revision
// and revision letter, e.g. 2.0.1a encoded as {0x02, 0x00, 0x01, 'a'}


void decodeVersionNumber(const MH_VERSION_TYPE version,
                         int &majorVersion,
                         int &minorVersion,
                         int &revision,
                         char &build);


int encodeVersionNumber(char* bufferPtr,
                        const size_t bufferSize,
                        size_t &bytesUsed,
                        const MH_VERSION_TYPE version);
// bytesUsed is set to the number of bytes encoded
// returns
//  0 = success
// -1 = null pointer
// -2 = insufficient buffer size
// -3 = both error conditions


int outputVersionNumber(Stream &serialDevice,
                        const MH_VERSION_TYPE version);
// returns bytes printed


void setVersionNumber(MH_VERSION_TYPE &version,
                      const int majorVersion,
                      const int minorVersion,
                      const int revision,
                      const char build);


// test methods
#ifdef MH_TEST_VNT

int testVersionNumber(Stream &serialDevice);

#endif // MH_TEST_VNT

} // close namespace mh_temperatureSensing

#endif // VERSIONNUMBERTYPE_H_INCLUDED

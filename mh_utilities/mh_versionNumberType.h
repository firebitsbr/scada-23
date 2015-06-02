#ifndef VERSIONNUMBERTYPE_H_INCLUDED
#define VERSIONNUMBERTYPE_H_INCLUDED

/*! \brief MH_VERSION_TYPE and supporting methods are provided to support
 *  configuration identification.
 *
 * MH_VERSION_TYPE is a 4-byte string in program memory.
 * Recommended use is major version, minor version, revision
 * and revision letter, e.g. 2.0.1a encoded as {0x02, 0x00, 0x01, 'a'}
 */
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

// determine environment
#ifdef ARDUINO
    #include <Arduino.h>
    #include <avr/pgmspace.h>
#elif __linux__
    #error "mh_serialHandlerClass linux implementation incomplete"
#elif _WIN32
    #error "mh_serialHandlerClass windows implementation incomplete"
#else
    #error "mh_serialHandlerClass unknown compilation environment"
#endif // determine environment

// uncomment this line to include test methods
#define MH_TEST_VNT

namespace mh_temperatureSensing
{

/*! \bug MH_VERSION type may be allocating a PROGMEM pointer to SRAM or
 *  failing to allocate either the pointer or the array to PROGMEM.
 *  The issue may be __progmem__ attribute support by GCC.
 *
 */
// sensor version
const size_t MH_VERSION_TYPE_SIZE = 4;
typedef char PROGMEM MH_VERSION_TYPE[MH_VERSION_TYPE_SIZE];
// recommended use is major version, minor version, revision
// and revision letter, e.g. 2.0.1a encoded as {0x02, 0x00, 0x01, 'a'}


/*! decodeVersionNumber assigns the bytes of version number to three integers and a char
 */
void decodeVersionNumber(const MH_VERSION_TYPE version,
                         int &majorVersion,
                         int &minorVersion,
                         int &revision,
                         char &build);

/*! encodeVersionNumber copies the contents of the version array to an array at bufferPtr
 */
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


/*! outputVersionNumber prints version to serialDevice as three integers and a character (i.e. 0.0.0a)
 */
int outputVersionNumber(Stream &serialDevice,
                        const MH_VERSION_TYPE version);
// returns bytes printed


// test methods
#ifdef MH_TEST_VNT

int testVersionNumber(Stream &serialDevice);

#endif // MH_TEST_VNT

} // close namespace mh_temperatureSensing

#endif // VERSIONNUMBERTYPE_H_INCLUDED

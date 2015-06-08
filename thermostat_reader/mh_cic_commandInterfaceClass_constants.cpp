#include <mh_cic_commandInterfaceClass.h>

/*! \brief Constants for configuration of commandInterfaceClass
 *
 */

/// versionNumber - a constant for identification and communication of version number
const mh_temperatureSensing::MH_VERSION_TYPE
mh_temperatureSensing::commandInterfaceClass::
versionNumber = { 0x00, 0x00, 0x01, 0x20 };

/// defaultBufferSize - the default size for the input/output buffer
const int
mh_temperatureSensing::commandInterfaceClass::
defaultBufferSize = 64;

/// headerSize - size of a command header in bytes
const size_t
mh_temperatureSensing::commandInterfaceClass::
headerSize = sizeof(CIC_SYNC_TYPE) +
             sizeof(CIC_ADDRESS_TYPE) +
             sizeof(CIC_ADDRESS_TYPE) +
             sizeof(CIC_MSG_NUM_TYPE) +
             sizeof(CIC_MESSAGE_TYPE) +
             sizeof(CIC_SIZE_TYPE);

/// framingSize - total command framing bytes
const size_t
mh_temperatureSensing::commandInterfaceClass::
framingSize = headerSize + sizeof(CIC_CRC_TYPE);

/// maxPayloadSize - maximum payload size that can be represented by CIC_SIZE_TYPE (integer max - framing size)
const size_t
mh_temperatureSensing::commandInterfaceClass::
maxPayloadSize = cicSizeTypeMax - framingSize;

/// statusMessageLength - fixed size of a status message
const mh_temperatureSensing::CIC_SIZE_TYPE
mh_temperatureSensing::commandInterfaceClass::
statusMessageLength = sizeof(CIC_SIZE_TYPE) +
                      sizeof(CIC_STATE_TYPE) +
                      sizeof(CIC_ERROR_TYPE) +
                      sizeof(COM_ERROR_TYPE);

/// typeMessagePayloadSize - fixed size of a type message
const mh_temperatureSensing::CIC_SIZE_TYPE
mh_temperatureSensing::commandInterfaceClass::
typeMessagePayloadSize = MH_VERSION_TYPE_SIZE +
                         sizeof(CIC_SENSOR_TYPE) +
                         MH_VERSION_TYPE_SIZE +
                         sizeof(CIC_SIZE_TYPE) +
                         sizeof(CIC_FLAGS_TYPE);

// SERIAL_BUFFER_SIZE is defined in HaredwareSerial.cpp
// if RAMEND < 1000 then SERIAL_BUFFER_SIZE = 16
// else SERIAL_BUFFER_SIZE = 64
// maximum bytes in buffer is (SERIAL_BUFFER_SIZE - 1)
#if (RAMEND < 1000)
#define SERIAL_MAX_BYTES_WAITING 15
#else
#define SERIAL_MAX_BYTES_WAITING 63
#endif
/// hardwareSerialMaxBytes - a constant used to anticipate hardware serial overflow
const int
mh_temperatureSensing::commandInterfaceClass::
hardwareSerialMaxBytes = SERIAL_MAX_BYTES_WAITING;

/// lineClearByteIntervals - number of byte periods to wait prior to transmission
const int
mh_temperatureSensing::commandInterfaceClass::
lineClearByteIntervals = 3;

/// readTimeoutByteIntervals - number of byte intervals to wait before assuming communications interruption
const int
mh_temperatureSensing::commandInterfaceClass::
readTimeoutByteIntervals = 4;



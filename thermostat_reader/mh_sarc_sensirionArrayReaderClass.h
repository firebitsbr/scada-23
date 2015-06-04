#ifndef SENSIRIONARRAYREADERCLASS_H_INCLUDED
#define SENSIRIONARRAYREADERCLASS_H_INCLUDED

/*! \brief This file contains the definitions for an object to read an array of Sensirion sensors.
 *
 * WARNINGS:
 * - Protection from null pointers to sensors is provided using the bit array m_sensorActiveMask.
 *   A bit is set in m_sensorActiveMask, a Sensirion object has been allocated at that index.
 *   Functions are not otherwise written to test for null pointers.  If the bit is not set,
 *   calls made to that index will cause null pointer errors and subsequent segmentation fault.
 *   Similarly for arrays, if any array is not properly allocated the sensor mask is set to 0.  Function
 *   calls accessing elements for which the corresponding mask bit is not set will cause segmentation fault.
 * - The encode functions can be used to overwrite memory at any location.  If the executing computer architecture
 *   does not enforce separation of instructions and data, malicious or erroneous calls to any encoding function
 *   can cause execution of malicious code.
 *
 * Usage notes:
 * - Only one type of measurement can be accomplished at a time.  Intended operation
 *   waits for completion of current measurement prior to start of next measurement.
 *   Specific limitations are (1) the sensor can take only one measurement at a time
 *   and (2) the same data set, m_rawDataArray, is used by both temperature and humidity
 *   functions.
 * - Humidity measurement must be preceded by a recent temperature measurement.  Current
 *   (or recent) temperature is used to calculate humidity.
 * - Duty cycle should not exceed 10% to prevent heating of the sensor.
 * - Use data encoding functions with caution.  Data array size is not verified by the encoding method.  See also
 *   the malicious code warning above.
 * - Data encoding and decoding functions do not address byte order issues.
 * - Array size is not retained or checked.  Arrays are allocated in initializeLocalArrays() based on
 *   the parameter numSensors.  During base class allocation a bit is set in m_sensorActiveMask for each sensor
 *   allocated, also based on the parameter numSensors.
 * - Commands are enumerated in sensirionArrayCommandsEnum
 */

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_sensirionArrayReaderClass linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_sensirionArrayReaderClass windows implementation incomplete"
#else
#error "mh_sarc_sensirionArrayReaderClass unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <stddef.h>
#include <stdint.h>

#include <tstatreader_pin_definitions.h>
#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>

// Format for encoded data:
//      SAC_SIZE_TYPE messageLength
//      SAC_MEASTYPE_TYPE measurementType
//      SAC_ERROR_TYPE error flags
//      SAC_FLAGS_TYPE values requested
//      SAC_FLAGS_TYPE values in error
//      SAC_CONVERTED_TYPE[valueCount] dataArray
// - the number of data elements is determined by counting the number of bits in valuesToPrint

// Format for commands:
//      SAC_COMMAND_TYPE command
//      SAC_SIZE_TYPE commandLength
//      byte[commandLength] commandData

// TODO: consider conversion of retained data vectors to raw data arrays.  Requires addition of vairable to
//       store resolution of the data when sampled and functions to calculate temperature and
//       humidity with resolution as a calling parameter
// TODO: consider #define / #ifdef to include or not include calling parameter and pointer tests.
//       This could be used to reduce code size on embedded systems with sufficient testing during
//       development to ensure calling parameters and pointers will be valid.

// set default serial device for debug output
#ifdef ARDUINO
extern Stream& sarcDebugStream;
#elif __linux__
#error "linux default output not defined"
#elif _WIN32
#error "windows default output not defined"
#else
#error "unknown environment, default output not defined"
#endif // determine environment

// debug code
// uncomment below to include print statements for debugging on embedded devices
// note - must also uncomment implementation of sarcDebugStream in cpp file
// #define MH_SARC_DEBUG_SENSOR_SETTINGS
// #define MH_SARC_DEBUG_MEASUREMENT
// #define MH_SARC_DEBUG_TEMP_MEASUREMENT
// #define MH_SARC_DEBUG_HUMID_MEASUREMENT
// #define MH_SARC_OUTPUT_DATA_WHEN_READ
// #define MH_SARC_DEBUG_READ_IMMEDIATE
// #define MH_SARC_DEBUG_SARC_PRINT
// #define MH_SARC_DEBUG_OUTPUTDATAARRAY
// #define MH_SARC_DEBUG_ENCODEDATAARRAY
// #define MH_SARC_DEBUG_DECODEDATAARRAY


namespace mh_temperatureSensing
{

class sensirionArrayReaderClass:
    public sensirionArrayClass
{
public:
    sensirionArrayReaderClass();
    sensirionArrayReaderClass(const int numSensors,
                              const int *dataPinArrayPtr,
                              const int *clockPinArrayPtr);
    ~sensirionArrayReaderClass();
    // methods to set resolution
    SAC_ERROR_TYPE setLowRes(void);
    SAC_ERROR_TYPE setHighRes(void);
    unsigned long getTempTimeoutMillis(void);
    unsigned long getHumidTimeoutMillis(void);
    // methods to initialize non-blocking measurement
    SAC_ERROR_TYPE beginTempMeasurement(SAC_FLAGS_TYPE sensorNumberFlags);
    SAC_ERROR_TYPE beginHumidMeasurement(SAC_FLAGS_TYPE sensorNumberFlags);
    SAC_ERROR_TYPE checkSensorsTemp(SAC_FLAGS_TYPE& dataReadyFlags);
    SAC_ERROR_TYPE checkSensorsHumid(SAC_FLAGS_TYPE& dataReadyFlags);
    // sequential blocking read of all sensors
    // all temperature then all humidity
    SAC_ERROR_TYPE readTemperatureImmediate(const SAC_FLAGS_TYPE valuesRequested,
                                            SAC_FLAGS_TYPE &valuesMeasured);
    SAC_ERROR_TYPE readHumidityImmediate(const SAC_FLAGS_TYPE valuesRequested,
                                         SAC_FLAGS_TYPE &valuesMeasured);
    SAC_ERROR_TYPE readAllImmediate(void);
    // print last temperatures
    SAC_ERROR_TYPE printTemperatures(Stream &serialDevice,
                                     SAC_FLAGS_TYPE valuesRequested);
    // print last humidities
    SAC_ERROR_TYPE printHumidities(Stream &serialDevice,
                                   SAC_FLAGS_TYPE valuesRequested);
    // print state and errors
    SAC_ERROR_TYPE printStatus(Stream &serialDevice);
    // encode temperatures
    SAC_ERROR_TYPE encodeTemperatures(char *buffer,
                                      const size_t buffSize,
                                      size_t &bytesEncoded,
                                      SAC_FLAGS_TYPE valuesRequested);
    // encode humidities
    SAC_ERROR_TYPE encodeHumidities(char *buffer,
                                    const size_t buffSize,
                                    size_t &bytesEncoded,
                                    SAC_FLAGS_TYPE valuesRequested);
    // encode state and errors errors
    SAC_ERROR_TYPE encodeStatus(char *buffer,
                                const size_t buffSize,
                                size_t &bytesEncoded);
    // decode status and errors
    static SAC_ERROR_TYPE decodeStatus(char *bufferPtr,
                                       const size_t bufferSize,
                                       size_t &bytesUsed,
                                       SAC_SIZE_TYPE &statedBytes,
                                       SAC_STATE_TYPE &sensorState,
                                       SAC_FLAGS_TYPE &activeMask,
                                       SAC_ERROR_TYPE &sacErrors,
                                       SAC_FLAGS_TYPE &sensorErrors);
    // clearErrors() returns previous base class error state
    SAC_ERROR_TYPE clearErrors(void);
    void initializeNoAlloc(void);
    SAC_ERROR_TYPE initialize(const int numSensors,
                              const int *dataPinArrayPtr,
                              const int *clockPinArrayPtr);
    void initializeLocalNoAlloc(void);
    SAC_ERROR_TYPE initializeLocalArrays(const int numSensors);
protected:
    void freeMemory(void);
    void freeLocalMemory(void);
public:
    SAC_ERROR_TYPE setResolution(const bool setLowResolution = false);
    SAC_ERROR_TYPE beginMeasurement(const SAC_FLAGS_TYPE sensorNumberFlags,
                                    SAC_FLAGS_TYPE& inProcessFlags,
                                    SAC_FLAGS_TYPE& beginErrorFlags,
                                    const uint8_t measurementType);
    SAC_ERROR_TYPE checkSensorsAndRead(SAC_FLAGS_TYPE& activeSensorSet,
                                       SAC_FLAGS_TYPE& dataReadyFlags,
                                       SAC_FLAGS_TYPE& errorFlags);
protected:
    // flags to control non-blocking measurements
    // flags to indicate sensing in process
    SAC_FLAGS_TYPE m_tempInProcessFlags = 0;
    SAC_FLAGS_TYPE m_humidInProcessFlags = 0;
    // flags to indicate error on last operation
    SAC_FLAGS_TYPE m_resolutionErrorFlags = 0;
    SAC_FLAGS_TYPE m_tempErrorFlags = 0;
    SAC_FLAGS_TYPE m_humidErrorFlags = 0;
    // variables to control measurement timeout
    unsigned long m_tempMeasStartTime = 0;
    unsigned long m_humidMeasStartTime = 0;
    // temperature and humidity timeout values
    // sensor times are:
    //   low resolution temperature    80 msec
    //   high resolution temperature  320 msec
    //   low resolution humidity       20 msec
    //   high resolution humidity      80 msec
    // timeouts add margin of 50%
    // duty cycle should not exceed 10%
    static const unsigned long lowResTempTimeout = 120ul;
    static const unsigned long highResTempTimeout = 480ul;
    static const unsigned long lowResHumidTimeout = 30ul;
    static const unsigned long highResHumidTimeout = 120ul;
    unsigned long m_tempTimeoutMillis = highResTempTimeout;
    unsigned long m_humidTimeoutMillis = highResHumidTimeout;
    // arrays to store measurement data and times
    // types are per Sensirion.h in Arduino Sensirion library
    SAC_SIZE_TYPE m_readerArraySize = 0;
    SAC_RAW_TYPE *m_rawDataArray = NULL;
    SAC_CONVERTED_TYPE *m_lastTemperatureArray = NULL;
    SAC_CONVERTED_TYPE *m_lastHumidityArray = NULL;
public:
    // length of status message
    static const SAC_SIZE_TYPE statusMessageLength;
    // test methods
    // test setting resolution
    friend int exerciseSensorSettings(Stream &serialDevice,
                                      sensirionArrayReaderClass *readerToTest);
    friend int exerciseSensirionArrayReaderClass(Stream &serialDevice,
            sensirionArrayReaderClass *readerToTest);
}; // end class sensirionArrayReaderClass


// test methods
// test setting resolution
int exerciseSensorSettings(Stream &serialDevice,
                           sensirionArrayReaderClass *readerToTest);

// exercise sensirionArrayReaderClass
int exerciseSensirionArrayReaderClass(Stream &serialDevice,
                                      sensirionArrayReaderClass *sensirionReader);

} // close namespace mh_temperatureSensing

#endif // SENSIRIONARRAYREADERCLASS_H_INCLUDED

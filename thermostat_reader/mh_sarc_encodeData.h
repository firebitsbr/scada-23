#ifndef MH_SARC_ENCODEDATA_H_INCLUDED
#define MH_SARC_ENCODEDATA_H_INCLUDED

/*! \brief Encode and decode methods are provided to support communication across devices.
 *
 *  WARNINGS:
 *  - The encode functions can be used to overwrite memory at any location.  If the executing computer architecture
 *    does not enforce separation of instructions and data, malicious or erroneous calls to any encoding function
 *    can cause execution of malicious code.
 *  - Array bounds are not checked.  Array elements are accessed corresponding to all bits set in valuesToPrint.
 */

// TODO: consider modification of all methods to templates for greater reuse.  Templated class types would be flagsType,
//       errorsType and convertedType

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_encodeData linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_encodeData windows implementation incomplete"
#else
#error "mh_sarc_encodeData unknown compilation environment"
#endif // determine environment

#include <stddef.h>
#include <stdint.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sarc_sensirionArrayReaderClass.h>


namespace mh_temperatureSensing
{

// methods for compution of message size
// bit count
template <class T_FlagsType>
int flagCount(const T_FlagsType flags);

// base size (fixed size data, no array)
template <class T_SizeType, class T_MeasurementType, class T_ErrorType, class T_FlagsType>
size_t computeBaseSize(void);
// recommend wrapping to size_t abc_computeBaseSize(void) where abc identifies the particular use

// complete message size
template <class T_SizeType, class T_MeasurementType, class T_ErrorType, class T_FlagsType, class T_DataType>
size_t computeMessageSize(T_FlagsType const flagVar);
// recommend wrapping to size_t abc_computeMessageSize(const SAC_FLAGS_TYPE flags) where abc identifies the particular use

SAC_ERROR_TYPE clearArrayElements(const SAC_FLAGS_TYPE valuesToClear,
                                  const int arraySize,
                                  SAC_CONVERTED_TYPE *dataArray);

// encode array to buffer
SAC_ERROR_TYPE encodeDataArray(char *bufferPtr,
                               const size_t bufferSize,
                               size_t &bytesEncoded,
                               const measurementTypeEnum measurementType,
                               const SAC_ERROR_TYPE errorFlags,
                               const SAC_FLAGS_TYPE valuesToPrint,
                               const SAC_FLAGS_TYPE valuesInError,
                               const size_t arraySize,
                               const SAC_CONVERTED_TYPE *dataArray);

// encode command to buffer
SAC_ERROR_TYPE encodeCommand(char *bufferPtr,
                             const size_t bufferSize,
                             size_t &bytesEncoded,
                             const sensirionArrayCommandsEnum commandValue,
                             const SAC_FLAGS_TYPE commandFlags);

// output array to serial device
SAC_ERROR_TYPE outputDataArray(Stream &outputDevice,
                               size_t &bytesWritten,
                               const measurementTypeEnum measurementType,
                               const SAC_ERROR_TYPE errorFlags,
                               const SAC_FLAGS_TYPE valuesToPrint,
                               const SAC_FLAGS_TYPE valuesInError,
                               const int arraySize,
                               const SAC_CONVERTED_TYPE *dataArray);

// decode buffer
SAC_ERROR_TYPE decodeDataArray(char *bufferPtr,
                               const size_t bufferSize,
                               size_t &bytesDecoded,
                               SAC_SIZE_TYPE &reportedSize,
                               measurementTypeEnum &reportedType,
                               SAC_ERROR_TYPE &reportedErrors,
                               SAC_FLAGS_TYPE &valuesStated,
                               SAC_FLAGS_TYPE &valuesInError,
                               SAC_FLAGS_TYPE &valuesReceived,
                               const int arraySize,
                               SAC_CONVERTED_TYPE *dataArray);

// decode command
SAC_ERROR_TYPE decodeCommand(char *bufferPtr,
                             const size_t bufferSize,
                             size_t &bytesDecoded,
                             sensirionArrayCommandsEnum &commandValue,
                             SAC_FLAGS_TYPE &commandFlags);

// output command
SAC_ERROR_TYPE outputCommand(Stream &serialDevice,
                             size_t &bytesWritten,
                             const sensirionArrayCommandsEnum commandValue,
                             const SAC_FLAGS_TYPE commandFlags);

} // close namespace mh_temperatureSensing

#endif // MH_SARC_ENCODEDATA_H_INCLUDED

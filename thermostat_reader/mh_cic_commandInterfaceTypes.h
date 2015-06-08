#ifndef COMMANDINTERFACETYPES_H_INCLUDED
#define COMMANDINTERFACETYPES_H_INCLUDED

#include <stdint.h>
#include <stddef.h>

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_cic_commandInterfaceTypes linux implementation incomplete"
#elif _WIN32
#error "mh_cic_commandInterfaceTypes windows implementation incomplete"
#else
#error "mh_cic_commandInterfaceTypes unknown compilation environment"
#endif // determine environment


/*! \brief mh_cic_commandInterfaceTypes is a collection of types and definitions
 *  to facilitate development, maintenance and cross-platform use of a basic command interface.
 *
 *  Print utilities are provided for most enumerated types.
 *
 *  Usage notes:
 *  - CIC_FLAGS_TYPE must be at least as large as the largest array
 *    of controlled sensors.  Recommend coordinating this type across
 *    all systems on the same data link.
 */


namespace mh_temperatureSensing
{

// synchronizatoin format
typedef uint8_t CIC_SYNC_TYPE;
const CIC_SYNC_TYPE syncValue = 170; // binary 10101010

// device addresses
// 0 is not recommended
// 255 is broadcast
typedef uint8_t CIC_ADDRESS_TYPE;
const CIC_ADDRESS_TYPE CIC_NULL_ADDRESS = 0;
const CIC_ADDRESS_TYPE CIC_BROADCAST_ADDRESS = ~( (CIC_ADDRESS_TYPE) 0);

typedef uint8_t CIC_FLAGS_TYPE;
// this definition determines the maximum number of sensors
// that can be controlled (one bit per sensor)

// message number
typedef uint8_t CIC_MSG_NUM_TYPE;

// message type
typedef uint8_t CIC_MESSAGE_TYPE;
enum cicMessageTypeEnum { CIC_MSG_NONE = 0,
                          CIC_MSG_ACK = 1,
                          CIC_MSG_NAK = 2,
                          CIC_MSG_TYPE = 3,
                          CIC_MSG_STATUS = 4,
                          CIC_MSG_CAPABILITIES = 5,
                          CIC_MSG_COMMAND = 6,
                          CIC_MSG_DATA = 7
                        };

// command types
typedef uint8_t CIC_COMMAND_TYPE;
enum cicCommandTypeEnum { CCT_NO_COMMAND = 0,
                          CCT_REPORT_TYPE = 1,
                          CCT_REPORT_STATUS = 2,
                          CCT_REPORT_SENSOR_CAPABILITIES = 3,
                          CCT_SET_MEAS_TYPE = 4,
                          CCT_SET_MEAS_RESOLUTION = 5,
                          CCT_START_MEASUREMENT = 6,
                          CCT_REPORT_VALUES = 7,
                          CCT_CLEAR_ERRORS = 8
                        };

// sensor types
typedef uint8_t CIC_SENSOR_TYPE;
enum cicSensorTypeEnum { CST_NONE = 0,
                         CST_TEMP_ANY = 1,
                         CST_TEMP_SENSIRION = 2,
                         CST_TEMP_TC_B = 3,
                         CST_TEMP_TC_E = 4,
                         CST_TEMP_TC_J = 5,
                         CST_TEMP_TC_K = 6,
                         CST_TEMP_TC_N = 7,
                         CST_TEMP_TC_R = 8,
                         CST_TEMP_TC_S = 9,
                         CST_TEMP_TC_T = 10,
                         CST_TEMP_TC_OTHER = 11,
                         CST_TEMP_RTD_PT10 = 12,
                         CST_TEMP_RTD_PT50 = 13,
                         CST_TEMP_RTD_PT100 = 14,
                         CST_TEMP_RTD_PT200 = 15,
                         CST_TEMP_RTD_PT500 = 16,
                         CST_TEMP_RTD_PT1000 = 17,
                         CST_TEMP_RTD_1000 = 18,
                         CST_TEMP_RTD_NI120 = 19,
                         CST_TEMP_RTD_OTHER = 20,
                         CST_THERMISTOR_44004_44033 = 21,
                         CST_THERMISTOR_44005_44030 = 22,
                         CST_THERMISTOR_44007_44034 = 23,
                         CST_THERMISTOR_44006_44031 = 24,
                         CST_THERMISTOR_44008_44032 = 25,
                         CST_THERMISTOR_YSI400 = 26,
                         CST_THERMISTOR_SPECTRUM_1003K_1K = 27,
                         CST_THERMISTOR_CUSTOM_SH = 28,
                         CST_THERMISTOR_OTHER = 29,
                         CST_TEMP_DIODE = 30,
                         CST_TEMP_SENS_RESISTOR = 31,
                         CST_TEMP_OTHER = 32,
                         CST_DIRECT_ADC = 33,
                         CST_HUMID_ANY = 34,
                         CST_HUMID_SENSIRION = 35,
                         CST_HUMID_OTHER = 36,
                         CST_PRESSURE_ANY = 37,
                         CST_PRESSURE_THRESHOLD = 38,
                         CST_PRESSURE_OTHER = 39,
                         CST_CURRENT_ANY = 40,
                         CST_CURRENT_AC = 41,
                         CST_CURRENT_DC = 42,
                         CST_CURRENT_OTHER = 43
                       };

// this type represents class-level errors
// one bit to represent each recorded error state
typedef uint32_t CIC_ERROR_TYPE;
// enumerated type to represent errors
enum cicErrorTypeEnum { CIC_SUCCESS = 0,
                        CIC_ERR_UNINIT = 1,
                        CIC_ERR_BAD_COUNT = 2,
                        CIC_ERR_BAD_ARRAY_PTR = 4,
                        CIC_COMM_ERROR = 8,
                        CIC_COMMAND_ERROR = 16,
                        CIC_ERR_TIMEOUT = 32,
                        CIC_ERR_INSUFF_MEMORY = 64,
                        CIC_ERR_STATE_ERROR = 128,
                        CIC_OTHER_ERROR = 256,
                        CIC_ERR_INCORRECT_TYPE = 512,
                        CIC_ERROR_UNKNOWN = 1024
                      };

// this type represents communications errors
typedef uint8_t COM_ERROR_TYPE;
// enumerated type to represent errors
enum comErrorTypeEnum { COMM_SUCCESS = 0,
                        COM_UNKNOWN_TYPE = 1,
                        COM_BAD_COUNT = 2,
                        COM_INSUFF_MEMORY = 4,
                        COM_BAD_CRC = 8,
                        COM_TIMEOUT = 16,
                        COM_UNABLE = 32,
                        COM_OTHER_ERROR = 64,
                        COM_ERROR_UNKNOWN = 128
                      };

// payload size
typedef uint8_t CIC_SIZE_TYPE;
const CIC_SIZE_TYPE cicSizeTypeMax = 255;

// data type identifier
typedef uint8_t CIC_DATAREP_TYPE;
enum dataTypeRepEnum { CDT_NONE = 0,
                       CDT_INT8 = 1,
                       CDT_INT16 = 2,
                       CDT_INT32 = 3,
                       CDT_INT64 = 4,
                       CDT_UINT8 = 5,
                       CDT_UINT16 = 6,
                       CDT_UINT32 = 7,
                       CDT_UINT64 = 8,
                       CDT_FLOAT4 = 9,
                       CDT_FLOAT8 = 10,
                       CDT_OTHER = 11
                     };

// CRC
typedef uint8_t CIC_CRC_TYPE;
const CIC_CRC_TYPE crcInitialValue = 0;
const CIC_CRC_TYPE crcPolynomial = 0xD5;

// source of buffer pointer
// used to determine if deallocation is required when
// performing buffer operations (allocate, resize, free memory)
enum bufferSourceEnum { NO_BUFFER = 0,
                        ALLOCATED = 1,
                        BY_REFERENCE = 2
                      };

// state machine class
typedef uint8_t CIC_STATE_TYPE;
enum cicStateEnum { CIC_UNINIT = 0,
                    CIC_INITIALIZED = 1,
                    CIC_START = 2,
                    CIC_INIT_TEMP = 3,
                    CIC_READING_TEMP = 4,
                    CIC_TEMP_REPORTERROR = 5,
                    CIC_TEMP_COMPLETE = 6,
                    CIC_INIT_HUMID = 7,
                    CIC_READING_HUMID = 8,
                    CIC_HUMID_REPORTERROR = 9,
                    CIC_HUMID_COMPLETE = 10,
                    CIC_WAIT_NEXT_ACTION = 11,
                    CIC_ALL_COMPLETE = 12,
                    CIC_SHUTDOWN = 13,
                    CIC_STATE_ERROR = 14,
                    CIC_STOP = 15,
                    CIC_SAFETY_STOP = 16
                  };


int printCicMessageTypeText(Stream &serialDevice, const CIC_MESSAGE_TYPE messageType);
int printCicMessageTypeText(Stream &serialDevice, const cicMessageTypeEnum messageType);
int printCicCommandTypeText(Stream &serialDevice, const CIC_COMMAND_TYPE messageType);
int printCicCommandTypeText(Stream &serialDevice, const cicCommandTypeEnum messageType);
int printCicErrorTypeText(Stream &serialDevice, const CIC_ERROR_TYPE errorValue);
int printCicErrorTypeText(Stream &serialDevice, const cicErrorTypeEnum errorValue);
int printCicSensorTypeText(Stream &serialDevice, const CIC_SENSOR_TYPE sensorType);
int printCicSensorTypeText(Stream &serialDevice, const cicSensorTypeEnum sensorType);
int printComErrorTypeText(Stream &serialDevice, const COM_ERROR_TYPE errorValue);
int printComErrorTypeText(Stream &serialDevice, const comErrorTypeEnum errorValue);
int printBufferSourceText(Stream &serialDevice, const bufferSourceEnum bufferSource);
int printCicStateTypeText(Stream &serialDevice, const cicStateEnum stateValue);

} // close namespace mh_temperatureSensing

#endif // COMMANDINTERFACETYPES_H_INCLUDED


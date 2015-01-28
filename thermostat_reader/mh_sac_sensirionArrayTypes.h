#ifndef MH_SAC_SENSIRIONARRAYTYPES_H_INCLUDED
#define MH_SAC_SENSIRIONARRAYTYPES_H_INCLUDED

namespace mh_temperatureSensing
{

// enumerated type to track sensor state
// also contains values for use in an sensor array reading

// sensor state
// intended for use in state machine implementations
typedef uint8_t SAC_STATE_TYPE;
enum sensirionSensorStateEnum { SSS_UNINIT = 0,
                                SSS_INITIALIZED = 1,
                                SSS_START = 2,
                                SSS_INIT_TEMP = 3,
                                SSS_READING_TEMP = 4,
                                SSS_TEMP_REPORTERROR = 5,
                                SSS_TEMP_COMPLETE = 6,
                                SSS_INIT_HUMID = 7,
                                SSS_READING_HUMID = 8,
                                SSS_HUMID_REPORTERROR = 9,
                                SSS_HUMID_COMPLETE = 10,
                                SSS_WAIT_NEXT_ACTION = 11,
                                SSS_ALL_COMPLETE = 12,
                                SSS_SHUTDOWN = 13,
                                SSS_STATE_ERROR = 14,
                                SSS_STOP = 15,
                                SSS_SAFETY_STOP = 16
                              };

typedef uint8_t SAC_FLAGS_TYPE;
// this definition determines the maximum number of sensors
// that can be controlled (one bit per sensor)

// enumerated type to represent measurement types
typedef uint8_t SAC_MEASTYPE_TYPE;
enum measurementTypeEnum { MH_NO_MEASUREMENT = 0,
                           MH_TEMP_SENSIRION = 1,
                           MH_HUMI_SENSIRION = 2,
                           MH_TEMP_THERMO_J = 3,
                           MH_TEMP_THERMO_K = 4
                         };

typedef uint32_t SAC_ERROR_TYPE;
// this type represents class-level errors
// one bit to represent each recorded error state
// enumerated type to track errors
enum sensirionArrayErrorsEnum { SAC_SUCCESS = 0,
                                SAC_UNINIT = 1,
                                SAC_BAD_COUNT = 2,
                                SAC_BAD_ARRAY_PTR = 4,
                                SAC_BAD_SENSOR_PTR = 8,
                                SAC_COMM_ERROR = 16,
                                SAC_SENSOR_UNAVAIL = 32,
                                SAC_RESOLUTION_ERROR = 64,
                                SAC_COMMAND_ERROR = 128,
                                SAC_MEASURE_ERROR = 256,
                                SAC_TIMEOUT = 512,
                                SAC_INSUFF_MEMORY = 1024,
                                SAC_STATE_ERROR = 2048,
                                SAC_OTHER_ERROR = 4096,
                                SAC_ERROR_UNKNOWN = 8192
                              };

typedef uint8_t SAC_COMMAND_TYPE;
// enumerate type for commands
enum sensirionArrayCommandsEnum { SAC_NO_ACTION = 0,
                                  SAC_REPORT_STATUS = 1,
                                  SAC_CLEAR_ERRORS = 2,
                                  SAC_SET_HIGHRES = 3,
                                  SAC_SET_LOWRES = 4,
                                  SAC_MEASURE_TEMP = 5,
                                  SAC_MEASURE_HUMID = 6,
                                  SAC_REPORT_TEMP = 7,
                                  SAC_REPORT_HUMID = 8
                                };

// print text of sensor state
SAC_ERROR_TYPE printSensirionSensorStateEnum(Stream &serialDevice,
                                              const sensirionSensorStateEnum stateToPrint);

// print text of flags set
SAC_ERROR_TYPE printSensirionFlagsType(Stream &serialDevice,
                                        const SAC_FLAGS_TYPE flagsToPrint);

// print text of measurement type
SAC_ERROR_TYPE printMeasurementTypeEnum(Stream &serialDevice,
                                         const measurementTypeEnum typeToPrint);

// print text of error bits set
SAC_ERROR_TYPE printSacErrorsType(Stream &serialDevice,
                                   const SAC_ERROR_TYPE errorsToPrint);

SAC_ERROR_TYPE printArrayCommandsEnum(Stream &serialDevice,
                                       size_t &bytesWritten,
                                       const sensirionArrayCommandsEnum commandToPrint);

} // close namespace mh_temperatureSensing

#endif // MH_SAC_SENSIRIONARRAYTYPES_H_INCLUDED

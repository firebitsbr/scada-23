
#include <Arduino.h>
#include <Sensirion.h>

#include "mh_sac_sensirionArrayTypes.h"


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
printSensirionSensorStateEnum(Stream &serialDevice,
                              const sensirionSensorStateEnum stateToPrint)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    switch (stateToPrint)
    {
        case SSS_UNINIT:
            serialDevice.print(F("SSS_UNINIT"));
            break;
        case SSS_INITIALIZED:
            serialDevice.print(F("SSS_INITIALIZED"));
            break;
        case SSS_START:
            serialDevice.print(F("SSS_START"));
            break;
        case SSS_INIT_TEMP:
            serialDevice.print(F("SSS_INIT_TEMP"));
            break;
        case SSS_READING_TEMP:
            serialDevice.print(F("SSS_READING_TEMP"));
            break;
        case SSS_TEMP_REPORTERROR:
            serialDevice.print(F("SSS_TEMP_REPORTERROR"));
            break;
        case SSS_TEMP_COMPLETE:
            serialDevice.print(F("SSS_TEMP_COMPLETE"));
            break;
        case SSS_INIT_HUMID:
            serialDevice.print(F("SSS_INIT_HUMID"));
            break;
        case SSS_READING_HUMID:
            serialDevice.print(F("SSS_READING_HUMID"));
            break;
        case SSS_HUMID_REPORTERROR:
            serialDevice.print(F("SSS_HUMID_REPORTERROR"));
            break;
        case SSS_HUMID_COMPLETE:
            serialDevice.print(F("SSS_HUMID_COMPLETE"));
            break;
        case SSS_WAIT_NEXT_ACTION:
            serialDevice.print(F("SSS_WAIT_NEXT_ACTION"));
            break;
        case SSS_ALL_COMPLETE:
            serialDevice.print(F("SSS_ALL_COMPLETE"));
            break;
        case SSS_SHUTDOWN:
            serialDevice.print(F("SSS_SHUTDOWN"));
            break;
        case SSS_STATE_ERROR:
            serialDevice.print(F("SSS_STATE_ERROR"));
            break;
        case SSS_STOP:
            serialDevice.print(F("SSS_STOP"));
            break;
        case SSS_SAFETY_STOP:
            serialDevice.print(F("SSS_SAFETY_STOP"));
            break;
        default:
            serialDevice.print(F("UNKNOWN,"));
            serialDevice.print(stateToPrint);
            retVal |= SAC_STATE_ERROR;
            break;
    } // end switch (stateToPrint)
    return retVal;
} // end SAC_ERROR_TYPE printSensirionSensorStateEnum(Stream &serialDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
printSensirionFlagsType(Stream &serialDevice,
                        const SAC_FLAGS_TYPE flagsToPrint)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    boolean havePrinted = false;
    SAC_FLAGS_TYPE sensorSelectBit = 1;
    int bitCount = 8 * sizeof(SAC_FLAGS_TYPE);
    for (int i=0; i<bitCount; i++)
    {
        if (flagsToPrint & sensorSelectBit)
        {
            if (havePrinted)
            {
                serialDevice.print(F(", "));
            } // end if (havePrinted);
            serialDevice.print(i);
            havePrinted = true;
        } // end if (flagsToPrint & sensorSelectBit)
        sensorSelectBit <<= 1;
    } // end for (i)
    return retVal;
} // end SAC_ERROR_TYPE printSensirionFlagsType(Stream &serialDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
printMeasurementTypeEnum(Stream &serialDevice,
                         const measurementTypeEnum typeToPrint)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    switch (typeToPrint)
    {
        case MH_NO_MEASUREMENT:
            serialDevice.print(F("MH_NO_MEASUREMENT"));
            break;
        case MH_TEMP_SENSIRION:
            serialDevice.print(F("MH_TEMP_SENSIRION"));
            break;
        case MH_HUMI_SENSIRION:
            serialDevice.print(F("MH_HUMI_SENSIRION"));
            break;
        case MH_TEMP_THERMO_J:
            serialDevice.print(F("MH_TEMP_THERMO_J"));
            break;
        case MH_TEMP_THERMO_K:
            serialDevice.print(F("MH_TEMP_THERMO_K"));
            break;
        default:
            serialDevice.print(F("UNKNOWN"));
            serialDevice.print(typeToPrint);
            retVal |= SAC_ERROR_UNKNOWN;
        break;
    } // end switch (typeToPrint)
    return retVal;
} // end SAC_ERROR_TYPE printMeasurementTypeEnum(Stream &serialDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
printSacErrorsType(Stream &serialDevice,
                   const SAC_ERROR_TYPE errorsToPrint)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    if (errorsToPrint == SAC_SUCCESS)
    {
        serialDevice.print(F("SAC_SUCCESS"));
    }
    else
    {
        SAC_ERROR_TYPE workVar = errorsToPrint;
        if (workVar & SAC_UNINIT)
        {
            serialDevice.print(F("SAC_UNINIT "));
            workVar &= ~SAC_UNINIT;
        }
        if (workVar & SAC_BAD_COUNT)
        {
            serialDevice.print(F("SAC_BAD_COUNT "));
            workVar &= ~SAC_BAD_COUNT;
        }
        if (workVar & SAC_BAD_ARRAY_PTR)
        {
            serialDevice.print(F("SAC_BAD_ARRAY_PTR "));
            workVar &= ~SAC_BAD_ARRAY_PTR;
        }
        if (workVar & SAC_BAD_SENSOR_PTR)
        {
            serialDevice.print(F("SAC_BAD_SENSOR_PTR "));
            workVar &= ~SAC_BAD_SENSOR_PTR;
        }
        if (workVar & SAC_COMM_ERROR)
        {
            serialDevice.print(F("SAC_COMM_ERROR "));
            workVar &= ~SAC_COMM_ERROR;
        }
        if (workVar & SAC_SENSOR_UNAVAIL)
        {
            serialDevice.print(F("SAC_SENSOR_UNAVAIL "));
            workVar &= ~SAC_SENSOR_UNAVAIL;
        }
        if (workVar & SAC_RESOLUTION_ERROR)
        {
            serialDevice.print(F("SAC_RESOLUTION_ERROR "));
            workVar &= ~SAC_RESOLUTION_ERROR;
        }
        if (workVar & SAC_COMMAND_ERROR)
        {
            serialDevice.print(F("SAC_COMMAND_ERROR "));
            workVar &= ~SAC_COMMAND_ERROR;
        }
        if (workVar & SAC_MEASURE_ERROR)
        {
            serialDevice.print(F("SAC_MEASURE_ERROR "));
            workVar &= ~SAC_MEASURE_ERROR;
        }
        if (workVar & SAC_TIMEOUT)
        {
            serialDevice.print(F("SAC_TIMEOUT "));
            workVar &= ~SAC_TIMEOUT;
        }
        if (workVar & SAC_INSUFF_MEMORY)
        {
            serialDevice.print(F("SAC_INSUFF_MEMORY "));
            workVar &= ~SAC_INSUFF_MEMORY;
        }
        if (workVar & SAC_ERROR_UNKNOWN )
        {
            serialDevice.print(F("SAC_ERROR_UNKNOWN "));
            workVar &= ~SAC_ERROR_UNKNOWN ;
        }
        // check for any remaining bits set
        if (workVar != SAC_SUCCESS)
        {
            serialDevice.print("UNKNOWN BITSET ");
            serialDevice.print(workVar, HEX);
            retVal |= SAC_ERROR_UNKNOWN;
        } // end if (workVal != SAC_SUCCESS)
    } // end if (errorsToPrint == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE printSacErrorsType(Stream &serialDevice, ... )



mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
printArrayCommandsEnum(Stream &serialDevice,
                       size_t &bytesWritten,
                       const sensirionArrayCommandsEnum commandToPrint)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    bytesWritten = 0;
    switch (commandToPrint)
    {
        case SAC_NO_ACTION:
            bytesWritten += serialDevice.print(F("SAC_NO_ACTION"));
            break;
        case SAC_REPORT_STATUS:
            bytesWritten += serialDevice.print(F("SAC_REPORT_STATUS"));
            break;
        case SAC_CLEAR_ERRORS:
            bytesWritten += serialDevice.print(F("SAC_CLEAR_ERRORS"));
            break;
        case SAC_SET_HIGHRES:
            bytesWritten += serialDevice.print(F("SAC_SET_HIGHRES"));
            break;
        case SAC_SET_LOWRES:
            bytesWritten += serialDevice.print(F("SAC_SET_LOWRES"));
            break;
        case SAC_MEASURE_TEMP:
            bytesWritten += serialDevice.print(F("SAC_MEASURE_TEMP"));
            break;
        case SAC_MEASURE_HUMID:
            bytesWritten += serialDevice.print(F("SAC_MEASURE_HUMID"));
            break;
        case SAC_REPORT_TEMP:
            bytesWritten += serialDevice.print(F("SAC_REPORT_TEMP"));
            break;
        case SAC_REPORT_HUMID:
            bytesWritten += serialDevice.print(F("SAC_REPORT_HUMID"));
            break;
        default:
            bytesWritten += serialDevice.print(F("UNKNOWN"));
            serialDevice.print(commandToPrint);
            retVal |= SAC_ERROR_UNKNOWN;
        break;
    } // end switch (commandToPrint)
    return retVal;
} // end SAC_ERROR_TYPE printArrayCommandsEnum(Stream &serialDevice, ... )




// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_encodeCommand linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_encodeCommand windows implementation incomplete"
#else
#error "mh_sarc_encodeCommand unknown compilation environment"
#endif // determine environment

#include <string.h>
#include <stdint.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sarc_encodeData.h>
#include <mh_cic_commandInterfaceTypes.h>


// !!
// !! See warnings in mh_sarc_encodeData.h for malicious code and pointer issues
// !!


// encode command to buffer
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
encodeCommand(char *buffer,
              const size_t bufferSize,
              size_t &bytesEncoded,
              const sensirionArrayCommandsEnum commandValue,
              const SAC_FLAGS_TYPE commandFlags)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // initial values of arguments passed by reference
    bytesEncoded = 0;
    // check input to read
    if (buffer == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
    }
    SAC_COMMAND_TYPE commandTypeValue = (SAC_COMMAND_TYPE) commandValue;
    const size_t numBytes = (sizeof(commandValue) + sizeof(commandTypeValue));
    if (bufferSize < numBytes)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
    }
    // if valid input, encode data
    if (retVal == SAC_SUCCESS)
    {
        bytesEncoded = numBytes;
        char *workPtr = buffer;
        // command
        memcpy((void*) workPtr, (void*) &commandTypeValue, sizeof(commandTypeValue));
        workPtr += sizeof(commandTypeValue);
        // arguments
        memcpy((void*) workPtr, (void*) &commandFlags, sizeof(commandFlags));
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE encodeCommand(char *buffer, ... )


// encode command to buffer
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
decodeCommand(char *bufferPtr,
              const size_t bufferSize,
              size_t &bytesDecoded,
              sensirionArrayCommandsEnum &commandValue,
              SAC_FLAGS_TYPE &commandFlags)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // initial values of arguments passed by reference
    bytesDecoded = 0;
    // check input to read
    if (bufferPtr == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
    }
    SAC_COMMAND_TYPE commandTypeValue = 0x0;
    if (bufferSize < sizeof(commandTypeValue))
    {
        retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
    }
    if (retVal == SAC_SUCCESS)
    {
        // read command
        char *workPtr = bufferPtr;
        memcpy((void*) &commandTypeValue, (void*) workPtr, sizeof(commandTypeValue));
        commandValue = (sensirionArrayCommandsEnum) commandTypeValue;
        workPtr += sizeof(commandTypeValue);
        bytesDecoded += sizeof(commandValue);
        // read arguments
        if (bufferSize >= (sizeof(commandTypeValue) + sizeof(commandFlags)))
        {
            memcpy((void*) &commandFlags, (void*) workPtr, sizeof(commandFlags));
            bytesDecoded += sizeof(commandFlags);
        }
        else
        {
            // clear command flags to prevent use of old data
            commandFlags = (SAC_FLAGS_TYPE) 0;
        } // end if (bufferSize >= (sizeof(commandValue) + sizeof(commandFlags))
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE decodeCommand(char *buffer, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
outputCommand(Stream &serialDevice,
              size_t &bytesWritten,
              const sensirionArrayCommandsEnum commandValue,
              const SAC_FLAGS_TYPE commandFlags)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // set initial values for values returned by reference
    bytesWritten = 0;
    retVal |= printArrayCommandsEnum(serialDevice,
                                     bytesWritten,
                                     commandValue);
    // only print arguments if command printed
    if (bytesWritten > 0)
    {
        bytesWritten += serialDevice.print(F("("));
        bytesWritten += serialDevice.print(commandFlags, BIN);
        bytesWritten += serialDevice.print(F(")"));
    } // end if (bytesWritten > 0)
    return retVal;
} // end outputCommand(Stream &outputDevice, ... )


// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_encodeData_arrays linux implementation incomplete"
#elif _WIN32
#error "mh_encodeData_arrays windows implementation incomplete"
#else
#error "mh_encodeData_arrays unknown compilation environment"
#endif // determine environment

#include <string.h>
#include <stdint.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sarc_encodeData.h>
#include <mh_cic_commandInterfaceTypes.h>

// set debug output
#ifdef ARDUINO
static Stream& encodeDebugStream = Serial;
#elif __linux__
#error "mh_encodeData_arrays linux implementation incomplete"
#elif _WIN32
#error "mh_encodeData_arrays windows implementation incomplete"
#else
#error "mh_encodeData_arrays unknown compilation environment"
#endif // set debug output


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
clearArrayElements(const SAC_FLAGS_TYPE valuesToClear,
                   const int arraySize,
                   SAC_CONVERTED_TYPE *dataArray)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // check input data
    if (dataArray == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
    }
    if ((arraySize <= 0) || (valuesToClear == 0))
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
    }
    // clear data only if success from input data check
    if (retVal == SAC_SUCCESS)
    {
        SAC_FLAGS_TYPE bitsRemaining = valuesToClear;
        // loop maximum is minimum of bitCount and arraySize
        int bitCount = 8 * sizeof(SAC_FLAGS_TYPE);
        int loopMax = min(bitCount, arraySize);
        SAC_FLAGS_TYPE sensorSelectBit = 1;
        for (int i=0; i<loopMax; i++)
        {
            if (valuesToClear & sensorSelectBit)
            {
                dataArray[i] = 0;
                bitsRemaining &= ~sensorSelectBit;
            } // end if (valuesToClear & sensorSelectBit)
            sensorSelectBit <<= 1;
        } // end for (i)
        if (bitsRemaining != 0)
        {
            retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
        } // end if (bitsRemaining != 0)
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_SIZE_TYPE clearData


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
encodeDataArray(char *bufferPtr,
                const size_t bufferSize,
                size_t &bytesEncoded,
                const measurementTypeEnum measurementType,
                const SAC_ERROR_TYPE errorFlags,
                const SAC_FLAGS_TYPE valuesToPrint,
                const SAC_FLAGS_TYPE valuesInError,
                const size_t arraySize,
                const SAC_CONVERTED_TYPE *dataArray)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // intial values of parameters passed by reference
    bytesEncoded = 0;
    // check input
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("encodeData() buffer pointer is "));
        encodeDebugStream.print((unsigned long) bufferPtr, HEX);
        encodeDebugStream.println();
        encodeDebugStream.print(F("encodeData() valuesToPrint ( "));
        printSensirionFlagsType(encodeDebugStream, valuesToPrint);
        encodeDebugStream.println(F(" )"));
        encodeDebugStream.print(F("encodeData() valuesInError ( "));
        printSensirionFlagsType(encodeDebugStream, valuesInError);
        encodeDebugStream.println(F(" )"));
        encodeDebugStream.print(F("encodeData() array pointer is "));
        encodeDebugStream.print((unsigned long) dataArray, HEX);
        encodeDebugStream.println();
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    if (bufferPtr == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.println(F("encodeData() null buffer pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    if (dataArray == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.println(F("encodeData() null array pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    if (bufferSize <= 0)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.println(F("encodeData() bufferSize < 0, returning SAC_BAD_COUNT"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    if (valuesToPrint == 0)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.println(F("encodeData() valuesToPrint == 0, returning SAC_BAD_COUNT"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    // test sufficiency of buffer size
    size_t dataBytes = computeMessageSize<SAC_SIZE_TYPE, SAC_MEASTYPE_TYPE, SAC_ERROR_TYPE, SAC_FLAGS_TYPE, SAC_CONVERTED_TYPE>(valuesToPrint);
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
    encodeDebugStream.print(F("encodeData() computed size is "));
    encodeDebugStream.println(dataBytes);
    encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    if (bufferSize < dataBytes)
    {
        retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("encodeData() buffer size < data bytes, "));
        encodeDebugStream.print(bufferSize);
        encodeDebugStream.print(F(" of "));
        encodeDebugStream.println(dataBytes);
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    // end of input testing
    // if valid input, encode data
    if (retVal == SAC_SUCCESS)
    {
        bytesEncoded = dataBytes;
        SAC_SIZE_TYPE encodedSize = dataBytes;
        char *workPtr = bufferPtr;
        // message length
        memcpy((void*) workPtr, (void*) &encodedSize, sizeof(encodedSize));
        workPtr += sizeof(encodedSize);
        // measurement type
        SAC_MEASTYPE_TYPE measTypeVal = (SAC_MEASTYPE_TYPE) measurementType;
        memcpy((void*) workPtr, (void*) &measTypeVal, sizeof(SAC_MEASTYPE_TYPE));
        workPtr += sizeof(SAC_MEASTYPE_TYPE);
        // error flags
        memcpy((void*) workPtr, (void*) &errorFlags, sizeof(errorFlags));
        workPtr += sizeof(errorFlags);
        // values flags
        memcpy((void*) workPtr, (void*) &valuesToPrint, sizeof(valuesToPrint));
        workPtr += sizeof(valuesToPrint);
        // values in error
        memcpy((void*) workPtr, (void*) &valuesInError, sizeof(valuesInError));
        workPtr += sizeof(valuesInError);
        // measurement values
        if (valuesToPrint != 0)
        {
            int bitCount = 8 * sizeof(SAC_FLAGS_TYPE);
            int loopMax = min(bitCount, arraySize);
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<loopMax; i++)
            {
                if (valuesToPrint & sensorSelectBit)
                {
                    memcpy((void*) workPtr, (void*) &dataArray[i], sizeof(dataArray[i]));
                    workPtr += sizeof(dataArray[i]);
                } // end if (valuesToPrint & sensorSelectBit)
                sensorSelectBit <<= 1;
            } // end for (i)
        } // end if (valuesToPrint != 0)
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("encodeData() encoded "));
        encodeDebugStream.print(dataBytes);
        encodeDebugStream.println(F(" bytes"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    else
    {
        bytesEncoded = 0;
        // set buffer to zeros
        if (bufferSize > 0)
        {
            memset(bufferPtr, 0, bufferSize);
        } // end if (bufferSize > 0)
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("encodeData() encoded "));
        encodeDebugStream.print(dataBytes);
        encodeDebugStream.println(F(" bytes"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE encodeDataArray()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
outputDataArray(Stream &outputDevice,
                size_t &bytesWritten,
                const measurementTypeEnum measurementType,
                const SAC_ERROR_TYPE errorFlags,
                const SAC_FLAGS_TYPE valuesToPrint,
                const SAC_FLAGS_TYPE valuesInError,
                const int arraySize,
                const SAC_CONVERTED_TYPE *dataArray)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // set initial values for values returned by reference
    bytesWritten = 0;
    // check input data
    if (dataArray == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
#ifdef MH_SARC_DEBUG_OUTPUTDATAARRAY
        encodeDebugStream.println(F("outputDataArray() null buffer pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_OUTPUTDATAARRAY
    }
    if (valuesToPrint == 0)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
#ifdef MH_SARC_DEBUG_OUTPUTDATAARRAY
        encodeDebugStream.println(F("outputDataArray() null array pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_OUTPUTDATAARRAY
    }
    // if valid input, output data
    if (retVal == SAC_SUCCESS)
    {
        // message size
        size_t messageLength = computeMessageSize<SAC_SIZE_TYPE, SAC_MEASTYPE_TYPE, SAC_ERROR_TYPE, SAC_FLAGS_TYPE, SAC_CONVERTED_TYPE>(valuesToPrint);
        SAC_SIZE_TYPE msgLenEnc = messageLength;
        bytesWritten += outputDevice.write((uint8_t*) &msgLenEnc, sizeof(msgLenEnc));
        // measurement type
        SAC_MEASTYPE_TYPE measTypeVal = (SAC_MEASTYPE_TYPE) measurementType;
        bytesWritten += outputDevice.write((uint8_t*) &measTypeVal, sizeof(measTypeVal));
        // error flags
        bytesWritten += outputDevice.write((uint8_t*) &errorFlags, sizeof(errorFlags));
        // values flags
        bytesWritten += outputDevice.write((uint8_t*) &valuesToPrint, sizeof(valuesToPrint));
        // values in error
        bytesWritten += outputDevice.write((uint8_t*) &valuesInError, sizeof(valuesInError));
        // measurement values
        if ((valuesToPrint != 0) && (dataArray != NULL))
        {
            int bitCount = 8 * sizeof(SAC_FLAGS_TYPE);
            int loopMax = min(bitCount, arraySize);
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<loopMax; i++)
            {
                if (valuesToPrint & sensorSelectBit)
                {
                    bytesWritten += outputDevice.write((uint8_t*) &dataArray[i], sizeof(SAC_CONVERTED_TYPE));
                } // end if (valuesToPrint & sensorSelectBit)
                sensorSelectBit <<= 1;
            } // end for (i)
        } // end if (valuesToPrint != 0)
        if (bytesWritten != messageLength)
        {
            retVal &= SAC_COMM_ERROR;
        } // end if (bytesWritten != messageLength)
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end outputDataArray(Stream &outputDevice, ... )


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::
decodeDataArray(char *bufferPtr,
                const size_t bufferSize,
                size_t &bytesDecoded,
                SAC_SIZE_TYPE &reportedSize,
                measurementTypeEnum &reportedType,
                SAC_ERROR_TYPE &reportedErrors,
                SAC_FLAGS_TYPE &valuesStated,
                SAC_FLAGS_TYPE &valuesInError,
                SAC_FLAGS_TYPE &valuesReceived,
                const int arraySize,
                SAC_CONVERTED_TYPE *dataArray)
{
    SAC_ERROR_TYPE retVal = SAC_SUCCESS;
    // set initial values for values returned by reference
    bytesDecoded = 0;
    reportedSize = 0;
    reportedType = MH_NO_MEASUREMENT;
    reportedErrors = SAC_SUCCESS;
    valuesStated = 0;
    valuesReceived = 0;
    // test call parameters
    if (bufferPtr == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
#ifdef MH_SARC_DEBUG_DECODEDATAARRAY
        encodeDebugStream.println(F("encodeData() null buffer pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_DECODEDATAARRAY
    }
    if (dataArray == NULL)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_ARRAY_PTR;
#ifdef MH_SARC_DEBUG_DECODEDATAARRAY
        encodeDebugStream.println(F("encodeData() null array pointer, returning SAC_BAD_ARRAY_PTR"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_DECODEDATAARRAY
    }
    if (bufferSize <= 0)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
#ifdef MH_SARC_DEBUG_DECODEDATAARRAY
        encodeDebugStream.print(F("decodeDataArray() bad buffer size, "));
        encodeDebugStream.print(bufferSize);
        encodeDebugStream.println(F(", returning SAC_BAD_COUNT"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_DECODEDATAARRAY
    }
    if (arraySize <= 0)
    {
        retVal |= (SAC_ERROR_TYPE)  SAC_BAD_COUNT;
#ifdef MH_SARC_DEBUG_DECODEDATAARRAY
        encodeDebugStream.print(F("decodeDataArray() bad array size, "));
        encodeDebugStream.print(arraySize);
        encodeDebugStream.println(F(", returning SAC_BAD_COUNT"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_DECODEDATAARRAY
    }
    // verify buffer is larger than fixed data size
    size_t minimumSize = computeBaseSize<SAC_SIZE_TYPE, SAC_MEASTYPE_TYPE, SAC_ERROR_TYPE, SAC_FLAGS_TYPE>();
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
    encodeDebugStream.print(F("decodeDataArray() minimum size is "));
    encodeDebugStream.println(minimumSize);
    encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    if (bufferSize < minimumSize)
    {
        // buffer is too small to decode header
        retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("encodeData() insufficient memory, bufferSize "));
        encodeDebugStream.print(bufferSize);
        encodeDebugStream.print(F(" of "));
        encodeDebugStream.print(minimumSize);
        encodeDebugStream.println(F(" needed"));
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
    }
    // decode header to verify buffer is large enough to read the complete message
    if (retVal == SAC_SUCCESS)
    {
        // read fixed data to determine complete message size
        bytesDecoded = minimumSize;
        char *workPtr = bufferPtr;
        // message length
        memcpy((void*) &reportedSize, (void*) workPtr, sizeof(SAC_SIZE_TYPE));
        workPtr += sizeof(SAC_SIZE_TYPE);
        // measurement type
        SAC_MEASTYPE_TYPE measTypeVal = MH_NO_MEASUREMENT;
        memcpy((void*) &measTypeVal, (void*) workPtr, sizeof(SAC_MEASTYPE_TYPE));
        workPtr += sizeof(SAC_MEASTYPE_TYPE);
        reportedType = (measurementTypeEnum) measTypeVal;
        // error flags
        memcpy((void*) &reportedErrors, (void*) workPtr, sizeof(SAC_ERROR_TYPE));
        workPtr += sizeof(SAC_ERROR_TYPE);
        // values flags
        memcpy((void*) &valuesStated, (void*) workPtr, sizeof(SAC_FLAGS_TYPE));
        workPtr += sizeof(SAC_FLAGS_TYPE);
        // values in error
        memcpy((void*) &valuesInError, (void*) workPtr, sizeof(SAC_FLAGS_TYPE));
        workPtr += sizeof(SAC_FLAGS_TYPE);
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("decodeDataArray() bytes decoded = "));
        encodeDebugStream.println(bytesDecoded);
        encodeDebugStream.flush();
        encodeDebugStream.print(F("decodeDataArray() reported size = "));
        encodeDebugStream.print(reportedSize);
        encodeDebugStream.print(F(", 0x"));
        encodeDebugStream.println(reportedSize, HEX);
        encodeDebugStream.flush();
        encodeDebugStream.print(F("decodeDataArray() reported errors = "));
        printSacErrorsType(encodeDebugStream, reportedErrors);
        encodeDebugStream.println();
        encodeDebugStream.flush();
        encodeDebugStream.print(F("  values stated                 "));
        printSensirionFlagsType(encodeDebugStream, valuesStated);
        encodeDebugStream.println();
        encodeDebugStream.flush();
        encodeDebugStream.print(F("  values in error               "));
        printSensirionFlagsType(encodeDebugStream, valuesInError);
        encodeDebugStream.println();
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
        // verify header data
        // test sufficiency of buffer size
        size_t computedSize = computeMessageSize<SAC_SIZE_TYPE, SAC_MEASTYPE_TYPE, SAC_ERROR_TYPE, SAC_FLAGS_TYPE, SAC_CONVERTED_TYPE>(valuesStated);
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
        encodeDebugStream.print(F("decodeDataArray() computed size is "));
        encodeDebugStream.println(computedSize);
        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
        if (bufferSize < computedSize)
        {
            retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
            encodeDebugStream.print(F("encodeData() insufficient memory, bufferSize "));
            encodeDebugStream.print(bufferSize);
            encodeDebugStream.print(F(" of "));
            encodeDebugStream.print(computedSize);
            encodeDebugStream.println(F(" needed"));
            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY        }
        }
        int valueCount = flagCount(valuesStated);
        if (arraySize < valueCount)
        {
            retVal |= (SAC_ERROR_TYPE) SAC_INSUFF_MEMORY;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
            encodeDebugStream.print(F("encodeData() arraySize < valueCount, "));
            encodeDebugStream.print(arraySize);
            encodeDebugStream.print(F(" of "));
            encodeDebugStream.print(valueCount);
            encodeDebugStream.println(F(" needed"));
            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
        }
#ifndef MH_ENCODEDATA_ALLOW_EMPTY_ARAY
        if (valueCount == 0)
        {
            retVal |= (SAC_ERROR_TYPE) SAC_BAD_COUNT;
        }
#endif // MH_ENCODEDATA_ALLOW_EMPTY_ARAY
        if (reportedSize != computedSize)
        {
            retVal |= (SAC_ERROR_TYPE) SAC_COMM_ERROR;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
            encodeDebugStream.print(F("encodeData() reportedSize != computedSize, "));
            encodeDebugStream.print(reportedSize);
            encodeDebugStream.print(F(" vs. "));
            encodeDebugStream.println(computedSize);
            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
        }
        // code block for decoding data
        // checks inside this block prevent segmentation fault for
        // - buffer size < computed size
        // - array size < max index indicated in flags
        // - valueCount == 0 (no action)
        // reported size < computed size resuls in decoding only to reported size
        {
            // clear values at array offsets claimed in message
            // clearArrayElements has protection against clearing values
            // beyond array size
            retVal |= clearArrayElements(valuesStated,
                                         arraySize,
                                         dataArray);
            // decode array elements
            // set loop counter to not exceed array bounds
            // internal checks prevent reading past min of message length and reported message size
            int bitCount = 8 * sizeof(SAC_FLAGS_TYPE);
            int loopMax = min(bitCount, arraySize);
            SAC_FLAGS_TYPE sensorSelectBit = 1;
            for (int i=0; i<loopMax; i++)
            {
                // if value is stated, array element should be present
                if (valuesStated & sensorSelectBit)
                {
                    // verify array size and buffer size are sufficient to read current offset
                    if ((bytesDecoded + sizeof(SAC_CONVERTED_TYPE)) <= bufferSize)
                    {
                        // verify sufficient message bytes available for read
                        if ((bytesDecoded + sizeof(SAC_CONVERTED_TYPE) <= reportedSize))
                        {
                            // read array value
                            memcpy((void*) &dataArray[i], (void*) workPtr, sizeof(SAC_CONVERTED_TYPE));
                            workPtr += sizeof(SAC_CONVERTED_TYPE);
                            valuesReceived |= sensorSelectBit;
                            bytesDecoded += sizeof(SAC_CONVERTED_TYPE);
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
                            encodeDebugStream.print(F("  array["));
                            encodeDebugStream.print(i);
                            encodeDebugStream.print(F("] = "));
                            encodeDebugStream.println(dataArray[i]);
                            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
                        }
                        else
                        {
                            // insufficient buffer size to read data
                            retVal |= (SAC_ERROR_TYPE)  SAC_COMM_ERROR;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
                            encodeDebugStream.print(F("encodeData() decode would exceed reporteSize, "));
                            encodeDebugStream.print(bytesDecoded + sizeof(SAC_CONVERTED_TYPE));
                            encodeDebugStream.print(F(" of "));
                            encodeDebugStream.println(reportedSize);
                            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
                        } // end if ((bytesDecoded + sizeof(SAC_CONVERTED_TYPE) <= reportedSize))
                    }
                    else
                    {
                        // insufficient array size to interperet data
                        retVal |= (SAC_ERROR_TYPE)  SAC_INSUFF_MEMORY;
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
                        encodeDebugStream.print(F("encodeData() decode would exceed bufferSize, "));
                        encodeDebugStream.print(bytesDecoded + sizeof(SAC_CONVERTED_TYPE));
                        encodeDebugStream.print(F(" of "));
                        encodeDebugStream.println(bufferSize);
                        encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
                    } // end if ((bytesDecoded + sizeof(SAC_CONVERTED_TYPE)) <= bufferSize)
                } // end if (valuesToClear & sensorSelectBit)
                sensorSelectBit <<= 1;
            } // end for (i)
#ifdef MH_SARC_DEBUG_ENCODEDATAARRAY
            encodeDebugStream.print(F("decodeData() valuesReceived = "));
            printSensirionFlagsType(encodeDebugStream, valuesReceived);
            encodeDebugStream.println();
            encodeDebugStream.flush();
#endif // MH_SARC_DEBUG_ENCODEDATAARRAY
        } // end code block for decoding data
    } // end if (retVal == SAC_SUCCESS)
    return retVal;
} // end SAC_ERROR_TYPE decodeDataArray()




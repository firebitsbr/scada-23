// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_encodeData_templates linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_encodeData_templates windows implementation incomplete"
#else
#error "mh_sarc_encodeData_templates unknown compilation environment"
#endif // determine environment

#include <string.h>
#include <stdint.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sarc_encodeData.h>
#include <mh_cic_commandInterfaceTypes.h>


// !!
// !! See warnings in mh_sarc_encodeData.h for malicious code and pointer issues
// !!

// count bits set
template <class T_FlagsType>
int
mh_temperatureSensing::
flagCount(const T_FlagsType flags)
{
    int retVal = 0;
    if (flags != 0)
    {
        int bitCount = 8 * sizeof(T_FlagsType);
        T_FlagsType sensorSelectBit = 1;
        for (int i=0; i<bitCount; i++)
        {
            if (flags & sensorSelectBit)
            {
                retVal++;
            } // end if (flags & sensorSelectBit)
            sensorSelectBit <<= 1;
        } // end for (i)
    } // end if (flags != 0)
    return retVal;
} // end template int flagCount(const T_flagsType flags)

template
int
mh_temperatureSensing::
flagCount<unsigned char>(unsigned char);


// base message size
template <class T_SizeType, class T_MeasurementType, class T_ErrorType, class T_FlagsType>
size_t
mh_temperatureSensing::
computeBaseSize(void)
{
    static const size_t messageLength = sizeof(T_SizeType) +
            sizeof(T_MeasurementType) +
            sizeof(T_ErrorType) +
            sizeof(T_FlagsType) +
            sizeof(T_FlagsType);
    return messageLength;
} // end template <> computeBaseSize()

template
size_t
mh_temperatureSensing::
computeBaseSize<mh_temperatureSensing::SAC_SIZE_TYPE,
                mh_temperatureSensing::SAC_MEASTYPE_TYPE,
                mh_temperatureSensing::SAC_ERROR_TYPE,
                mh_temperatureSensing::SAC_FLAGS_TYPE>(void);


// compute message size
template <class T_SizeType, class T_MeasurementType, class T_ErrorType, class T_FlagsType, class T_DataType>
size_t
mh_temperatureSensing::
computeMessageSize(T_FlagsType const flags)
{
    // count values to be reported
    size_t valueCount = flagCount(flags);
    // calculate message length
    size_t messageLength = computeBaseSize<T_SizeType, T_MeasurementType, T_ErrorType, T_FlagsType>();
    messageLength += sizeof(T_DataType) * valueCount;
    return messageLength;
} // template size_t computeMessageSize()


template
size_t
mh_temperatureSensing::
computeMessageSize<mh_temperatureSensing::SAC_SIZE_TYPE,
                   mh_temperatureSensing::SAC_MEASTYPE_TYPE,
                   mh_temperatureSensing::SAC_ERROR_TYPE,
                   mh_temperatureSensing::SAC_FLAGS_TYPE,
                   mh_temperatureSensing::SAC_CONVERTED_TYPE>(SAC_FLAGS_TYPE valuesToPrint);


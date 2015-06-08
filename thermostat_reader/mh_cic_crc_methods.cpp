// CRC methods are a minor modification of open source crc implementation from
// http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
// also available at
// http://www.boost.org/doc/libs/1_56_0/libs/crc/
// distributed under the boost license
// http://www.boost.org/doc/libs/1_56_0/LICENSE_1_0.txt
// alternative implementations at sourceforge
// http://sourceforge.net/projects/crccalculator/files/CRC/

// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_cic_commandInterfaceClass linux implementation incomplete"
#elif _WIN32
#error "mh_cic_commandInterfaceClass windows implementation incomplete"
#else
#error "mh_cic_commandInterfaceClass unknown compilation environment"
#endif // determine environment

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


int
mh_temperatureSensing::commandInterfaceClass::
crcInit(CIC_CRC_TYPE &crcValue)
{
    // based on an open source crc implementation from
    // http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
    crcValue = crcInitialValue;
    return 0;
} // end int crcInit(CIC_CRC_TYPE &crcValue)


int
mh_temperatureSensing::commandInterfaceClass::
updateCrc(CIC_CRC_TYPE &crcValue,
          const uint8_t* dataAddress,
          const size_t numBytes)
{
// minor modification of open source crc implementation from
// http://www.barrgroup.com/Embedded-Systems/How-To/CRC-Calculation-C-Code
// also available at
// http://www.boost.org/doc/libs/1_56_0/libs/crc/
// distributed under the boost license
// http://www.boost.org/doc/libs/1_56_0/LICENSE_1_0.txt
// alternative implementations at sourceforge
// http://sourceforge.net/projects/crccalculator/files/CRC/
    if (dataAddress == NULL)
    {
        return -1;
    }
    if (numBytes < 0)
    {
        return -2;
    }

    const int width = (8 * sizeof(CIC_CRC_TYPE));
    const CIC_CRC_TYPE topbit = (1 << (width - 1));

    /*
     * Perform modulo-2 division, a byte at a time.
     */
    for (size_t byte = 0; byte < numBytes; ++byte)
    {
        /*
         * Bring the next byte into the remainder.
         */
        crcValue ^= (dataAddress[byte] << (width - 8));

        /*
         * Perform modulo-2 division, a bit at a time.
         */
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            /*
             * Try to divide the current data bit.
             */
            if (crcValue & topbit)
            {
                crcValue = (crcValue << 1) ^ crcPolynomial;
            }
            else
            {
                crcValue = (crcValue << 1);
            } // end
        } // end for (bit)
    } // end for (byte)
    return 0;
} // end int updateCrc(CIC_CRC_TYPE &crcValue, ... )


int
mh_temperatureSensing::commandInterfaceClass::
completeCrc(CIC_CRC_TYPE &crcValue)
{
    // remainder is calculated in crcUpdate()
    // no action
    return 0;
} // end int completeCrc(CIC_CRC_TYPE &crcValue)


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
checkPayloadPtrAndSize(void)
{
    return checkPayloadPtrAndSize(m_payloadBufferPtr,
                                  m_payloadBufferSize);
} // end CIC_ERROR_TYPE checkPayloadPtrAndSize(void)



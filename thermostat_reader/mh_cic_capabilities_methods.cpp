#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>


/// NOTE: respondCapabilities not complete
mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
respondCapabilities(Stream &serialDevice,
                    const size_t payloadSize)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    if (m_payloadBufferPtr == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if (m_payloadBufferSize < payloadSize);
    {
        retVal |= CIC_ERR_INSUFF_MEMORY;
    }
    if (retVal == CIC_SUCCESS)
    {
        const int messageSize = framingSize + payloadSize;
        int bytesWritten = 0;
        retVal |= sendMessage(serialDevice,
                              bytesWritten,
                              m_receivedSourceAddress,
                              CIC_MSG_CAPABILITIES);
        if (bytesWritten != messageSize)
    {
        retVal |= CIC_COMM_ERROR;
    } // end if (bytesWritten != messageSize)
} // end if (retVal == CIC_SUCCESS)
return retVal;
} // end CIC_ERROR_TYPE respondCapabilities(Stream &serialDevice)



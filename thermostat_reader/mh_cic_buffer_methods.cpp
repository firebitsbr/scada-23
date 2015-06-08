#include <stdint.h>
#include <stddef.h>

#include <mh_cic_commandInterfaceTypes.h>
#include <mh_cic_commandInterfaceClass.h>

mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
allocateBuffer(const size_t bufferSize)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // check input
    if ((bufferSize <= 0) || (bufferSize > cicSizeTypeMax))
    {
        retVal |= CIC_ERR_BAD_COUNT;
        if (m_payloadBufferPtr == NULL)
        {
            retVal |= CIC_ERR_BAD_ARRAY_PTR;
        } // end if (m_payloadBufferPtr == NULL)
    } // end if ((bufferSize > cicSizeTypeMax) || (bufferSize > cicSizeTypeMax))
    if (retVal == CIC_SUCCESS)
    {
        if ((m_payloadBufferPtr != NULL) && (m_bufferSource == ALLOCATED))
        {
            delete m_payloadBufferPtr;
            m_payloadBufferPtr = NULL;
            m_payloadBufferSize = 0;
            m_bufferSource = NO_BUFFER;
            m_payloadByteCounter = 0;
        }
        m_payloadBufferPtr = new char[bufferSize];
        if (m_payloadBufferPtr != NULL)
        {
            m_payloadBufferSize = bufferSize;
            m_bufferSource = ALLOCATED;
            m_payloadByteCounter = 0;
        }
        else
        {
            retVal |= CIC_ERR_BAD_ARRAY_PTR;
        } // end if (m_payloadBufferPtr != NULL)
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE allocateBuffer(size_t bufferSize)


mh_temperatureSensing::CIC_ERROR_TYPE
mh_temperatureSensing::commandInterfaceClass::
setBufferByReference(const size_t newBufferSize,
                     char* newBufferAddress)
{
    CIC_ERROR_TYPE retVal = CIC_SUCCESS;
    // check input
    if (newBufferAddress == NULL)
    {
        retVal |= CIC_ERR_BAD_ARRAY_PTR;
    }
    if (newBufferSize <= 0)
    {
        retVal |= CIC_ERR_BAD_COUNT;
        if (m_payloadBufferPtr == NULL)
        {
            retVal |= CIC_ERR_BAD_ARRAY_PTR;
        } // end if (m_payloadBufferPtr == NULL)
    } // end if (newBufferSize <= 0)
    if (retVal == CIC_SUCCESS)
    {
        if ((m_payloadBufferPtr != NULL) && (m_bufferSource == ALLOCATED))
        {
            delete m_payloadBufferPtr;
        } // end if ((m_payloadBufferPtr != NULL) && (m_bufferSource == ALLOCATED))
        m_payloadBufferPtr = newBufferAddress;
        // check buffer size but allow use of
        // oversize buffer
        if (newBufferSize <= cicSizeTypeMax)
        {
            m_payloadBufferSize = newBufferSize;
        }
        else
        {
            m_payloadBufferSize = cicSizeTypeMax;
            retVal |= CIC_ERR_BAD_COUNT;
        } // end if (bufferSize > cicSizeTypeMax)
        m_bufferSource = BY_REFERENCE;
        m_payloadByteCounter = 0;
    } // end if (retVal == CIC_SUCCESS)
    return retVal;
} // end CIC_ERROR_TYPE allocateBuffer(size_t bufferSize)



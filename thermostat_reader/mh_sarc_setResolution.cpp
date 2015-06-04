// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_setResolution linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_setResolution windows implementation incomplete"
#else
#error "mh_sarc_setResolution unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <string.h>
#include <stddef.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>
#include <mh_sarc_sensirionArrayReaderClass.h>
#include <mh_sarc_encodeData.h>


// command sensors to configure for high resolution
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
setHighRes(void)
{
    return setResolution(false);
} // end int setHighRes()


// command sensors to configure for low resolution
mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
setLowRes(void)
{
    return setResolution(true);
} // end int setLowRes()


mh_temperatureSensing::SAC_ERROR_TYPE
mh_temperatureSensing::sensirionArrayReaderClass::
setResolution(const bool setLowResolution)
{
    SAC_ERROR_TYPE retVal = 0;
    // configure member variables for new resolution
    if (setLowResolution)
    {
        m_tempTimeoutMillis = lowResTempTimeout;
        m_humidTimeoutMillis = lowResHumidTimeout;
    }
    else
    {
        m_tempTimeoutMillis = highResTempTimeout;
        m_humidTimeoutMillis = highResHumidTimeout;
    } // end if (setLowResolution;
    // configure sensors for new resolution
    m_resolutionErrorFlags = 0;
    uint8_t workVar = 0;
    SAC_FLAGS_TYPE sensorSelectBit = 1;
    for (int i=0; i<maxSensors; i++)
    {
        if (sensorSelectBit & m_sensorActiveMask)
        {
            uint8_t errCode = m_sensorPtrArray[i]->readSR(&workVar);
            if (errCode == 0)
            {
#ifdef MH_SARC_DEBUG_SENSOR_SETTINGS
                sarcDebugStream.print(F("Sensor "));
                sarcDebugStream.print(i);
                if (workVar & LOW_RES)
                {
                    sarcDebugStream.println(F(" was low resolution"));
                }
                else
                {
                    sarcDebugStream.println(F(" was high resolution"));
                } // end if (workVar & LOW_RES)
#endif // MH_SARC_DEBUG_SENSOR_SETTINGS
                if (setLowResolution)
                {
                    workVar |= LOW_RES;
                }
                else
                {
                    workVar &= (~LOW_RES);
                } // end if (setLowResolution)
                if (m_sensorPtrArray[i]->writeSR(workVar) == 0)
                {
                    // success
                    // no-op
#ifdef MH_SARC_DEBUG_SENSOR_SETTINGS
                    sarcDebugStream.print(F("Sensor "));
                    sarcDebugStream.print(i);
                    if (workVar & LOW_RES)
                    {
                        sarcDebugStream.println(F(" now low resolution"));
                    }
                    else
                    {
                        sarcDebugStream.println(F(" now high resolution"));
                    } // end if (workVar & LOW_RES)
#endif // MH_SARC_DEBUG_SENSOR_SETTINGS
                }
                else
                {
                    m_resolutionErrorFlags |= sensorSelectBit;
                    retVal = SAC_RESOLUTION_ERROR;
#ifdef MH_SARC_DEBUG_SENSOR_SETTINGS
                    sarcDebugStream.print(F("Error - sensor "));
                    sarcDebugStream.print(i);
                    sarcDebugStream.println(F(" not set"));
#endif // MH_SARC_DEBUG_SENSOR_SETTINGS
                } // end if (m_sensorPtrArray[i]->writeSR(&workVar) != 0)
            }
            else
            {
                m_resolutionErrorFlags |= sensorSelectBit;
                retVal = SAC_RESOLUTION_ERROR;
#ifdef MH_SARC_DEBUG_SENSOR_SETTINGS
                sarcDebugStream.print(F("Error - sensor "));
                sarcDebugStream.print(i);
                sarcDebugStream.println(F(" unable to set resolution (could not read sensor register)"));
#endif // MH_SARC_DEBUG_SENSOR_SETTINGS
            } // end if (errCode == 0)
        } // end if (sensorSelectBit & m_sensorActiveMask)
        sensorSelectBit <<= 1;
    } // end for (i=0; i<maxSensors; i++)
    return retVal;
} // end int setResolution()


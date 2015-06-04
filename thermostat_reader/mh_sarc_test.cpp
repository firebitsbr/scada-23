// determine environment
#ifdef ARDUINO
#include <Arduino.h>
#elif __linux__
#error "mh_sarc_test linux implementation incomplete"
#elif _WIN32
#error "mh_sarc_test windows implementation incomplete"
#else
#error "mh_sarc_test unknown compilation environment"
#endif // determine environment

#include <Sensirion.h>
#include <string.h>
#include <stddef.h>

#include <mh_sac_sensirionArrayTypes.h>
#include <mh_sac_sensirionArrayClass.h>
#include <mh_sarc_sensirionArrayReaderClass.h>
#include <mh_sarc_encodeData.h>
#include <mh_sarc_test.h>


// test setting resolution
int
mh_temperatureSensing::
exerciseSensorSettings(Stream &serialDevice,
                       sensirionArrayReaderClass *readerToTest)
{
    int retVal = 0;
    serialDevice.println(F("exercising sensor settings start"));
    serialDevice.print(F("sensors to set "));
    serialDevice.println(readerToTest->m_sensorActiveMask, HEX);
    if (readerToTest->setLowRes() != 0) retVal -= 1;
    if (readerToTest->setHighRes() != 0) retVal -= 2;
    if (readerToTest->setLowRes() != 0) retVal -= 4;
    if (readerToTest->setHighRes() != 0) retVal -= 8;
    serialDevice.print(F("exercising sensor settings complete, return value "));
    serialDevice.println(retVal);
    return retVal;
} // end int exercise SensorSettings()

int
mh_temperatureSensing::
exerciseSensirionArrayReaderClass(Stream &serialDevice,
                                  sensirionArrayReaderClass *sensirionReader)
{
    int retVal = 0;
    serialDevice.println(F("\nExercising sensirionArrayReaderClass, 15 seconds"));
    SAC_FLAGS_TYPE activeSensors = sensirionReader->getActiveMask();
    SAC_SIZE_TYPE bufferSize = computeMessageSize<SAC_SIZE_TYPE, SAC_MEASTYPE_TYPE, SAC_ERROR_TYPE, SAC_FLAGS_TYPE, SAC_CONVERTED_TYPE>(activeSensors);
    char* buffer = new char[bufferSize];
    serialDevice.print(F("Buffer size is "));
    serialDevice.println(bufferSize);
    serialDevice.print(F("Buffer address is "));
    serialDevice.println((unsigned long) buffer, HEX);
    serialDevice.print(F("State is "));
    printSensirionSensorStateEnum(serialDevice, sensirionReader->getState());
    serialDevice.println();
    sensirionReader->setLowRes();
    serialDevice.println();

    SAC_ERROR_TYPE errors = 0;
    size_t bytesEncoded = 0;
    size_t bytesDecoded = 0;
    SAC_SIZE_TYPE reportedSize = 0;
    measurementTypeEnum reportedType = MH_NO_MEASUREMENT;
    SAC_ERROR_TYPE decodeErrors = 0;
    SAC_ERROR_TYPE reportedErrors = 0;
    SAC_FLAGS_TYPE valuesStated = 0;
    SAC_FLAGS_TYPE valuesReceived = 0;
    SAC_FLAGS_TYPE valuesInError = 0;
    const int arraySize = 8;
    SAC_CONVERTED_TYPE dataArray[arraySize];

    sensirionReader->clearErrors();

    for (int rounds=0; rounds < 3; rounds++)
    {
        // read temperature
        serialDevice.println(F("\nTemperature"));
        sensirionReader->beginTempMeasurement(activeSensors);
        delay(sensirionReader->getTempTimeoutMillis());
        SAC_FLAGS_TYPE dataReadyFlags = 0;
        sensirionReader->checkSensorsTemp(dataReadyFlags);
        serialDevice.print(F("returned from checkSensorsTemp, dataReadyFlags   = ( "));
        printSensirionFlagsType(sarcDebugStream, dataReadyFlags);
        sarcDebugStream.println(F(" )"));
        serialDevice.print(F("returned from checkSensorsTemp, m_tempErrorFlags = ( "));
        printSensirionFlagsType(sarcDebugStream, sensirionReader->m_tempErrorFlags);
        sarcDebugStream.println(F(" )"));
        sensirionReader->printTemperatures(serialDevice, dataReadyFlags);
        errors = sensirionReader->encodeTemperatures(buffer, bufferSize, bytesEncoded, activeSensors);
        decodeErrors = decodeDataArray(buffer,
                                       bufferSize,
                                       bytesDecoded,
                                       reportedSize,
                                       reportedType,
                                       reportedErrors,
                                       valuesStated,
                                       valuesInError,
                                       valuesReceived,
                                       arraySize,
                                       dataArray);
        serialDevice.print(F("Read errors: "));
        printSacErrorsType(serialDevice, errors);
        serialDevice.println();
        serialDevice.print(F("Decode errors: "));
        printSacErrorsType(serialDevice, decodeErrors);
        serialDevice.println();
        serialDevice.print(F("Bytes decoded "));
        serialDevice.println(bytesDecoded);
        serialDevice.print(F("Reported size "));
        serialDevice.println(reportedSize);
        serialDevice.print(F("Reported type "));
        printMeasurementTypeEnum(serialDevice, reportedType);
        serialDevice.println();
        serialDevice.print(F("Reported errors "));
        printSacErrorsType(serialDevice, reportedErrors);
        serialDevice.println();
        serialDevice.print(F("Values stated "));
        serialDevice.println(valuesStated, HEX);
        serialDevice.print(F("Values received "));
        serialDevice.print(valuesReceived, HEX);
        serialDevice.print(F("  "));
        printSensirionFlagsType(serialDevice, valuesReceived);
        serialDevice.println();
        serialDevice.print(F("Values in error "));
        serialDevice.print(valuesInError, HEX);
        serialDevice.print(F("  "));
        printSensirionFlagsType(serialDevice, valuesInError);
        serialDevice.println();
        serialDevice.println(F("Data array:"));
        for (int i=0; i<arraySize; i++)
        {
            serialDevice.print(i);
            serialDevice.print(F("  "));
            serialDevice.print(dataArray[i]);
            serialDevice.print(F("  (read as "));
            serialDevice.print(sensirionReader->m_lastTemperatureArray[i]);
            serialDevice.println(F(")"));

        } // end for (i)
        serialDevice.println(F("end of temperatures\n\n"));

        // read humidity
        serialDevice.println(F("\nHumidity"));
        sensirionReader->beginHumidMeasurement(activeSensors);
        delay(sensirionReader->getHumidTimeoutMillis());
        dataReadyFlags = 0;
        sensirionReader->checkSensorsHumid(dataReadyFlags);
        serialDevice.print(F("returned from checkSensorsHumid, dataReadyFlags   = ( "));
        printSensirionFlagsType(sarcDebugStream, dataReadyFlags);
        sarcDebugStream.println(F(" )"));
        serialDevice.print(F("returned from checkSensorsHumid, m_humidErrorFlags = ( "));
        printSensirionFlagsType(sarcDebugStream, sensirionReader->m_humidErrorFlags);
        sarcDebugStream.println(F(" )"));
        sensirionReader->printHumidities(serialDevice, dataReadyFlags);
        errors = sensirionReader->encodeHumidities(buffer, bufferSize, bytesEncoded, activeSensors);
        decodeErrors = decodeDataArray(buffer,
                                       bufferSize,
                                       bytesDecoded,
                                       reportedSize,
                                       reportedType,
                                       reportedErrors,
                                       valuesStated,
                                       valuesInError,
                                       valuesReceived,
                                       arraySize,
                                       dataArray);
        serialDevice.print(F("Bytes decoded "));
        serialDevice.println(bytesDecoded);
        serialDevice.print(F("Reported size "));
        serialDevice.println(reportedSize);
        serialDevice.print(F("Reported type "));
        printMeasurementTypeEnum(serialDevice, reportedType);
        serialDevice.println();
        serialDevice.print(F("Reported errors "));
        printSacErrorsType(serialDevice, reportedErrors);
        serialDevice.println();
        serialDevice.print(F("Values stated "));
        serialDevice.println(valuesStated, HEX);
        serialDevice.print(F("Values Received "));
        serialDevice.print(valuesReceived, HEX);
        serialDevice.print(F("  "));
        printSensirionFlagsType(serialDevice, valuesReceived);
        serialDevice.println();
        serialDevice.print(F("Values in error "));
        serialDevice.print(valuesInError, HEX);
        serialDevice.print(F("  "));
        printSensirionFlagsType(serialDevice, valuesInError);
        serialDevice.println();
        serialDevice.println(F("Data array:"));
        for (int i=0; i<arraySize; i++)
        {
            serialDevice.print(i);
            serialDevice.print("  ");
            serialDevice.print(dataArray[i]);
            serialDevice.print(F("  (read as "));
            serialDevice.print(sensirionReader->m_lastHumidityArray[i]);
            serialDevice.println(F(")"));
        } // end for (i)
        serialDevice.println(F("end of humidity\n\n"));

        // wait
        serialDevice.println(F("Waiting 5 seconds"));
        delay(5000);
        serialDevice.print(F("\n\n"));
    } // end for (rounds)

    serialDevice.println(F("sensirionArrayReaderClass exercise complete"));
    return retVal;

}


/*

// potential driver code
sensirionArrayReaderClass sensirionReader(numDevices, dataPins, clockPins);
int srcRet = exerciseSensorSettings(Serial, &sensirionReader);
if (srcRet != 0)
{
  Serial.print("Error - exerciseSensorSettings() returned ");
  Serial.println(srcRet);
}
srcRet = exerciseSensirionArrayReaderClass(Serial, &sensirionReader);
if (srcRet != 0)
{
  Serial.print("Error - exerciseSensirionArrayReaderClass() returned ");
  Serial.print((unsigned char) srcRet);
  Serial.print(", ");
  Serial.println((unsigned char) srcRet, HEX);
}

*/


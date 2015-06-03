#ifndef MH_SARC_TEST_H_INCLUDED
#define MH_SARC_TEST_H_INCLUDED

/*! \brief Minimal test code to check connectivity to sensors by setting modes and reading data.
 *
 */

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

#include <mh_sarc_sensirionArrayReaderClass.h>

namespace mh_temperatureSensing
{

// test setting resolution
/// exercise SensorSettings() sets sensors to low and high resolution
int exerciseSensorSettings(Stream &serialDevice,
                           sensirionArrayReaderClass *readerToTest);

// exercise sensirionArrayReaderClass
/// exerciseSensirionArrayReaderClass() reads temperature and humidity
int exerciseSensirionArrayReaderClass(Stream &serialDevice,
                                      sensirionArrayReaderClass *sensirionReader);

} // close namespace mh_temperatureSensing



#endif // MH_SARC_TEST_H_INCLUDED

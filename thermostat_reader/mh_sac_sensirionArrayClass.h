#ifndef MH_SAC_SENSIRIONARRAYCLASS_H_INCLUDED
#define MH_SAC_SENSIRIONARRAYCLASS_H_INCLUDED

#include <Arduino.h>
#include <Sensirion.h>

#include "mh_sac_sensirionArrayTypes.h"

// this file contains the declarations for an object to contain an array temperature/humidity sensors

// WARNINGS
// as of 11/05/2014
// - initialize() must be called prior to use.  Member pointers to arrays and data structures may have
//   invalid but non-NULL values (compiler warns on initialization of non-static data)
// - Protection from null pointers to sensors is provided using the bit array m_sensorActiveMask.
//   For each sensor, a bit is set in m_sensorActiveMask, a Sensirion object has been allocated at that
//   index.  Functions are not otherwise written to test for null pointers.  If the bit is not set,
//   calls made to that index will cause null pointer errors and subsequent segmentation fault.
//   Similarly for arrays, if any array is not properly allocated the sensor mask is set to 0.  Function
//   calls accessing elements for which the corresponding mask bit is not set will cause segmentation fault.

// Notes
// as of 11/05/2014
// - Code creates compiler warnings for constant initialization of non-static class members.  Initialize
//   re-sets the class members. Test all implementations to verify member values are established prior to use.
// - Exception handling is disabled in the Arduino environment (compiled -fno-exceptions).  Addition of
//   exception handling around memory allocation is suggested if used with exceptions enabled.


namespace mh_temperatureSensing
{

class sensirionArrayClass
{
public:
    sensirionArrayClass();
    sensirionArrayClass(const int numSensors,
                        const int *dataPinArrayPtr,
                        const int *clockPinArrayPtr);
    ~sensirionArrayClass();
    // state and error data
    SAC_ERROR_TYPE getErrorFlags(void);
    sensirionSensorStateEnum getState(void);
    SAC_FLAGS_TYPE getActiveMask(void);
protected:
    // initialize non-array member variables
    void initializeNoAlloc(void);
    // initialize all member variables
    SAC_ERROR_TYPE initialize(const int numSensors,
                               const int *dataPinArrayPtr,
                               const int *clockPinArrayPtr);
    // free memory allocated during initializaton of arrays
    void freeMemory(void);
    // setState() sets the value of m_sensorState and returns its previous value
    sensirionSensorStateEnum setState(sensirionSensorStateEnum newState);
    // clearErrors() sets m_errorFlags to SAC_SUCCESS
    // clearErrors() returns previous error state
    SAC_ERROR_TYPE clearErrors(void);
protected:
    // number of sensors in array
    uint8_t m_numSensors = 0;
    // maximum number of sensors in array
    // initially implemented max is 8 (uint8_t for flags type)
    // maximum can be modified to 16, 32, 64 by changing typedef SAC_FLAGS_TYPE
    static const uint8_t maxSensors = 8 * sizeof(SAC_FLAGS_TYPE);
    // pointer to array of Sensirion devices
    Sensirion **m_sensorPtrArray = NULL;
    // bit fields to indicate what sensors are active
    // sensors currently active
    SAC_FLAGS_TYPE m_sensorActiveFlags = 0x00;
    // mask of sensors present and properly initialized
    SAC_FLAGS_TYPE m_sensorActiveMask = 0x00;
    // state of array
    // indicates both initialization and current state for use in state machine
    sensirionSensorStateEnum m_sensorState = SSS_UNINIT;
    // error flags
    // can indicate success (0) or multiple errors based on the bits set
    // see mh_sac_sensirionArrayTypes.h for enumeration of the possible errors
    SAC_ERROR_TYPE m_errorFlags = SAC_SUCCESS;
}; // end class sensirionArrayClass

} // close namespace mh_temperatureSensing

#endif // MH_SAC_SENSIRIONARRAYCLASS_H_INCLUDED

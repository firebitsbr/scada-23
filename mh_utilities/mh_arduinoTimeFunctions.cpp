// Notes:
// - actionDue() only works correctly if time elapsed is less than half the maximum
//   value that can be represented by an unsigned long value.  Wrapping from a value
//   near the maximum represented value to a value near zero is handled; however,
//   if the time elapsed exceeds half of the maximum represented value the function
//   will return 'false' in error.  Similarly, actionDue() only works correctly for
//   events scheduled less than half of the maximum represented value into the future.
//   For events more than half of the maximum represented value in the future, the
//   funtion will return 'true' in error.

// TODO: consider conversion of unsignedTimeDifference to template<typename T>
// TODO: consider conversion ofactionDue to template<typename T>
// TODO: consider conversion of millisTimeout() and microsTimeout() to call a
//       single set of instructions with a function pointer to the
//       current time function

// determine environment
#ifdef ARDUINO
    #include <Arduino.h>
#elif __linux__
    // Linux includes here
#elif _WIN32
    // Windows includes here
#else
    #error "mh_arduinoTimeFunctions unknown compilation environment"
#endif // determine environment

#include "mh_arduinoTimeFunctions.h"

// local work function
// not necessary because of mapping of both cases to the
// same operation but may make clear that both cases were
// considered.
static unsigned long
unsignedTimeDifference(const unsigned long startUnsigned,
                       const unsigned long endUnsigned)
{
    if (endUnsigned > startUnsigned)
    {
        return (endUnsigned - startUnsigned);
    }
    else
    {
        // negative case - unsigned wraps, same equation but ambiguous result
        return (endUnsigned - startUnsigned);
    } // end if (endUnsigned > startUnsigned)
} // end unsignedTimeDifference()


unsigned long
mh_timeFunctions::
microsElapsed(const unsigned long startMicros,
              const unsigned long endMicros)
{
    return unsignedTimeDifference(startMicros, endMicros);
} // end unsigned long microsElapsed()


unsigned long
mh_timeFunctions::
millisElapsed(const unsigned long startMillis,
              const unsigned long endMillis)
{
    return unsignedTimeDifference(startMillis, endMillis);
} // end unsigned long millisElapsed()


boolean
mh_timeFunctions::
actionDue(const unsigned long actionDueTime,
          const unsigned long callTime,
          const unsigned long wrapValue)
{
    boolean retVal = false;
    unsigned long timeElapsed = unsignedTimeDifference(actionDueTime, callTime);
    // operation may be counterintuitive
    // action is due if a non-negative value is returned for the time passed since the due time
    // because all unsigned values are positive, values less than the wrap value are accepted as
    // indicating a non-negative time elapsed.
    if (timeElapsed < wrapValue)
    {
        retVal = true;
    } // end if (timeElapsed < wrapValue)
    return retVal;
} // end boolean actionDue()


boolean
mh_timeFunctions::
millisTimeout(const unsigned long startMillis,
              const unsigned long timeoutMillis,
              const unsigned long wrapValue)
{
    unsigned long timeoutTime = startMillis + timeoutMillis;
    unsigned long currentMillis = millis();
    return actionDue(timeoutTime, currentMillis, wrapValue);
} // end boolean millisTimeout()


boolean
mh_timeFunctions::
microsTimeout(const unsigned long startMicros, const unsigned long timeoutMicros, const unsigned long wrapValue)
{
    unsigned long timeoutTime = startMicros + timeoutMicros;
    unsigned long currentMicros = micros();
    return actionDue(timeoutTime, currentMicros, wrapValue);
} // end boolean microsTimeout()


// Arduino-similar time functions
// use with other microcontrollers and operating systems requires
// defininition of the following methods:
// delay()
// delayMicroseconds()
// millis()
// micros()
#ifdef ARDUINO
    // no additional action
#elif __linux__
    #error "mh_arduinoTimeFunctions linux implementation incomplete"
#elif _WIN32
    #error "mh_arduinoTimeFunctions windows implementation incomplete"
#else
    #error "mh_arduinoTimeFunctions unknown compilation environment"
#endif // Arduino-similar time functions


#ifdef MH_INCLUDE_ARDUINOTIMEFUNCTIONS_TEST_CODE

int
mh_timeFunctions::
testMillisTimeout(void)
{
    int retVal = 0;
  Serial.println("test millisTimeout - output should be 10 false, 10 true");
  Serial.print("Start millis() ");
  unsigned long startMillis = millis();
  Serial.println(startMillis);
  boolean timedOut[20] = { false };
  unsigned int returnTime[20] = { 0 };
  for (int i=0; i<20; i++)
  {
    timedOut[i] = millisTimeout(startMillis, 1000);
    returnTime[i] = millis();
    delay(100);
  } // end for (i) (upper)
  for (int i=0; i<20; i++)
  {
    Serial.print("timeout ");
    Serial.print(timedOut[i]);
    Serial.print(" returned at ");
    Serial.print(returnTime[i]);
    if (i < 10)
    {
        if (timedOut[i])
        {
            Serial.println(" *** error - true before 100ms ");
            retVal--;
        }
        else
        {
            Serial.println();
        } // end if (timedOut[i])
    }
    else
    {
        if (timedOut[i])
        {
            Serial.println();
        }
        else
        {
            Serial.println(" *** error - false after 100ms ");
            retVal--;
        } // end if (timedOut[i])
    } // end if (i < 10)
  } // end for (i) (lower)
  return retVal;
} // end int testMillisTimeout()

#endif // MH_INCLUDE_ARDUINOTIMEFUNCTIONS_TEST_CODECTIONS

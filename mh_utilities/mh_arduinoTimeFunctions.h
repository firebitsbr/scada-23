#ifndef ARDUINOTIMEFUNCTIONS_H_INCLUDED
#define ARDUINOTIMEFUNCTIONS_H_INCLUDED

// simple time functions to handle near-term events

// Notes:
// - microsElapsed() and millisElapsed() are limited by the maximum unsigned values
//   that can be represented by their calling and return type.
// - actionDue() only works correctly if time elapsed is less than half the maximum
//   value that can be represented by an unsigned long value.  Wrapping from a value
//   near the maximum represented value to a value near zero is handled; however,
//   if the time elapsed exceeds half of the maximum represented value the function
//   will return 'false' in error.  Similarly, actionDue() only works correctly for
//   events scheduled less than half of the maximum represented value into the future.
//   For events more than half of the maximum represented value in the future, the
//   funtion will return 'true' in error.

// #define MH_INCLUDE_ARDUINOTIMEFUNCTIONS_TEST_CODE

namespace mh_timeFunctions
{

const unsigned long HALF_MAX_UNSIGNED_LONG = 0xfffffffful / 2ul;

unsigned long microsElapsed(const unsigned long startMicros,
                            const unsigned long endMicros);

unsigned long millisElapsed(const unsigned long startMillis,
                            const unsigned long endMillis);

boolean actionDue(const unsigned long actionDueTime,
                  const unsigned long callTime,
                  const unsigned long wrapValue = HALF_MAX_UNSIGNED_LONG);

boolean millisTimeout(const unsigned long startMillis,
                      const unsigned long timeoutMillis,
                      const unsigned long wrapValue = HALF_MAX_UNSIGNED_LONG);

boolean microsTimeout(const unsigned long startMicros,
                      const unsigned long timeoutMicros,
                      const unsigned long wrapValue = HALF_MAX_UNSIGNED_LONG);

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
#endif // determine environment

#ifdef MH_INCLUDE_ARDUINOTIMEFUNCTIONS_TEST_CODE
int testMillisTimeout(void);
#endif // MH_INCLUDE_ARDUINOTIMEFUNCTIONS_TEST_CODE

} // close namespace mh_timeFunctions

#endif // ARDUINOTIMEFUNCTIONS_H_INCLUDED

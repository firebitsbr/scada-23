#ifndef CONTRIBUTED_FREEMEM_H_INCLUDED
#define CONTRIBUTED_FREEMEM_H_INCLUDED


namespace mh_contributed {

int freeRam (void);
// calculate free ram for Arduino
// http://www.controllerprojects.com/2011/05/23/determining-sram-usage-on-arduino/

int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

} // close namespace mh_contributed

#endif // CONTRIBUTED_FREEMEM_H_INCLUDED

#ifndef _PUMP_H_
#define _PUMP_H_

#ifdef TEST
#include "arduino_emu.h"
#else
#include "Arduino.h"
#endif

class Pump {
    uint8_t pin; // pin which switch pump on/off
    uint8_t state; // HIGH  when on, or LOW when off
    uint8_t broken; // broken flag: 0 - servisable else broken 
  public:
    Pump(uint8_t); // constructor: set pin number
    void switchOn(); // switch pump on
    void switchOff(); // switch pump off
    uint8_t getState(); // get pump current state
    void exchange(); // switch to opposite state: from on to off and vice versa
    uint8_t isBroken(); // check is this pump broken or not connected
    void setBroken(uint8_t isBroken); //set pump into broken or servisable state
};

#endif

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
  public:
    Pump(uint8_t); // constructor: set pin number
    void switchOn(); // switch pump on
    void switchOff(); // switch pump off
    uint8_t getState(); // get pump current state
    void exchange(); // switch to opposite state: from on to off and vice versa
};

#endif

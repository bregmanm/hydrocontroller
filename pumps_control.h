#ifndef _PUMPS_CONTROL_H_
#define _PUMPS_CONTROL_H_
// Control of pumps group

#include "pump.h"

#define MAX_PUMPS_COUNT 2

class PumpsControl {
  Pump* pumps[MAX_PUMPS_COUNT]; // array of pumps
  uint8_t pumps_count;  // number of pumps in this control
  uint8_t current_pump; // number of the current pump which works or not
public:
  PumpsControl(); // constructor
  void addPump(Pump*); // add pump to array of pumps
  void switchCurrentPump(); // set the next pump as a current
  Pump* getCurrentPump(); // get the current pump
};
#endif

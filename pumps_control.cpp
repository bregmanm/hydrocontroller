#include "pumps_control.h"

PumpsControl::PumpsControl() {
  int i;
  for (i = 0; i < MAX_PUMPS_COUNT; i++) {
    pumps[i] = (Pump*)0;
  }
  pumps_count = 0; // no pumps on start
  current_pump = 0xff; // no current pumps
}

void PumpsControl::addPump(Pump* pump) {
  if (pumps_count < MAX_PUMPS_COUNT) {
    if (pumps_count == 0) {
      current_pump = 0; // make the first pump is current
    }
    pumps[pumps_count++] = pump;
  }
}

void PumpsControl::switchCurrentPump() {
  if (pumps_count > 1) { // when the current number of pumps is more than 1
    do {
      if (++current_pump == pumps_count) { // get the next pump
        current_pump = 0; // cyclic buffer of pumps
      }
    } while (pumps[current_pump]->isBroken()); // skip broken pumps
  }
}

Pump* PumpsControl::getCurrentPump() {
  return pumps[current_pump];
}

void PumpsControl::handleInterruptAnalogComp(uint8_t isRising) {
  if (isRising) { // rising - switch pump off
    pumps[current_pump]->switchOff();
    switchCurrentPump();
  } else { // falling - switch pump on
    pumps[current_pump]->switchOn();
  }
}



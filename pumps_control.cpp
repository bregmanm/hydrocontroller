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

h_mode_t PumpsControl::getMode() {
  return mode;
}

void PumpsControl::setMode(h_mode_t mode) {
  setup_common();
  this->mode = mode;
  switch (mode) {

    case MANUAL:
      setup_manual();
      break;

    case AUTO:
      setup_auto();
      break;
  }
}

automatic_state_t PumpsControl::getAutomaticState() {
  return automatic_state;
}

uint8_t PumpsControl::getCurrentPumpNumber() {
  return current_pump;
}

void PumpsControl::setup_common() {
  int i;
  setAdcsrb(0); // (Disable) ACME: Analog Comparator Multiplexer Enable
  for (i = 0; i < pumps_count; i++) { // switch off all pumps
    pumps[i]->switchOff();
  }
}

void PumpsControl::setup_manual() { // no additional setup
}

void PumpsControl::setup_auto() {
  // Set reference voltage at the middle of gate
  analogWrite(pin_analog_write_ref_voltage, low_threshold + ((high_threshold - low_threshold) >> 1));
  if (getAco()) { // read ACO of analog comparator
    setup_auto_falling(); // start pressure is high
  } else {
    setup_auto_rising(); // start pressure is low
  }
}

void  PumpsControl::setup_auto_rising() {
  automatic_state = H_RISING;
  analogWrite(pin_analog_write_ref_voltage, high_threshold);
  setRisingEdge();
  getCurrentPump()->switchOn();
}

void PumpsControl::setup_auto_falling() {
  automatic_state = H_FALLING;
  analogWrite(pin_analog_write_ref_voltage, low_threshold);
  setFallingEdge();
  getCurrentPump()->switchOff();
}

void PumpsControl::setLowThreshold(int low_threshold) {
  this->low_threshold = low_threshold;
}

void PumpsControl::setHighThreshold(int high_threshold) {
  this->high_threshold = high_threshold;
}

void PumpsControl::setPinAnalogWriteReferenceVoltage(uint8_t pin_analog_write_ref_voltage) {
  this->pin_analog_write_ref_voltage = pin_analog_write_ref_voltage;
  pinMode(pin_analog_write_ref_voltage, OUTPUT);   // sets the pin as output
}

void PumpsControl::switchOnCurrentPump() {
  if (mode == MANUAL) {
    getCurrentPump()->switchOn();
  }
}

void PumpsControl::switchOffCurrentPump() {
  if (mode == MANUAL) {
    getCurrentPump()->switchOff();
  }
}

void PumpsControl::handleInterruptAnalogComp() {
  if (mode != AUTO) { // handle interrupt in auto mode only
    return;
  }
  switch (automatic_state) {
    case H_RISING:
      setup_auto_falling();
      switchCurrentPump();
      break;
    case H_FALLING:
      setup_auto_rising();
      break;
  }
}



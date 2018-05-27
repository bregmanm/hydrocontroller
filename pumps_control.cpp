#include "pumps_control.h"

#ifdef TEST
#include <iostream>
#endif

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

void PumpsControl::setup_common() {
  int i;
#ifdef TEST
  std::cout << "ADCSRB = 0" << std::endl;
#else
  ADCSRB = 0; // (Disable) ACME: Analog Comparator Multiplexer Enable
#endif
  for (i = 0; i < pumps_count; i++) { // switch off all pumps
    pumps[i]->switchOff();
  }
}

void PumpsControl::setup_manual() { // no additional setup
}

void PumpsControl::setup_auto() {
  int startPressure = analogRead(pressure_analog_channel); // current pressure value
  if (startPressure > startPpressureThreshold) {
    setup_auto_falling(); // start pressure is high
  } else {
    setup_auto_rising(); // start pressure is low
  }
}

void  PumpsControl::setup_auto_rising() {
  automatic_state = RISING;
  analogWrite(pin_analog_write_ref_voltage, high_threshold);

#ifdef TEST
  std::cout << "Set ACSR to rising edge" << std::endl;
#else
  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
    | bit (ACIE)    // Analog Comparator Interrupt Enable
    | bit (ACIS1)
    | bit (ACIS0);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode 
#endif
  getCurrentPump()->switchOn();
}

void PumpsControl::setup_auto_falling() {
  automatic_state = FALLING;
  analogWrite(pin_analog_write_ref_voltage, low_threshold);
#ifdef TEST
  std::cout << "Set ACSR to rising edge" << std::endl;
#else
  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
    | bit (ACIE)    // Analog Comparator Interrupt Enable
    | bit (ACIS1);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode #endif
#endif
  getCurrentPump()->switchOff();
}

void PumpsControl::setLowThreshold(int low_threshold) {
  this->low_threshold = low_threshold;
}

void PumpsControl::setHighThreshold(int high_threshold) {
  this->high_threshold = high_threshold;
}

void PumpsControl::setStartPpressureThreshold(int startPpressureThreshold) {
  this->startPpressureThreshold = startPpressureThreshold;
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

void PumpsControl::setPressureAnalogChannel(uint8_t pressure_analog_channel) {
  this-> pressure_analog_channel = pressure_analog_channel;
}

void PumpsControl::handleInterruptAnalogComp() {
  if (mode != AUTO) { // handle interrupt in auto mode only
    return;
  }
  switch (automatic_state) {
    case RISING:
      setup_auto_falling();
      switchCurrentPump();
      break;
    case FALLING:
      setup_auto_rising();
      break;
  }
}



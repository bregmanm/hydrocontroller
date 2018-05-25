#include "pump.h"

Pump::Pump(uint8_t pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT); // pin for pump control is output
  broken = 0; // pump is not broken
  switchOff();
};

void Pump::switchOn() {
  if (!broken) { // do not switch on in the broken state!
    digitalWrite(pin, HIGH);
    state = HIGH;
  }
};

void Pump::switchOff() {
  digitalWrite(pin, LOW);
  state = LOW;
};

uint8_t Pump::getState() {
  return state;
}

void Pump::exchange() {
  state = (LOW == state) ? HIGH : LOW;
  digitalWrite(pin, state);
}

uint8_t Pump::isBroken() {
  return broken;
}

void Pump::setBroken(uint8_t isBroken) {
  broken = isBroken;
}
 

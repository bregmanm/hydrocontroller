#include "pump.h"

Pump::Pump(uint8_t pin) {
  this->pin = pin;
  pinMode(pin, OUTPUT); // pin for pump control is output
  switchOff();
};

void Pump::switchOn() {
  digitalWrite(pin, HIGH);
  state = HIGH;
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
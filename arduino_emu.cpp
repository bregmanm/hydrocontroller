#include <iostream>
#include "arduino_emu.h"

uint8_t ADCSRB;

void pinMode(uint8_t pin, uint8_t mode) {
  std::cout << "Pin " << (int)pin << " set to " << ((OUTPUT == mode) ? "OUTPUT" : "INPUT") << " mode" << std::endl;
}

void digitalWrite(uint8_t pin, uint8_t value) {
  std::cout << "Pin " << (int)pin << " set to " << ((HIGH == value) ? "HIGH" : "LOW") << " value" << std::endl;
}

int analogRead(uint8_t analogChannel) {
  std::cout << "Read from analog channel " << (int) analogChannel << ", test value is " << TEST_ANALOG_READ_VALUE << std::endl;
  return TEST_ANALOG_READ_VALUE;
}

void analogWrite(uint8_t analogChannel, int value) {
  std::cout << "Write to analog channel " << (int) analogChannel << ", value " << value << std::endl;
}
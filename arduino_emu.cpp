#include <iostream>
#include "arduino_emu.h"

void pinMode(uint8_t pin, uint8_t mode) {
  std::cout << "Pin " << (int)pin << " set to " << ((OUTPUT == mode) ? "OUTPUT" : "INPUT") << " mode" << std::endl;
}

void digitalWrite(uint8_t pin, uint8_t value) {
  std::cout << "Pin " << (int)pin << " set to " << ((HIGH == value) ? "HIGH" : "LOW") << " value" << std::endl;
}

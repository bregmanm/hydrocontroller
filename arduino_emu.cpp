#include <iostream>
#include "arduino_emu.h"
#include <bitset>

static uint8_t ACO_value;

void pinMode(uint8_t pin, uint8_t mode) {
  std::cout << "Pin " << (int)pin << " set to " << ((OUTPUT == mode) ? "OUTPUT" : "INPUT") << " mode" << std::endl;
}

void digitalWrite(uint8_t pin, uint8_t value) {
  std::cout << "Pin " << (int)pin << " set to " << ((HIGH == value) ? "HIGH" : "LOW") << " value" << std::endl;
}

void analogWrite(uint8_t analogChannel, int value) {
  std::cout << "Write to analog channel " << (int) analogChannel << ", value " << value << std::endl;
}

void setAdcsrb(uint8_t value) {
  std::cout << "Write to ADCSRB value " << (int) value << std::endl;
}

void setAco(uint8_t value) {
  std::bitset<8> aco(value);
  std::cout << "Set emulator ACO value " << aco << std::endl;
  ACO_value = value;
}

uint8_t getAco() {
  std::bitset<8> aco(ACO_value);
  std::cout << "Get emulator ACO value " << aco << std::endl;
  return ACO_value;
}

void setRisingEdge() {
    std::cout << "Set ACSR to rising edge" << std::endl;
}

void setFallingEdge() {
  std::cout << "Set ACSR to falling edge" << std::endl;
}

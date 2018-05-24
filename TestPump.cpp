#include <iostream>

#include "pump.h"

int main() {
  std::cout << "Init\n";
  Pump pump(5);
  std::cout << "Switch on\n";
  pump.switchOn();
  std::cout << "Switch ff\n";
  pump.switchOff();
  std::cout << "Exchange\n";
  pump.exchange();
  std::cout << "Next exchange\n";
  pump.exchange();
  return 0;
}
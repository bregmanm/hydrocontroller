#include <iostream>

#include "pumps_control.h"

int main() {
  std::cout << "Init: set up pump1 on pin 5 and pump2 on pin 6\n";
  Pump pump1(5), pump2(6);
  std::cout << "Init: set up pumps control\n";
  PumpsControl pumpsControl;
  std::cout << "Init: add pump1 and pump2 to pumps control\n";
  pumpsControl.addPump(& pump1);
  pumpsControl.addPump(& pump2);
  std::cout << "Switch on the current pump: must be on pin 5\n";
  pumpsControl.getCurrentPump()->switchOn();
  std::cout << "Switch off the current pump: must be on pin 5\n";
  pumpsControl.getCurrentPump()->switchOff();
  std::cout << "Switch to the next pump and switch on: must be on pin 6\n";
  pumpsControl.switchCurrentPump();
  pumpsControl.getCurrentPump()->switchOn();
  std::cout << "Switch off the current pump: must be on pin 6\n";
  pumpsControl.getCurrentPump()->switchOff();
  std::cout << "Switch to the next pump and switch on: must be on pin 5\n";
  pumpsControl.switchCurrentPump();
  pumpsControl.getCurrentPump()->switchOn();
  std::cout << "Switch off the current pump: must be on pin 5\n";
  pumpsControl.getCurrentPump()->switchOff();
  std::cout << "Interrupt from analog comparator: falling.\n";
  std::cout << "Pump1 must be switched on.\n";
  pumpsControl.handleInterruptAnalogComp(0);
  std::cout << "Interrupt from analog comparator: rising.\n";
  std::cout << "Pump1 must be switched off.\n";
  pumpsControl.handleInterruptAnalogComp(1);
  std::cout << "Interrupt from analog comparator: falling.\n";
  std::cout << "Pump2 must be switched on.\n";
  pumpsControl.handleInterruptAnalogComp(0);
  std::cout << "Interrupt from analog comparator: rising.\n";
  std::cout << "Pump2 must be switched off.\n";
  pumpsControl.handleInterruptAnalogComp(1);
  std::cout << "Interrupt from analog comparator: falling.\n";
  std::cout << "Pump1 must be switched on.\n";
  pumpsControl.handleInterruptAnalogComp(0);
  std::cout << "Interrupt from analog comparator: rising.\n";
  std::cout << "Pump1 must be switched off.\n";
  pumpsControl.handleInterruptAnalogComp(1);
  return 0;
}
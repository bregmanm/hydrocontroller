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
  return 0;
}
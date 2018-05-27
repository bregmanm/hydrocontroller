#include <iostream>

#include "pumps_control.h"

int main() {
  std::cout << "Init: set up pump1 on pin 0 and pump2 on pin 1\n";
  Pump pump1(0), pump2(1);
  std::cout << "Init: set up pumps control\n";
  PumpsControl pumpsControl;
  std::cout << "Init: add pump1 and pump2 to pumps control\n";
  pumpsControl.addPump(& pump1);
  pumpsControl.addPump(& pump2);
  std::cout << "Init: set manual mode\n";
  pumpsControl.setMode(MANUAL);

  std::cout << "Switch on the current pump: must be on pin 0\n";
  pumpsControl.switchOnCurrentPump();
  std::cout << "Switch off the current pump: must be on pin 0\n";
  pumpsControl.switchOffCurrentPump();
  std::cout << "Switch to the next pump and switch on: must be on pin 1\n";
  pumpsControl.switchCurrentPump();
  pumpsControl.switchOnCurrentPump();
  std::cout << "Switch off the current pump: must be on pin 1\n";
  pumpsControl.switchOffCurrentPump();
  std::cout << "Switch to the next pump and switch on: must be on pin 0\n";
  pumpsControl.switchCurrentPump();
  pumpsControl.switchOnCurrentPump();
  std::cout << "Switch off the current pump: must be on pin 0\n";
  pumpsControl.switchOffCurrentPump();
  return 0;
}
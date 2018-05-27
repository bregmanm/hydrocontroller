#include <iostream>
#include <assert.h>     /* assert */

#include "pumps_control.h"
#include "arduino_emu.h"

int main() {
  std::cout << "Init: set up pump1 on pin 0 and pump2 on pin 1\n";
  Pump pump1(0), pump2(1);
  std::cout << "Init: set up pumps control\n";
  PumpsControl pumpsControl;
  std::cout << "Init: add pump1 and pump2 to pumps control\n";
  pumpsControl.addPump(& pump1);
  pumpsControl.addPump(& pump2);
  std::cout << "Init: set pin for analog write reference voltage to 3\n";
  pumpsControl. setPinAnalogWriteReferenceVoltage(3);
  std::cout << "Init: set high and low thresholds\n";
  pumpsControl.setLowThreshold(20);
  pumpsControl.setHighThreshold(100);
  std::cout << "Init: set start pressure threshold more than reading value\n";
  pumpsControl.setStartPpressureThreshold(TEST_ANALOG_READ_VALUE + 1);
  std::cout << "Init: set pin number for analog comparator reference voltage - pin3\n";
  pumpsControl. setPinAnalogWriteReferenceVoltage(3);
  std::cout << "Init: set number of pressure reading analog channel - 0\n";
  pumpsControl. setPressureAnalogChannel(0);

  std::cout << "Init: set auto mode - pump1 must be started (pin 0)\n";
  pumpsControl.setMode(AUTO);
  assert(HIGH == pump1.getState());
  assert(LOW == pump2.getState());

  std::cout << "Handle analog comparator interrupt - pump1 must be stopped (pin 0)\n";
  pumpsControl.handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(LOW == pump2.getState());
  std::cout << "Handle analog comparator interrupt - pump2 must be started (pin 1)\n";
  pumpsControl.handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(HIGH == pump2.getState());
  std::cout << "Handle analog comparator interrupt - pump2 must be stopped (pin 1)\n";
  pumpsControl.handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(LOW == pump2.getState());

  std::cout << "Init: set start pressure threshold less than reading value\n";
  pumpsControl.setStartPpressureThreshold(TEST_ANALOG_READ_VALUE - 1);

  std::cout << "Init: set auto mode - pump1 must be not started (pin 0)\n";
  pumpsControl.setMode(AUTO);
  assert(LOW == pump1.getState());
  assert(LOW == pump2.getState());

  std::cout << "Handle analog comparator interrupt - pump1 must be started (pin 0)\n";
  pumpsControl. handleInterruptAnalogComp();
  assert(HIGH == pump1.getState());
  assert(LOW == pump2.getState());
  std::cout << "Handle analog comparator interrupt - pump1 must be stopped (pin 0)\n";
  pumpsControl. handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(LOW == pump2.getState());
  std::cout << "Handle analog comparator interrupt - pump2 must be started (pin 1)\n";
  pumpsControl. handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(HIGH == pump2.getState());
  std::cout << "Handle analog comparator interrupt - pump2 must be stopped (pin 1)\n";
  pumpsControl. handleInterruptAnalogComp();
  assert(LOW == pump1.getState());
  assert(LOW == pump2.getState());
 

  return 0;
}
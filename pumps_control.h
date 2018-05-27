#ifndef _PUMPS_CONTROL_H_
#define _PUMPS_CONTROL_H_
// Control of pumps group

#include "pump.h"

#define MAX_PUMPS_COUNT 2

enum h_mode_t { // mode of pump control
  MANUAL,
  AUTO
};

enum automatic_state_t {
	RISING,
	FALLING
};

class PumpsControl {
  Pump* pumps[MAX_PUMPS_COUNT]; // array of pumps
  uint8_t pumps_count;  // number of pumps in this control
  uint8_t current_pump; // number of the current pump which works or not
  h_mode_t mode; // current mode: automatic or manual
  uint8_t pin_analog_write_ref_voltage; // pin for analog write reference voltage
  automatic_state_t automatic_state; // current state in the automatic mode: rising or falling
  int low_threshold, high_threshold; // reference voltages for analog comparator thresholds
  uint8_t pressure_analog_channel; // analog channel for reading pressure
  int startPpressureThreshold; // Value for comparing with start pressure for setting RISING or FALLING auto mode

  void  setup_common(); //common setup for all modes
  void  setup_manual(); //start work manual mode
  void  setup_auto(); //start work auto mode
  void  setup_auto_rising(); //setup analog comparator for rising and switch pump on
  void  setup_auto_falling(); //setup analog comparator for falling

public:
  PumpsControl(); // constructor
  void addPump(Pump*); // add pump to array of pumps
  void switchCurrentPump(); // set the next pump as a current
  Pump* getCurrentPump(); // get the current pump
  void setMode(h_mode_t); // set current mode: auto or manual and start the appropriate setup. Must be called at the end of other settings
  void setPinAnalogWriteReferenceVoltage(uint8_t); // set number of pin for analog write reference voltage
  void setLowThreshold(int); // set low threshold value
  void setHighThreshold(int); // set high threshold value
  void setStartPpressureThreshold(int); // setting start pressure threshold
  void setPressureAnalogChannel(uint8_t); // set the number of pressure analog channel
  void handleInterruptAnalogComp(); // calls when interrupt from analog comparator is occured. Input value: 0 - interrupt on falling, else - on rising
  void switchOnCurrentPump(); // switch on current pump on manual mode
  void switchOffCurrentPump(); // switch off current pump on manual mode
};
#endif

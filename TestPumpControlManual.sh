#!/bin/bash

g++ -D TEST TestPumpControlManual.cpp arduino_emu.cpp pump.cpp pumps_control.cpp -o TestPumpControlManual

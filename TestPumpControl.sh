#!/bin/bash

g++ -D TEST TestPumpControl.cpp arduino_emu.cpp pump.cpp pumps_control.cpp -o TestPumpControl

#!/bin/bash

g++ -D TEST TestPumpControlAuto.cpp arduino_emu.cpp pump.cpp pumps_control.cpp -o TestPumpControlAuto

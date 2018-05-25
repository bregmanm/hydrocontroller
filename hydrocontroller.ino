/*
  Hydrocontroller implementation and remote serial monitor.

*/

#include "pumps_control.h"

#define BAUD_RATE 9600

#define PARAMS_REPEAT 1000

#define LOW_PRESSURE 200
#define HIGH_PRESSURE 700
#define INPUT_BUFFER_LENGTH 500
#define END_OF_UNIT '\r'
#define SERIAL_TIMEOUT 5000

// RS485 params
const uint8_t     PIN_direction_TX_RX = 10;
byte inputBuffer[INPUT_BUFFER_LENGTH];

void switchTX_HI() {
  digitalWrite(PIN_direction_TX_RX, HIGH); // transmit mode
  delay(1);
}

void switchTX_LOW() {
  digitalWrite(PIN_direction_TX_RX, LOW); // receive mode
  delay(1);
}

void sendChar(char c) {
  switchTX_HI();
  Serial.print(c);
  delay(1);
  switchTX_LOW();
}

void transmitStringHigh(String s) {
  for (int i = 0; i < s.length(); i++) {
    Serial.print(s.charAt(i));
    delay(1);
  }
}

void transmitEndOfUnit() {
  Serial.print(END_OF_UNIT);
  delay(1);
  switchTX_LOW();
}

void transmitString(String s) {
  switchTX_HI();
  transmitStringHigh(s);
  transmitEndOfUnit();
}

enum mode_t {
	manual,
	automatic
};

enum automatic_state_t {
	rising,
	falling
};

enum print_t {
	params,
	pressure
};

enum pump_mode_t {
	firstPump,
	secondPump,
	bothPumps
};

mode_t mode; // current mode of hydrocontroller
automatic_state_t automatic_state = rising;
pump_mode_t pump_mode = bothPumps;
pump_mode_t current_pump = secondPump;
byte *cmdPtr;

print_t mode_print;

bool is_print = false;
unsigned long print_time;

Pump pump1 = Pump(0);
Pump pump2 = Pump(1);
PumpsControl pumpsControl = PumpsControl();


void printHelp() {
	switchTX_HI();
	transmitStringHigh("Available commands:\n");
	transmitStringHigh("H or h - print help\n");
	transmitStringHigh("A or a - switch to the automatic mode\n");
	transmitStringHigh("M or m - switch to the manual mode\n");
	transmitStringHigh("S or s - print current status\n");
	transmitStringHigh("U or u - start print params each ");
	transmitStringHigh(String(PARAMS_REPEAT));
	transmitStringHigh(" ms\n");
	transmitStringHigh("P or p - start print pressure each ");
	transmitStringHigh(String(PARAMS_REPEAT));
	transmitStringHigh(" ms\n");
	transmitStringHigh("V or v - stop print params or pressure each ");
	transmitStringHigh(String(PARAMS_REPEAT));
	transmitStringHigh(" ms\n");
	transmitStringHigh("B or b - switch on/off the first pump in manual state\n");
	transmitStringHigh("C or C - switch on/off the second pump in manual state\n");
	transmitStringHigh("D or d - switch first-second-both pumps in automatic state\n");
	transmitEndOfUnit();
}

int readPressure() {
	return analogRead(A0);
}

void stop_pumps() {
	pump1.switchOff();
	pump2.switchOff();
}

void manage_pumps() {
	switch (automatic_state) {
		case rising:
			switch (pump_mode) {
				case firstPump:
					pump2.switchOff();
					pump1.switchOn();
					break;
				case secondPump:
					pump1.switchOff();
					pump2.switchOn();
					break;
				case bothPumps:
					current_pump = (current_pump == firstPump)?secondPump:firstPump;
					if (current_pump == firstPump) {
						pump1.switchOn();
					} else {
						pump2.switchOn();
					}
					break;
			}
			break;
		case falling:
			stop_pumps();
			break;
	}
}
			

void setup_automatic() {
	mode = automatic;
	stop_pumps();
	automatic_state = (readPressure() > HIGH_PRESSURE)?falling:rising;
	manage_pumps();
	transmitString("Automatic mode\n");
}

void setup_manual() {
	mode = manual;
	stop_pumps();
	transmitString("Manual mode\n");
}

void print_pressure() {
	transmitStringHigh("Pressure is ");
	transmitStringHigh(String(readPressure()));
	transmitStringHigh("\n");
}

void print_params() {
	print_pressure();
	transmitStringHigh("Pump1 is ");
	transmitStringHigh(pump1.getState()?"ON":"OFF");
	transmitStringHigh("\n");
	transmitStringHigh("Pump2 is ");
	transmitStringHigh(pump2.getState()?"ON":"OFF");
	transmitStringHigh("\n");
}

void print_status() {
	switchTX_HI();
	transmitStringHigh("Current mode is ");
	transmitStringHigh(mode == automatic?"automatic":"manual");
	transmitStringHigh("\n");

	if (mode == automatic) {
		transmitStringHigh((automatic_state == rising)?"rising":"falling");
		transmitStringHigh("\n");

		switch (pump_mode) {
			case firstPump:
				transmitStringHigh("firstPump\n");
				break;
			case secondPump:
				transmitStringHigh("secondPump\n");
				break;
			case bothPumps:
				transmitStringHigh("bothPumps\n");
				transmitStringHigh((current_pump == firstPump)?"firstPump":"secondPump");
				transmitStringHigh("\n");
				break;
		}
	}		
	print_params();
	transmitEndOfUnit();
}

void processCommand(String cmd) {
    if (cmd.length() == 0) { // empty command - ignore
		return;
	}
	char incomingByte = cmd.charAt(0);
	if (isAlpha(incomingByte)) {
		Serial.println(char(incomingByte));
		if (incomingByte >= 'a') {
			incomingByte -= 'a' - 'A';
		}
		switch (incomingByte) {
			case 'H':
				printHelp();
				break;
			case 'A': // Switch to automatic mode
				setup_automatic();
				break;
			case 'M': // Switch to manual mode
				setup_manual();
				break;
			case 'S': // get status
				print_status();
				break;
/*
				case 'U': // Start print params
				Serial.print("Start print params each ");
				Serial.print(PARAMS_REPEAT);
				Serial.println(" ms");
				mode_print = params;
				is_print = true;
				print_time = millis();
				break;
			case 'V': // Stoop print
				Serial.print("Stop print params or pressure each ");
				Serial.print(PARAMS_REPEAT);
				Serial.println(" ms");
				is_print = false;
				break;
			case 'P': // Start print pressure
				Serial.print("Start print pressure each ");
				Serial.print(PARAMS_REPEAT);
				Serial.println(" ms");
				mode_print = pressure;
				is_print = true;
				print_time = millis();
				break;
*/
				case 'B': // Switch on/off the first pump
				if (mode == manual) {
                                        pump1.exchange();
				}
				break;
			case 'C': // Switch on/off the second pump
                                        pump2.exchange();
				break;
			case 'D': // Switch first-second-both pumps in automatic mode
				if (mode == automatic) {
					switch (pump_mode) {
						case firstPump:
							pump_mode = secondPump;
							if (automatic_state == rising) {
								pump1.switchOff();
								pump2.switchOn();
							}
							Serial.println("secondPump");
							break;
						case secondPump:
							pump_mode = bothPumps;
							current_pump = firstPump;
							if (automatic_state == rising) {
								pump1.switchOn();
								pump2.switchOff();
							}
							Serial.println("bothPumps");
							break;
						case bothPumps:
							pump_mode = firstPump;
							if (automatic_state == rising) {
								pump1.switchOn();
								pump2.switchOff();
							}
							Serial.println("firstPump");
							break;
					}
				}
				break;
				
			default:
				transmitString("Wrong command\n");
				// printHelp();
		}
	}
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pumpsControl.addPump(&pump1);
  pumpsControl.addPump(&pump2);

  cmdPtr = inputBuffer;
  setup_automatic();
}

void serialEvent() {
	// read the incoming byte:
	byte incomingByte = Serial.read();
	if (!(incomingByte == END_OF_UNIT)) { // wait end of command
		*cmdPtr++ = incomingByte;
		if (cmdPtr - (byte*)inputBuffer >= INPUT_BUFFER_LENGTH - 1) {
			transmitString("Command is too long\n");
            cmdPtr = inputBuffer;
		}
		return;
	}
	// Command was received
	*cmdPtr = (byte)0;
	String cmd = String((char *)inputBuffer);
    cmdPtr = inputBuffer;
	processCommand(cmd);
}

// the loop routine runs over and over again forever:
void loop() {
	if (mode == automatic) {
		boolean state_changed = false;
		int curr_pressure = readPressure();
		switch (automatic_state) {
			case rising:
				if (curr_pressure > HIGH_PRESSURE) {
					automatic_state = falling;
					state_changed = true;
				}
				break;
			case falling:
				if (curr_pressure < LOW_PRESSURE) {
					automatic_state = rising;
					state_changed = true;
				}
				break;
		}
		if (state_changed) {
			manage_pumps();
		}
	}

	if (is_print) {
		unsigned long curr_time = millis();
		if (curr_time > print_time + PARAMS_REPEAT) { // print params
			print_time = curr_time;
			switchTX_HI();
			switch (mode_print) {
				case params:
					print_params();
					break;
				case pressure:
					print_pressure();
					break;
			}
			transmitEndOfUnit();
		}
	}
			
	/*	

  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1);        // delay in between reads for stability
  */
}

/*
  Hydrocontroller implementation and remote serial monitor.

*/

#define BAUD_RATE 9600

#define PARAMS_REPEAT 1000

#define LOW_PRESSURE 200
#define HIGH_PRESSURE 700

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

print_t mode_print;

int incomingByte = 0;	// for incoming serial data
bool is_print = false;
unsigned long print_time;

bool volatile pump1_mode = false;
bool volatile pump2_mode = false;

void printHelp() {
	Serial.println("Available commands:");
	Serial.println("H or h - print help");
	Serial.println("A or a - switch to the automatic mode");
	Serial.println("M or m - switch to the manual mode");
	Serial.println("S or s - print current status");
	Serial.print("U or u - start print params each ");
	Serial.print(PARAMS_REPEAT);
	Serial.println(" ms");
	Serial.print("P or p - start print pressure each ");
	Serial.print(PARAMS_REPEAT);
	Serial.println(" ms");
	Serial.print("V or v - stop print params or pressure each ");
	Serial.print(PARAMS_REPEAT);
	Serial.println(" ms");
	Serial.println("B or b - switch on/off the first pump in manual state");
	Serial.println("C or C - switch on/off the second pump in manual state");
	Serial.println("D or d - switch first-second-both pumps in automatic state");
}

int readPressure() {
	return analogRead(A0);
}

void stop_pumps() {
	pump1_mode = false;
	pump2_mode = false;
}

void manage_pumps() {
	switch (automatic_state) {
		case rising:
			switch (pump_mode) {
				case firstPump:
					pump2_mode = false;
					pump1_mode = true;
					break;
				case secondPump:
					pump1_mode = false;
					pump2_mode = true;
					break;
				case bothPumps:
					current_pump = (current_pump == firstPump)?secondPump:firstPump;
					if (current_pump == firstPump) {
						pump1_mode = true;
					} else {
						pump2_mode = true;
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
	Serial.println("Automatic mode");
}

void setup_manual() {
	mode = manual;
	stop_pumps();
	Serial.println("Manual mode");
}

void print_pressure() {
	Serial.print("Pressure is ");
	Serial.println(readPressure());
}

void print_params() {
	print_pressure();
	Serial.print("Pump1 is ");
	Serial.println(pump1_mode?"ON":"OFF");
	Serial.print("Pump2 is ");
	Serial.println(pump2_mode?"ON":"OFF");
}

void print_status() {
	Serial.print("Current mode is ");
	Serial.println(mode == automatic?"automatic":"manual");
	if (mode == automatic) {
		Serial.println((automatic_state == rising)?"rising":"falling");
		switch (pump_mode) {
			case firstPump:
				Serial.println("firstPump");
				break;
			case secondPump:
				Serial.println("secondPump");
				break;
			case bothPumps:
				Serial.println("bothPumps");
				Serial.println((current_pump == firstPump)?"firstPump":"secondPump");
				break;
		}
	}		
	print_params();
}

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
	setup_automatic();
}

void serialEvent() {
	// read the incoming byte:
	incomingByte = Serial.read();
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
			case 'B': // Switch on/off the first pump
				if (mode == manual) {
					pump1_mode = !pump1_mode;
				}
				break;
			case 'C': // Switch on/off the second pump
				if (mode == manual) {
					pump2_mode = !pump2_mode;
				}
				break;
			case 'D': // Switch first-second-both pumps in automatic mode
				if (mode == automatic) {
					switch (pump_mode) {
						case firstPump:
							pump_mode = secondPump;
							if (automatic_state == rising) {
								pump1_mode = false;
								pump2_mode = true;
							}
							Serial.println("secondPump");
							break;
						case secondPump:
							pump_mode = bothPumps;
							current_pump = firstPump;
							if (automatic_state == rising) {
								pump1_mode = true;
								pump2_mode = false;
							}
							Serial.println("bothPumps");
							break;
						case bothPumps:
							pump_mode = firstPump;
							if (automatic_state == rising) {
								pump1_mode = true;
								pump2_mode = false;
							}
							Serial.println("firstPump");
							break;
					}
				}
				break;
				
			default:
				Serial.println("Wrong command");
				printHelp();
		}
	}
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
			switch (mode_print) {
				case params:
					print_params();
					break;
				case pressure:
					print_pressure();
					break;
			}
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

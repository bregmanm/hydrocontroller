/*
  Hydrocontroller implementation and remote serial monitor.

*/

#define BAUD_RATE 9600

#define PARAMS_REPEAT 1000

enum mode_t {
	manual,
	automatic
};

mode_t mode; // current mode of hydrocontroller

int incomingByte = 0;	// for incoming serial data
bool is_print_params = false;
unsigned long print_param_time;

int volatile pressure = 0; // water pressure

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
	Serial.print("V or v - stop print params each ");
	Serial.print(PARAMS_REPEAT);
	Serial.println(" ms");
}

void setup_automatic() {
	mode = automatic;
	Serial.println("Automatic mode");
}

void setup_manual() {
	mode = manual;
	pump1_mode = false;
	pump2_mode = false;
	Serial.println("Manual mode");
}

void print_params() {
	Serial.print("Pressure is ");
	Serial.println(pressure);
	Serial.print("Pump1 is ");
	Serial.println(pump1_mode?"ON":"OFF");
	Serial.print("Pump2 is ");
	Serial.println(pump2_mode?"ON":"OFF");
}

void print_status() {
	Serial.print("Current mode is ");
	Serial.println(mode == automatic?"automatic":"manual");
	print_params();
}

// the setup routine runs once when you press reset:
void setup() {
	Serial.begin(BAUD_RATE);
	setup_automatic();
}

// the loop routine runs over and over again forever:
void loop() {
	if (Serial.available() > 0) {
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
					is_print_params = true;
					print_param_time = millis();
					break;
				case 'V': // Stoop print params
					Serial.print("Stop print params each ");
					Serial.print(PARAMS_REPEAT);
					Serial.println(" ms");
					is_print_params = false;
					break;
					
				default:
					Serial.println("Wrong command");
					printHelp();
			}
		}
	}
	if (is_print_params) {
		unsigned long curr_time = millis();
		if (curr_time > print_param_time + PARAMS_REPEAT) { // print params
			print_param_time = curr_time;
			print_params();
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

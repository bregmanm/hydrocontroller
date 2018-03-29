/*
  Hydrocontroller implementation and remote serial monitor.

*/

#define BAUD_RATE 9600

enum mode_t {
	manual,
	automatic
};

mode_t mode; // current mode of hydrocontroller

int incomingByte = 0;	// for incoming serial data

void printHelp() {
	Serial.println("Available commands:");
	Serial.println("H or h - print help");
	Serial.println("A or a - switch to the automatic mode");
	Serial.println("M or m - switch to the manual mode");
	Serial.println("S or s - print current status");
}

void setup_automatic() {
	mode = automatic;
	Serial.println("Automatic mode");
}

void setup_manual() {
	mode = manual;
	Serial.println("Manual mode");
}

void print_status() {
	Serial.print("Current mode is ");
	Serial.println(mode == automatic?"automatic":"manual");
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
				default:
					Serial.println("Wrong command");
					printHelp();
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

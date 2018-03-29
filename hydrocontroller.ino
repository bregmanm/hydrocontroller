/*
  Hydrocontroller implementation and remote serial monitor.

*/

#define BAUD_RATE 9600

int incomingByte = 0;	// for incoming serial data

void printHelp() {
	Serial.println("Available commands:");
	Serial.println("H or h - print help");
}

// the setup routine runs once when you press reset:
void setup() {
	Serial.begin(BAUD_RATE);
}

// the loop routine runs over and over again forever:
void loop() {
	if (Serial.available() > 0) {
		// read the incoming byte:
		incomingByte = Serial.read();
		if (isAlpha(incomingByte)) {
			Serial.println(char(incomingByte));
			switch (incomingByte) {
				case 'H':
				case 'h':
					printHelp();
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

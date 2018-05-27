/*
  Hydrocontroller implementation and remote serial monitor.

*/

#include "pumps_control.h"

#define BAUD_RATE 9600

#define PARAMS_REPEAT 1000

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

enum print_t {
	params,
	pressure
};

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
	transmitStringHigh(pumpsControl.getMode() == AUTO?"automatic":"manual");
	transmitStringHigh("\n");

	if (pumpsControl.getMode() == AUTO) {
		transmitStringHigh((pumpsControl.getAutomaticState() == H_RISING)?"rising":"falling");
		transmitStringHigh("\n");
		transmitStringHigh((pumpsControl.getCurrentPumpNumber() == 0)?"first pump":"second pump");
		transmitStringHigh("\n");

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
				pumpsControl.setMode(AUTO);
				break;
			case 'M': // Switch to manual mode
				pumpsControl.setMode(MANUAL);
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
				
			default:
				transmitString("Wrong command\n");
				// printHelp();
		}
	}
}

ISR (ANALOG_COMP_vect)
  {
    pumpsControl.handleInterruptAnalogComp();
  }


// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pumpsControl.addPump(&pump1);
  pumpsControl.addPump(&pump2);
  pumpsControl. setPinAnalogWriteReferenceVoltage(3);
  //TEMP - values for light sensor!!!
  pumpsControl.setLowThreshold(81);
  pumpsControl.setHighThreshold(201);
  pumpsControl.setStartPpressureThreshold(400);

  pumpsControl. setPinAnalogWriteReferenceVoltage(3);
  pumpsControl. setPressureAnalogChannel(0);

  pumpsControl.setMode(AUTO);


  cmdPtr = inputBuffer;
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

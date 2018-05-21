#define BAUD_RATE 9600
#define INPUT_BUFFER_LENGTH 200
#define END_OF_UNIT '\r'
#define SERIAL_TIMEOUT 5000

// RS485 params
const uint8_t     PIN_direction_TX_RX = 10;
byte inputBuffer[INPUT_BUFFER_LENGTH];
const String request = "request";
const String response = "response";

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

void transmitString(String s) {
  switchTX_HI();
  for (int i = 0; i < s.length(); i++) {
    Serial.print(s.charAt(i));
    delay(1);
  }
  Serial.print(END_OF_UNIT);
  delay(1);
  switchTX_LOW();
}

void blink(unsigned long interval, int repeat) {
	delay(500);
	for (int i = 0; i < repeat; i++) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(interval);
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
	}
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.setTimeout(SERIAL_TIMEOUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // Set RS485 to receive mode
  pinMode(PIN_direction_TX_RX,   OUTPUT); // D10 is output pin
  digitalWrite(PIN_direction_TX_RX, LOW); // Receive mode
}

void loop() {
	transmitString(request);
	delay(10000);                       // wait for a second
}

void serialEvent() {
	blink(1000, 1);
	// read the incoming bytes:
	size_t bytesReceived = Serial.readBytesUntil(END_OF_UNIT, inputBuffer, INPUT_BUFFER_LENGTH);
	if (bytesReceived == 0) {
		blink(500, 2);
	} else if (bytesReceived != response.length() + 1) {
		blink(500, 3);
	} else {
		inputBuffer[bytesReceived - 1] = byte(0);
		String inputStr = String((char *)inputBuffer);
		if (!inputStr.equals(response)) {
			blink(500, 4);
		} else {
			blink(2000, 1);
		}
	}
}

#define BAUD_RATE 9600
#define INPUT_BUFFER_LENGTH 200
#define END_OF_UNIT '\n'

// RS485 params
const uint8_t     PIN_direction_TX_RX = 10;
char inputBuffer[INPUT_BUFFER_LENGTH];

void switchTX_HI() {
  digitalWrite(PIN_direction_TX_RX, HIGH); // transmit mode
  delay(1);
}

void switchTX_LOW() {
  digitalWrite(PIN_direction_TX_RX, LOW); // receive mode
  delay(1);
}

void transmitString(String s) {
  switchTX_HI();
  for (int i = 0; i < s.length(); i++) {
    Serial.print(s.charAt(i));
    delay(1);
  }
  switchTX_LOW();
}

int analogWritePin = 3;      // output pin which generates analog voltage
int currOutputValue = 0;
int incomingByte = 0;	// for incoming serial data
volatile boolean triggered;

ISR (ANALOG_COMP_vect)
  {
  triggered = true;
  }

void setup()
{
  Serial.begin(BAUD_RATE);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(analogWritePin, OUTPUT);   // sets the pin as output
  analogWrite(analogWritePin, currOutputValue);
  ADCSRB = 0;           // (Disable) ACME: Analog Comparator Multiplexer Enable
}

void loop()
{
  if (triggered)
    {
    Serial.println ("Triggered!"); 
    triggered = false;
    }
}

void serialEvent() {
	size_t bytesReceived = Serial.readBytesUntil(END_OF_UNIT, inputBuffer, INPUT_BUFFER_LENGTH);
	if (!bytesReceived) { // error input
	  return;
	}
	// read the incoming byte:
	incomingByte = inputBuffer[0];
	switch (incomingByte) {
	  case '+':
	    currOutputValue += 20;
	    if (currOutputValue > 255) {
		currOutputValue = 255;
	    }
	    transmitString(String(currOutputValue));
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	  case '<':
	    currOutputValue ++;
	    if (currOutputValue > 255) {
		currOutputValue = 255;
	    }
	    transmitString(String(currOutputValue));
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	  case '-':
	    currOutputValue -= 20;
	    if (currOutputValue < 0) {
		currOutputValue = 0;
	    }
	    transmitString(String(currOutputValue));
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	  case '>':
	    currOutputValue --;
	    if (currOutputValue < 0) {
		currOutputValue = 0;
	    }
	    transmitString(String(currOutputValue));
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	  case 'f':
	  case 'F': // falling edge
		  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
        		| bit (ACIE)    // Analog Comparator Interrupt Enable
		        | bit (ACIS1);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on falling edge)
	    break;
	  case 'r':
	  case 'R': // rising edge
		  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
        		| bit (ACIE)    // Analog Comparator Interrupt Enable
		        | bit (ACIS1)
			| bit (ACIS0);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on rising edge)
	    break;
	  case 'p':
	  case 'P': // print comparator's status
                transmitString("Comparator: " + String((ACSR & bit(ACO)) >> 5));
	    break;
	}
}

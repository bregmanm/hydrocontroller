#define BAUD_RATE 9600

int analogWritePin = 3;      // output pin which generates analog voltage
int currOutputValue = 0;
int incomingByte = 0;	// for incoming serial data

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(analogWritePin, OUTPUT);   // sets the pin as output
  analogWrite(analogWritePin, currOutputValue);
}

void loop()
{
}

void serialEvent() {
	// read the incoming byte:
	incomingByte = Serial.read();
	switch (incomingByte) {
	  case '+':
	    currOutputValue += 20;
	    if (currOutputValue > 255) {
		currOutputValue = 255;
	    }
	    Serial.println(currOutputValue);
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	  case '-':
	    currOutputValue -= 20;
	    if (currOutputValue < 0) {
		currOutputValue = 0;
	    }
	    Serial.println(currOutputValue);
	    analogWrite(analogWritePin, currOutputValue);
	    break;
	}
}

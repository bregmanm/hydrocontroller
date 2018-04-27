#define BAUD_RATE 9600

int analogWritePin = 3;      // output pin which generates analog voltage
int currOutputValue = 0;
volatile boolean triggered;

ISR (ANALOG_COMP_vect)
  {
  triggered = true;
  }

void setup()
{
  Serial.begin(BAUD_RATE);
  pinMode(analogWritePin, OUTPUT);   // sets the pin as output
  analogWrite(analogWritePin, currOutputValue);
  ADCSRB = 0;           // (Disable) ACME: Analog Comparator Multiplexer Enable
  ACSR =  bit (ACI)     // (Clear) Analog Comparator Interrupt Flag
        | bit (ACIE)    // Analog Comparator Interrupt Enable
        | bit (ACIS1);  // ACIS1, ACIS0: Analog Comparator Interrupt Mode Select (trigger on falling edge)

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

int analogWritePin = 3;      // output pin which generates analog voltage

void setup()
{
  pinMode(analogWritePin, OUTPUT);   // sets the pin as output
}

void loop()
{
  analogWrite(analogWritePin, val / 4);  // analogRead values go from 0 to 1023, analogWrite values from 0 to 255
}
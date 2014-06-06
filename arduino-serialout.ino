/*
  Shift Register lighting control
  based on Tom Igoe's shift register example

 */

//Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 8;
//Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 12;
////Pin connected to Data in (DS) of 74HC595
const int dataPin = 11;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("reset");
}

void loop() {
  long i = 0;
  while( 1 )
  {
    digitalWrite(latchPin, LOW);
//    shiftOut(dataPin, clockPin, MSBFIRST, i >> 16);
    shiftOut(dataPin, clockPin, MSBFIRST, i >> 8);
    shiftOut(dataPin, clockPin, MSBFIRST, i);
    digitalWrite(latchPin, HIGH);
    ++i;
  }
}

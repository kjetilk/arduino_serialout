/*
  Shift Register lighting control
  based on Tom Igoe's shift register example

 */

// Inputs and outputs numbered differently?
int inputs[8]  = { 22, 23, 24, 25, 26, 27, 28, 29 };

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
  
  for( int i = 0; i < 8; ++i )
  {
    pinMode( inputs[i], INPUT);
    digitalWrite( inputs[i], HIGH);
  }
  
  Serial.begin(9600);
  Serial.println("reset");
}

int state = 0xFF; // Bitshifted last-state

void toggle( int input, int* outs, int outputc )
{
  if( digitalRead( inputs[ input ] ) == 0 )
  {
    if( ( state & ( 0x01 << input ) ) != 0 )
    {
      Serial.println( "Trykket" );
//      Serial.println( digitalRead( relay0 ) );
      for( int i = 0; i < outputc; ++i )
      {
        int output = outputs[ outs[ i ] ];
        digitalWrite( output, !digitalRead( output ) );

        Serial.println( state, HEX );
        state ^= 0x01 << input;
        Serial.println( state, HEX );
      }
      delay( 100 );
    }
  }
  else if( ( state & ( 0x01 << input ) ) == 0 )
  {
    Serial.println( "Sluppet" );
    state ^= 0x01 << input;
    Serial.println( state, HEX );
    delay( 100 );
  }
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

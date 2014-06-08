// Inputs and outputs numbered differently?
int inputs[]  = { 22, // Inngang 1:
                  23, // Inngang 2:  
                  24, // Inngang 3:
                  25, // Inngang 4:
                  26, // Inngang 5:  
                  27, // Inngang 6:  
                  28, // Inngang 7:  
                  29, // Inngang 8:  
                  30, // Inngang 9:  
                  31, // Inngang 10: 
                  32, // Inngang 11: 
                  33, // Inngang 12: 
                  34, // Inngang 13: 
                  35, // Inngang 14: 
                  36, // Inngang 15: 
                  37, // Inngang 16: 
                  38, // Inngang 17: 
                  39, // Inngang 18: 
                  40, // Inngang 19: 
                  41, // Inngang 20: 
                  42, // Inngang 21: 
                  43, // Inngang 22: 
                  44, // Inngang 23: 
                  45, // Inngang 24: 
 };

int inputCounter[ sizeof( inputs ) ]; // Counts how many rotations/checks the button has been pressed.

uint32_t outputs = 0;

//Pin connected to latch pin 
const int latchPin = 8;
//Pin connected to clock pin 
const int clockPin = 9;
////Pin connected to Data in 
const int dataPin = 10;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(clockPin, OUTPUT);
  
  for( unsigned int i = 0; i < sizeof( inputs ); ++i )
  {
    pinMode( inputs[i], INPUT);      // Set as input
    digitalWrite( inputs[i], HIGH);  // Enable pull-up resistor
    inputCounter[ i ] = 0;
  }
  
  Serial.begin(9600);
  Serial.println("reset");
 }

void readInputs() // Reads trough all inputs, sets output if relevant
{
  for( unsigned int i = 0; i < sizeof( inputs ); ++i )
  {
    if( digitalRead( inputs[ i ] ) == 0 ) // Button pressed
    {
      ++inputCounter[ i ];
      if( inputCounter[ i ] == 0x7fff )
      {
        --inputCounter[ i ];
      }
      if( inputCounter[ i ] == 0x10 ) // 10 rotations of about 5ms(?)
      {
        outputs ^= (uint32_t)1 << i; // Toggle
      }
    }
  }
}

void setOutputs() // Pushes out all the outputs
{
  digitalWrite( latchPin, LOW );
  shiftOut( dataPin, clockPin, MSBFIRST, outputs >> 16 );
  shiftOut( dataPin, clockPin, MSBFIRST, outputs >> 8 );
  shiftOut( dataPin, clockPin, MSBFIRST, outputs );
  digitalWrite( latchPin, HIGH );
}

void loop()
{
  // Just read and write for now.
  readInputs();
  setOutputs();
}

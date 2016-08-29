// Inputs and outputs numbered differently?
byte inputs[]  = { 
  22, // 50, Due to conflict with Ethernet
  23, // 51,
  24, // 52,
  25, // 53,
  26,
  27, 
  28, 
  29,
  30,
  31,
  32,
  33,
  34,
  35,
  36, // Pins 37-44 unused since they are connected with dimmers
  45, 
  46,
  47,
  48,
  49
};

int inputCounter[ sizeof( inputs ) ]; // Counts how many rotations/checks the button has been pressed.

uint32_t outputs = 0;

/* ** Pinouts on TPIC6A596 Board **
Pin count starting opposite terminal
0 - Clock (Pin 10 on chip, 8 on Arduino)
1 - Latch (Pin 9 RCK on chip, 9 on Arduino)
2 - Data  (Pin 22 on chip, 10 on Arduino)
3 - Ground
4 - Power (Pin 21 on chip, 5V from Arduino)
*/


//Pin connected to clock pin 
const int clockPin = 8; 
//Pin connected to latch pin 
const int latchPin = 9; 
////Pin connected to Data pin 
const int dataPin = 10;
//Pin outputting night status
const int nightOutPin = 11;
//Pin outputting away status
const int awayOutPin = 12;
/////Pin connected status
const int statusPin = 13;
//Pin inputting night status
const int nightPin = 37;
//Pin inputting away status
const int awayPin = 38;
//Pin for red LED
const int redPin = 39;
//Pin for green LED
const int greenPin = 40;

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(redPin, OUTPUT);
  digitalWrite( redPin, HIGH ); // Just to blink it
  pinMode(greenPin, OUTPUT);
  pinMode(dataPin, OUTPUT);  
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(nightOutPin, OUTPUT);  
  pinMode(awayOutPin, OUTPUT);
  pinMode(statusPin, OUTPUT);

  for( unsigned int i = 0; i < sizeof( inputs ); ++i )
  {
    pinMode( inputs[i], INPUT);      // Set as input
    digitalWrite( inputs[i], HIGH);  // Enable pull-up resistor
    inputCounter[ i ] = 0;
  }

  digitalWrite( statusPin, LOW );
  digitalWrite( nightOutPin, LOW );
  digitalWrite( awayOutPin, LOW );
  digitalWrite( greenPin, LOW );
  digitalWrite( redPin, LOW );

  Serial.begin(9600);
  Serial.println("reset");
}

void readInputs() // Reads trough all inputs, sets output if relevant
{
  for( unsigned int i = 0; i < sizeof( inputs ); ++i )
  {
    if( digitalRead( inputs[ i ] ) == 0 ) // Button pressed
    {
      digitalWrite( statusPin, HIGH );  
      ++inputCounter[ i ];
      if( inputCounter[ i ] == 0x7fff )
      {
        --inputCounter[ i ];
      }
      if( inputCounter[ i ] == 0x10 ) // 16 rotations of about 5ms(?)
      {
        outputs ^= (uint32_t)1 << i; // Toggle
      }
    } else {
      inputCounter[ i ] = 0;
      digitalWrite( statusPin, LOW ); // So, it'll be dimmed by number of switches
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


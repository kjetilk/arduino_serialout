// Inputs and outputs numbered differently?
byte inputs[]  = { 
  26, // Lager
  27, // Teknisk
  28, // Garderobe
  29, // Vindfang
  30, // Gang kjeller
  31, // Gjesterom
  32, // Kontor
  33, // Matbod
  34, // Disp
  35, // Speillys bad 1 etg
  36, // Utelys
  45, // LED dusj 2. etg (unused)
  46, // Marius 
  47, // Speillys bad 2 etg
  48, // Trappelys
  49, // Eivind
  22, // Synne, 50, Due to conflict with Ethernet
  23, // Vaskerom 51, Due to conflict with Ethernet
  25, // Bordlys 52, Due to conflict with Ethernet
  24, // Gang 53, Due to conflict with Ethernet
  37  // Overskap
};

int inputCounter[ sizeof( inputs ) ]; // Counts how many rotations/checks the button has been pressed.

uint32_t outputs = 0;
boolean isaway = 0;

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
const int nightPin = 39;
//Pin inputting away status
const int awayPin = 41;
//Pin for red LED
const int redPin = 38;
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
    pinMode( inputs[i], INPUT_PULLUP);  // Set as input and enable pull-up resistor
    inputCounter[ i ] = 0;
  }

  pinMode( nightPin, INPUT_PULLUP);  // Set as night input and enable pull-up resistor
  pinMode( awayPin, INPUT_PULLUP);  // Set as away input and enable pull-up resistor

  digitalWrite( statusPin, LOW );
  digitalWrite( greenPin, LOW );
  digitalWrite( redPin, LOW );

  // To communicate status to other Arduinos
  digitalWrite( nightOutPin, HIGH );
  digitalWrite( awayOutPin, HIGH );


  Serial.begin(9600);
  Serial.println("reset");
}

void setAway() {
  isaway = 1;
  outputs = 0x1fffff;
  digitalWrite( awayOutPin, LOW );
  digitalWrite( greenPin, LOW );
}

void setHome() {
  isaway = 0;
  digitalWrite( awayOutPin, HIGH );
  digitalWrite( greenPin, HIGH );
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
      if (isaway) { // Then, we've just gotten home
        Serial.println(F("Lights on => at home"));
        setHome();
      }
      Serial.println(outputs);
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
  if( digitalRead( nightPin ) == 0 ) { // Then, the night switch has been pressed
    Serial.println(F("Turn off everything at night."));
    outputs = 0x1fffff;
    digitalWrite( nightOutPin, LOW );
    delay(100);
    digitalWrite( nightOutPin, HIGH );
  } else if( digitalRead( awayPin ) == 0 ) { // Then, the away switch has been pressed
    if (isaway) { // Then, we've just gotten home
      Serial.println(F("Setting us at home"));
      setHome();
      delay(1000);
    } else { // We're just about to leave
      Serial.println(F("Setting us away"));
      setAway();
      delay(1000);
    }
  } else {
    readInputs();
  }
  setOutputs();
}


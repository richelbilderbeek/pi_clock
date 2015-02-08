// Pi Clock
// (C) 2015 Richel Bilderbeek
// #  0  1  2  3  4  5  6  7  8
// s  1  2  4  8 16 32
// m           1  2  4  8 16 32
// h  8 16              1  2  4
//
//
/*


Omschrijving chip SN74HC595:

 16 15 14 13 12 11 10 9
 |  |  |  |  |  |  |  |
 +--+--+--+--+--+--+--+
 |>                   |
 +--+--+--+--+--+--+--+
 |  |  |  |  |  |  |  |
 1  2  3  4  5  6  7  8

 Officiele pin namen:

 1: Q2
 2: Q3
 3: Q4
 3: Q5
 5: Q6
 6: Q7
 7: Q8 
 8: GND
 9: QH*: om te daisy chainen, anders ongebruikt
 10: SRCLR* (naar 5V)
 11: SRCLK (naar D4): clock pin
 12: RCLK (naar D3): latch pin
 13: OE* (naar GND)
 14: SER (naar D2): data pin
 15: Q1
 16: VCC (naar 5V)


Eerste shift register:

 16 15 14 13 12 11 10 9
 |  |  |  |  |  |  |  |
 +--+--+--+--+--+--+--+
 |>                   |
 +--+--+--+--+--+--+--+
 |  |  |  |  |  |  |  |
 1  2  3  4  5  6  7  8

 Verbindingen:

 1: LED 2
 2: LED 3
 3: LED 4
 3: LED 5
 5: LED 6
 6: LED 7
 7: LED 8 
 8: GND
 9: naar pin 14 van het tweede shift register (dit is de daisy chain verbinding)
 10: naar 5V
 11: naar D4: clock pin
 12: naar D3: latch pin
 13: naar GND
 14: naar D2: data pin
 15: naar LED1
 16: naar 5V

Tweede shift register:

 16 15 14 13 12 11 10 9
 |  |  |  |  |  |  |  |
 +--+--+--+--+--+--+--+
 |>                   |
 +--+--+--+--+--+--+--+
 |  |  |  |  |  |  |  |
 1  2  3  4  5  6  7  8

 Verbindingen:

 1: LED 10
 2: LED 11
 3: LED 12
 3: LED 13
 5: LED 14
 6: LED 15
 7: LED 816
 8: GND
 9: wordt niet gebruikt
 10: naar 5V
 11: naar D4: clock pin
 12: naar D3: latch pin
 13: naar GND
 14: naar pin 9 van eerste shift register (dit is de daisy chain verbinding)
 15: naar LED 9
 16: naar 5V

*/

#include <LongTimer.h>

//If NDEBUG is #defined, it is a release version
//If NDEBUG is commented out, it is a debug version
#define NDEBUG

const int datapin  = 2;
const int latchpin = 3;
const int clockpin = 4;
const int pin_16_hours = 5; //The pin connected to the LED to show 16 hours

void setup() 
{
  pinMode(latchpin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(datapin , OUTPUT);
  pinMode(pin_16_hours , OUTPUT);
  Serial.begin(9600);
  #ifndef NDEBUG
  Serial.println("PiClock v. 1.0 (debug version)");
  #else //NDEBUG
  Serial.println("PiClock v. 1.0 (release version)");
  #endif //NDEBUG
  Serial.println("LongTimer v. " + LongTimer::GetVersion());
  ShowBinary(0);
}

void loop() 
{
  LongTimer t;
  while (1)
  {
    ShowTime(
      static_cast<int>(t.GetSecs() % 60),
      static_cast<int>(t.GetMins() % 60),
      static_cast<int>(t.GetHours() % 24)
    );
    delay(1000);
    /*  
    for (int i=0; i!=256; ++i)
    {
      Serial.println(IntToBinary(i)); 
      delay(1000);
    }
    */
    /*
    for (int i=0; i!=16; ++i)
    {
      ShowBinary(1 << i);
      delay(1000);
    }
    */
    /*
    ShowTime(8,8,8);
    delay(1000);
    */
    /*
    for (int i=0; i!=5; ++i)
    {
      ShowTime(1 << i,1 << i,1 << i);
      delay(1000);
    }
    */
  }
}

void ShowTime(const int secs, const int mins, const int hours)
{
  digitalWrite(pin_16_hours,hours >= 16 ? HIGH : LOW);
  long x = 0; //The number to show in binary, must be long
  for (int i=0; i!=4; ++i)
  {
    if ((1 << (4 - 1 - i)) & hours) { ++x; }
    x <<= 1;
    //if (x < 0) Serial.println("ERROR");    
  }
  for (int i=0; i!=6; ++i)
  {
    if ((1 << (6 - 1 - i)) & mins) { ++x; }
    x <<= 1;   
    //if (x < 0) Serial.println("ERROR");    
  }
  for (int i=0; i!=6; ++i)
  {
    if ((1 << (6 - 1 - i)) & secs) { ++x; }
    if (i < 5) { x <<= 1; }   
    //if (x < 0) Serial.println("ERROR");    
  }
  //if (x < 0) Serial.println("ERROR");    
  
  #ifndef NDEBUG
  Serial.println(IntToBinary(x));
  #endif //NDEBUG
  ShowBinary(x % (256 * 256));
}

String IntToBinary(const long x) //Must be long 
{
  if (x < 0) { Serial.println("IntToBinary error: x must be positive"); }    
  String s;
  for (int i=0; i!=16; ++i)
  {
    long one = 1;
    s += ((one << (16 - 1 - i)) & x) ? "1" : "0";  
  }
  return s;
}


void ShowBinary(const long value) //Must be long
{
  if (value < 0) { Serial.println("ShowBinary error: value must be positive"); }
  //Start met schrijven
  digitalWrite(latchpin,LOW);

  //Bereken wat te schrijven
  // (dit moet in twee keer: een keer per shift register)
  const long high_value = value / 256;
  const long low_value  = value % 256;

  #ifndef NDEBUG
  Serial.print(IntToBinary(high_value));
  Serial.print(" ");
  Serial.println(IntToBinary(low_value));
  #endif // NDEBUG

  //Schrijf naar de shift registers
  //(opmerking: mocht je toch maar een shift register aansluiten,
  // dan wordt de high_value gewoon overschreven door de low_value,
  // oftewel: je ziet dan enkel de low_value)
  WriteToShiftRegister(high_value); 
  WriteToShiftRegister(low_value);

  //Klaar met schrijven
  digitalWrite(latchpin,HIGH);
}

void WriteToShiftRegister(const byte value) 
{
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //prepare shift register for bit shifting
  digitalWrite(datapin, LOW);
  digitalWrite(clockpin, LOW);

  for (int i=7; i>=0; i--)  
  {
    digitalWrite(clockpin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(datapin, value & (1<<i) ? HIGH : LOW);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(clockpin, HIGH);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(datapin, LOW);
  }

  //stop shifting
  digitalWrite(clockpin, LOW);
}

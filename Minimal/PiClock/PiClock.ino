/*

  Minimal Pi Clock
  (C) 2015 Richel Bilderbeek

2015-07-05: v.1.0: Initial version

Piezo:
  
    6              GND
    |  +--------+  |
    +--+   P    +--+
       +--------+

  6: piezo pin
  P: Piezo
  GND: Arduino GND pin

*/

#include <Time.h>

//If NDEBUG is #defined, it is a release version
//If NDEBUG is commented out, it is a debug version
//#define NDEBUG

const int pin_piezo = 4; //The pin connected to the piezo
const int pin_error = 13; //The pin connected to the error LED

void OnError(const String& error_message)
{
  Serial.print("ERROR: ");  
  Serial.println(error_message);  
  while (1)
  {
    //Blink LED
    digitalWrite(pin_error,!digitalRead(pin_error));
    //Write to serial
    delay(1000);
  }
}

void TestTime()
{
  //  setTime(hours, minutes, seconds, days, months, years);
  const int hours = 11;
  const int minutes = 22;
  const int seconds = 33;
  const int days = 0;
  const int months = 0;
  const int years = 0;
  setTime(hours, minutes, seconds, days, months, years);
  if (hour() != hours) { OnError("TestTime #1"); }
  if (minute() != minutes) { OnError("TestTime #1"); }
  if (second() != seconds) { OnError("TestTime #1"); }
  setTime(0,0,0,0,0,0);
}

void setup() 
{
  pinMode(pin_piezo, OUTPUT);
  pinMode(pin_error, OUTPUT);
  Serial.begin(9600); //Cannot be used: chip is used stand-alone
  #ifndef NDEBUG
  Serial.println("PiClock v. 1.0 (debug version)");
  #else //NDEBUG
  Serial.println("PiClock v. 1.0 (release version)");
  #endif //NDEBUG
  TestTime();
}

void loop() 
{
  // Is it Pi o'clock yet? That is, 3:14 PM, also known as 15:14
  bool is_pi_oclock = false;
  int last_sec = -1; //The previous second, used to detect a change in time, to be sent to serial monitor
  
  while (1)
  {
    //Show the time
    const int s = second();
    const int m =  minute();
    const int h = hour();

    if (last_sec == s) 
    {
      continue;
    }

    last_sec = s;

    //Detect pi o'clock
    if (h == 15 && m == 14) 
    {
      //Already beeped?
      if (!is_pi_oclock)
      {
        is_pi_oclock = true;
        const int frequency_hz = 3142;
        const int duration_msec = 3142;
        tone(pin_piezo,frequency_hz,duration_msec);
      }
    }
    else 
    { 
      is_pi_oclock = false; 
    }
  }
}

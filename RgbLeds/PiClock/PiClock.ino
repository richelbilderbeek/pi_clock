/*

  Pi Clock with RGB LEDs
  (C) 2015 Richel Bilderbeek

2015-02-28: v.1.0: Initial version
2015-03-01: v.1.1: Bugfix: setting the time multiple times works also after the first time
2015-03-12: v.1.2: Can also set time from Serial Monitor
2015-03-22: v.1.3: Use of Time library
2015-04-01: v.1.4: Removed reset pin, improved comments, support 4x AAA batteries as power source

Clock layout:

 #  0  1  2  3  4  5  6  7  8
 s  1  2  4  8 16 32
 m           1  2  4  8 16 32
 h  8 16              1  2  4


Arduino connections:
   2: to first shift register pin 14: data pin (the other shift register gets its data from daisy chaining)
   3: to both shift registers pin 12: latch pin
   4: to both shift registers pin 11: clock pin
   5: RGB LED 2, blue, displays 16 hours
   8: Left capacitive sensor, sensor pin
   9: Left capacitive sensor, helper pin
  10: Right capacitive sensor, sensor pin
  11: Right capacitive sensor, helper pin

Description shift register chip SN74HC595:

  16 15 14 13 12 11 10 9
  |  |  |  |  |  |  |  |
  +--+--+--+--+--+--+--+
  |>                   |
  +--+--+--+--+--+--+--+
  |  |  |  |  |  |  |  |
  1  2  3  4  5  6  7  8

SN74HC595 pin names:

   1: Q2
   2: Q3
   3: Q4
   3: Q5
   5: Q6
   6: Q7
   7: Q8 
   8: GND
   9: QH*: for daisy chaining, unused otherwise
  10: SRCLR* (to 5V)
  11: SRCLK (to D4): clock pin
  12: RCLK (to D3): latch pin
  13: OE* (to GND)
  14: SER (to D2): data pin
  15: Q1
  16: VCC (to 5V)

First shift register SN74HC595 connections:

        D2
        |
        |   +-----+
        +---+  R  +---GND
        |   +-----+
        |
  16 15 14 13 12 11 10 9
  |  |  |  |  |  |  |  |
  +--+--+--+--+--+--+--+
  |>                   |
  +--+--+--+--+--+--+--+
  |  |  |  |  |  |  |  |
  1  2  3  4  5  6  7  8
     

   1: RGB LED 2, red, displays 2 seconds
   2: RGB LED 3, red, displays 4 seconds
   3: RGB LED 4, red, displays 8 seconds
   3: RGB LED 5, red, displays 16 seconds
   5: RGB LED 6, red, displays 32 seconds
   6: RGB LED 4, green, displays 1 minutes
   7: RGB LED 5, green, displays 2 minutes 
   8: GND
   9: to pin 14 of the second shift register (this is the daisy chain connection)
  10: to 5V
  11: to D4: clock pin
  12: to D3: latch pin
  13: to GND
  14: to D2: data pin, and to pull-down resistance R
  15: RGB LED 1, red, displays 1 seconds
  16: to 5V
  R: pull down resistance, 10 kOhm (brown-black-orange-gold)
  GND: Arduino GND pin

Second shift register SN74HC595 connections:

  16 15 14 13 12 11 10 9
  |  |  |  |  |  |  |  | 
  +--+--+--+--+--+--+--+
  |>                   |
  +--+--+--+--+--+--+--+
  |  |  |  |  |  |  |  |
  1  2  3  4  5  6  7  8


   1: RGB LED 7, green, displays 8 minutes
   2: RGB LED 8, green, displays 16 minutes
   3: RGB LED 9, green, displays 32 minutes
   3: RGB LED 6, blue, displays 1 hours
   5: RGB LED 7, blue, displays 2 hours
   6: RGB LED 8, blue, displays 4 hours
   7: RGB LED 9, blue, displays 8 hours
   8: GND
   9: unused, used for daisy chaining otherwise
  10: to 5V
  11: to D4: clock pin
  12: to D3: latch pin
  13: to GND
  14: connect to pin 9 of first shift register (this is the daisy chain connection)
  15: RGB LED 6, green, displays 4 minutes
  16: to 5V

Piezo:
  
    6              GND
    |  +--------+  |
    +--+   P    +--+
       +--------+

  6: piezo pin
  P: Piezo
  GND: Arduino GND pin

  
Left capacitive sensor:


       +--------+     +--------+  
   8 --+   RS   +--+--+   RH   |-- 9
       +--------+  |  +--------+
                   |
                   X
    
  8: sensor pin
  9: helper pin
  RH: 'resistance helper', resistance of at least 1 Mega-Ohm (brown-black-green-gold)
  RS: 'resistance sensor', resistance of 1 kOhm (brown-black-red-gold)
  X: place to touch wire

Right capacitive sensor:
  
       +--------+     +--------+  
  10 --+   RS   +--+--+   RH   |-- 9
       +--------+  |  +--------+
                   |
                   X
  
  10: sensor pin
  9: helper pin
  RH: 'resistance helper', resistance of at least 1 Mega-Ohm (brown-black-green-gold)
  RS: 'resistance sensor', resistance of 1 kOhm (brown-black-red-gold)
  X: place to touch wire

RGB LEDs:
    ___
   /   \
  |     |
  +-+-+-+
  | | | |
  | | | |
  | | |
    |
    
  1 2 3 4

1: Blue, connect with resistance of 1000 (brown-black-red-gold) to Arduino pin
2: GND
3: Red, connect with resistance of 1000 (brown-black-red-gold) to Arduino pin
4: Green, connect with resistance of 2200 (red-red-red-gold) to Arduino pin


Uploading

When uploading, disconnect the reset pin, otherwise you get async() errors

*/

#include <CapacitiveSensor.h>
#include <Time.h>

//If NDEBUG is #defined, it is a release version
//If NDEBUG is commented out, it is a debug version
//#define NDEBUG

//If you use an USB cable or adapter, set use_usb_for_power to true
//If you use 4x AAA batteries, set use_usb_for_power to false
const bool use_usb_for_power = false;

const int datapin  = 2;
const int latchpin = 3;
const int clockpin = 4;
const int pin_16_hours = 5; //The pin connected to the LED to show 16 hours

const int pin_piezo = 6; //The pin connected to the piezo

const int pin_sensor1 =  8;
const int pin_helper1 =  9;
CapacitiveSensor sensor1 
  = CapacitiveSensor(pin_helper1,pin_sensor1);        

const int pin_sensor2 =  10;
const int pin_helper2 = pin_helper1;
CapacitiveSensor sensor2 
  = CapacitiveSensor(pin_helper2,pin_sensor2);        

//Threshold for capacitive sensor, which determines the sensitivity of the sensors
// - too low: the program will think more often there is a touch, possibly even when you do not touch
// - too high: the program will think less often there is a touch, possibly even when you do touch
//Some guidelines I use:
// - Using an adapter or USB: 200
// - Using 4x 1.5 AAA battery: 50
const int threshold_usb = 200;
const int threshold_batteries = 50;

void OnError(const String& error_message)
{
  Serial.print("ERROR: ");  
  Serial.println(error_message);  
  while (1)
  {
    //Blink LED
    digitalWrite(pin_16_hours,!digitalRead(pin_16_hours));
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
  pinMode(latchpin,OUTPUT);
  pinMode(clockpin,OUTPUT);
  pinMode(datapin ,OUTPUT);
  pinMode(pin_16_hours , OUTPUT);
  Serial.begin(9600); //Cannot be used: chip is used stand-alone
  #ifndef NDEBUG
  Serial.println("PiClock v. 1.2 (debug version)");
  #else //NDEBUG
  Serial.println("PiClock v. 1.2 (release version)");
  #endif //NDEBUG
  TestTime();
  ShowBinary(0);
}

//0 = 00 : none pressed
//1 = 01 : right pressed
//2 = 10 : left pressed
//3 = 11 : both pressed
const int state_no_sensor_pressed    = 0; //00
const int state_right_sensor_pressed = 1; //01
const int state_left_sensor_pressed  = 2; //10
const int state_both_sensors_pressed = 3; //11

///Obtain the current state of both capacitive sensors
int GetSensors()
{
  //The higher 'samples' is set, the more accurate the sensors measure
  const int samples = 30;
  //Measure the capacitive sensors
  const int r1 = sensor1.capacitiveSensor(samples);
  const int r2 = sensor2.capacitiveSensor(samples);
  //The threshold value, which determines the sensitivity of the sensors
  // - too low: the program will think more often there is a touch, possibly even when you do not touch
  // - too high: the program will think less often there is a touch, possibly even when you do touch
  //Some guidelines I use:
  // - Using an adapter or USB: 200
  // - Using 4x 1.5 AAA battery: 50
  const int threshold = use_usb_for_power ? threshold_usb : threshold_batteries;
  const int state =  (r1 >= threshold ? 2 : 0) + (r2 >= threshold ? 1 : 0);
  return state;
}

/// User can choose to set the hours
/// - left sensor: cancel
/// - right sensor: set hours
void SetHours()
{
  Serial.println("Setting hours");
  int h = hour();
  bool accept = false; //Accept editing the hours
  while (1)
  {
    const int sensor_state = GetSensors();
    if (sensor_state == state_left_sensor_pressed)
    {
      //  setTime(hours, minutes, seconds, days, months, years);
      setTime(h,minute(),second(),0,0,0);
      break; 
    }
    if (sensor_state == state_right_sensor_pressed)
    {
      if (accept) { h = (h + 1) % 24; }
      else { accept = true; }
    }
    ShowTime(0,0,h);
    delay(100);
  }
  #ifndef NDEBUG
  if (h <  0) { OnError("h <  0, h = " + String(h)); }
  if (h > 23) { OnError("h > 23, h = " + String(h)); }
  #endif // NDEBUG
}

/// User can choose to set the minutes
/// - left sensor: cancel
/// - right sensor: set minutes
void SetMinutes()
{
  Serial.println("Setting minutes");
  int m = minute();
  bool accept = false; //Accept editing the minutes
  while (1)
  {
    ShowTime(0,m,0);
    const int sensor_state = GetSensors();
    if (sensor_state == state_left_sensor_pressed)
    {
      //  setTime(hours, minutes, seconds, days, months, years);
      setTime(hour(),m,second(),0,0,0);
      break; 
    }
    if (sensor_state == state_right_sensor_pressed)
    {
      if (accept) { m = (m + 1) % 60; }
      else { accept = true; }
    }
    delay(100);
  }
  #ifndef NDEBUG
  if (m <  0) { OnError("m <  0, h = " + String(m)); }
  if (m > 59) { OnError("m > 59, h = " + String(m)); }
  #endif // NDEBUG
}

/// User can choose to set the seconds
/// - left sensor: cancel
/// - right sensor: set seconds
void SetSeconds()
{
  Serial.println("Setting seconds");
  int s = second();
  bool accept = false; //Accept editing the seconds
  while (1)
  {
    ShowTime(s,0,0);
    const int sensor_state = GetSensors();
    if (sensor_state == state_left_sensor_pressed)
    {
      //  setTime(hours, minutes, seconds, days, months, years);
      setTime(hour(),minute(),s,0,0,0);
      break; 
    }
    if (sensor_state == state_right_sensor_pressed)
    {
      if (accept) { s = (s + 1) % 60; }
      else { accept = true; }
    }
    delay(100);
  }
  #ifndef NDEBUG
  if (s <  0) { OnError("s <  0, h = " + String(s)); }
  if (s > 59) { OnError("s > 59, h = " + String(s)); }
  #endif // NDEBUG
}


/// User can choose to set the time
/// - left sensor: select hours, minutes, seconds, cancel
/// - right sensor: change hours, minutes, seconds
void SetTime()
{
  Serial.println("Setting time");
  //The user can choose to set the hours, make the hours blink
  while (1)
  {
    //Blink hours
    ShowNonsenseTime(0,0,31);
    delay(100);
    ShowTime(0,0,0);
    delay(100);
    const int sensor_state = GetSensors();
    if (sensor_state == state_right_sensor_pressed)
    {
      SetHours();
      break;
    }
    if (sensor_state == state_left_sensor_pressed)
    {
      break; 
    }
  }  
  //The user can choose to set the minutes, make the minutes blink
  while (1)
  {
    //Blink minutes
    ShowNonsenseTime(0,63,0);
    delay(100);
    ShowTime(0,0,0);
    delay(100);
    const int sensor_state = GetSensors();
    if (sensor_state == state_right_sensor_pressed)
    {
      SetMinutes();
      break;
    }
    if (sensor_state == state_left_sensor_pressed)
    {
      break; 
    }
  }  
  //The user can choose to set the seconds, make the seconds blink
  while (1)
  {
    //Blink second
    ShowNonsenseTime(63,0,0);
    delay(100);
    ShowTime(0,0,0);
    delay(100);
    const int sensor_state = GetSensors();
    if (sensor_state == state_right_sensor_pressed)
    {
      SetSeconds();
      break;
    }
    if (sensor_state == state_left_sensor_pressed)
    {
      break; 
    }
  }  
}

void SetTimeFromSerial()
{
  //Serial.println("Start of SetTimeFromSerial");
  const int  h  = Serial.available() ? Serial.parseInt() : -1;
  delay(10);
  const char c1 = Serial.available() ? Serial.read() : '0';
  delay(10);
  const int  m  = Serial.available() ? Serial.parseInt() : -1;
  delay(10);
  //If the user does not supply seconds, it is assumed he/she wants 0 seconds
  const char c2 = Serial.available() ? Serial.read() : '0';
  delay(10);
  const int  s  = Serial.available() ? Serial.parseInt() : 0;
  delay(10);
  const String used = String(h) + String(c1) + String(m) + String(c2) + String(s);
  if (h == -1) 
  {
    Serial.println(String("No hours, use e.g. '12:34:56' (used '") + used + String("')"));
    return;
  }
  if (c1 == '0') 
  {
    Serial.println("No first seperator, use e.g. '12:34:56' (used '" + used + "')");
    return;
  }
  if (m == -1) 
  {
    Serial.println("No minutes, use e.g. '12:34:56' (used '" + used + "')");
    return;
  }
  //if (c2 == '0') 
  //{
  //  Serial.println("No second seperator, use e.g. '12:34:56'");
  //  return;
  //}
  //if (s == -1) 
  //{
  //  Serial.println("No seconds, use e.g. '12:34:56'");
  //  return;
  //}
  if (h < 0 || h > 23)
  {
    Serial.println("Hours must be in range [0,23]");
    return;
  }
  if (m < 0 || h > 59)
  {
    Serial.println("Minutes must be in range [0,59]");
    return;
  }
  if (s < 0 || s > 59)
  {
    Serial.println("Seconds must be in range [0,59]");
    return;
  }
  //  setTime(hours, minutes, seconds, days, months, years);
  setTime(h,m,s,0,0,0);

}

void loop() 
{
  // Is it Pi o'clock yet? That is, 3:14 PM, also known as 15:14
  bool is_pi_oclock = false;
  int last_sec = -1; //The previous second, used to detect a change in time, to be sent to serial monitor
  
  while (1)
  {
    //Respond to touches
    const int sensors_state = GetSensors();
    if (sensors_state == state_left_sensor_pressed) { SetTime(); delay(100); }

    if (Serial.available())
    {
      delay(100);
      SetTimeFromSerial();  
    }
    //Show the time
    const int s = second();
    const int m =  minute();
    const int h = hour();


    if (last_sec == s) 
    {
      continue;
    }

    last_sec = s;
    ShowTime(s,m,h);

    //if (sensors_state == state_right_sensor_pressed) 
    { 
      //Send debug message to console window
      const String time_now = String(h) + ":" + String(m) + ":" + String(s);
      Serial.println(time_now);
      delay(100);
    }

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

///Show the time on all RGB LEDs
void ShowTime(const int secs, const int mins, const int hours)
{
  #ifndef NDEBUG
  if (hours <  0) { OnError("ShowTime: hours <  0, hours = " + String(hours)); }
  if (hours > 23) { OnError("ShowTime: hours > 23, hours = " + String(hours)); }
  if (mins <  0) { OnError("ShowTime: mins <  0, mins = " + String(mins)); }
  if (mins > 59) { OnError("ShowTime: mins > 59, mins = " + String(mins)); }
  if (secs <  0) { OnError("ShowTime: secs <  0, secs = " + String(secs)); }
  if (secs > 59) { OnError("ShowTime: secs > 59, secs = " + String(secs)); }
  #endif // NDEBUG
  ShowNonsenseTime(secs,mins,hours);  
}

///Also allows to display nonsense time, like 31:63:63, which is used in setting the time
void ShowNonsenseTime(const int secs, const int mins, const int hours)
{
  
  //The 16 hours pin is trivial
  digitalWrite(pin_16_hours,hours >= 16 ? HIGH : LOW);
  
  //Write the rest of the time to the two shift registers
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
  
  #ifndef NDEBUG
  if (x < 0) { OnError("ShowTime: x < 0, x = " + String(x) + ", in binary: " +IntToBinary(x)); }
  #endif //NDEBUG
  ShowBinary(x % (256 * 256));
}

///Convert an integer value to a binary string
String IntToBinary(const long x) //Must be long 
{
  #ifndef NDEBUG
  if (x < 0) { OnError("IntToBinary: x must be positive, x = " + String(x)); }
  #endif //NDEBUG
  String s;
  for (int i=0; i!=16; ++i)
  {
    long one = 1;
    s += ((one << (16 - 1 - i)) & x) ? "1" : "0";  
  }
  return s;
}


///Show a binary value with the RGB LEDs 
void ShowBinary(const long value) //Must be long
{
  #ifndef NDEBUG
  if (value < 0) { OnError("ShowBinary: value must be positive, value = " + String(value)); }
  #endif //NDEBUG
  //Start of modification
  digitalWrite(latchpin,LOW);

  //Calculate what to write
  // (these are two values: one per shift register)
  const long high_value = value / 256;
  const long low_value  = value % 256;

  //#ifndef NDEBUG
  //Serial.print("high_value: ");
  //Serial.print(IntToBinary(high_value));
  //Serial.print(", low_value: ");
  //Serial.println(IntToBinary(low_value));
  //#endif // NDEBUG

  //Write to shift registers
  //(note: would you only connect one shift register,
  // you will only see low_value,
  // as high_value will simply be overwritten)
  WriteToShiftRegister(high_value); 
  WriteToShiftRegister(low_value);

  //Done with modification
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

# PiClock

Arduino project for a clock that displays the time in binary beeps at pi o'clock PM


![Pi Clock 1](PiClock1.jpg)
![Pi Clock 2](PiClock2.jpg)
![Pi Clock 3](PiClock3.jpg)
![Pi Clock 4](PiClock4.jpg)

## How to read the time

Determine which LED goes on an off every second. This is LED with index 0. Then the LEDs are ordered clockwise. LEDs 4 and 8 change state every 1 in 5 seconds, to indicate their position.

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
| seconds   |  1 | 2 | 4 | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  | 4 | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |

Examples

| LED       |  R | R | R | R |    | G  | G | G | B  | B  | B | B | Blue  |

This equals: 3:14:15 (hh:mm:ss)

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
| LED       |  R | R | R | R |    | G  | G | G | B  | B  | B | B | Blue  |
| seconds   |  1 | 2 | 4 | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  | 4 | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |

## Backside

![Pi Clock 5](PiClock5.jpg)
![Pi Clock 6](PiClock6.jpg)
![Pi Clock 7](PiClock7.jpg)

## Prototype

![Pi Clock Prototype 1](PiClockPrototype1.jpg)
![Pi Clock Prototype 2](PiClockPrototype2.jpg)

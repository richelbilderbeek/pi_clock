# PiClock

Arduino project for a clock that displays the time in binary and beeps at pi o'clock PM

[Movie of the PiClock ticking (.mpg) ](NeoPixels/PiClock.mpg)

![Pi Clock 1](NeoPixels/PiClockFront1.jpg)
![Pi Clock 2](NeoPixels/PiClockFront2.jpg)
![Pi Clock 3](NeoPixels/PiClockFront3.jpg)
![Pi Clock 4](NeoPixels/PiClockFront4.jpg)


## How to read the time

Determine which LED goes on an off every second. This is LED with index 0. Then the LEDs are ordered clockwise. LEDs 4 and 8 change state every 1 in 5 seconds, to indicate their position.

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|------:|
| seconds   |  1 | 2 | 4 | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  | 4 | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |

### Example 1

Image the LEDs having the following colors (R: Red, G: Green, B: blue, blank: no color):

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|
| LED       |  R | R |   |   |    | G  |   |   | B  |    |   |   |

This equals: 1:02:03 (hh:mm:ss)

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|------:|
| LED       |  R | R |   |   |    | G  |   |   | B  |    |   |   |
| seconds   |  1 | 2 | 4 | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  | 4 | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |


### Example 2

Image the LEDs having the following colors (R: Red, G: Green, B: blue, blank: no color):

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|
| LED       |  R | R | R | R |    | G  | G | G | B  | B  | B | B |

This equals: 3:14:15 (hh:mm:ss)

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|------:|
| LED       |  R | R | R | R |    | G  | G | G | B  | B  | B | B |       |
| seconds   |  1 | 2 | 4 | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  | 4 | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |

### Example 3

Image the LEDs having the following colors (R: Red, G: Green, B: blue, blank: no color, M: magenta (R + B), Y: yellow (R + G), C: cyan (G + B)):

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|
| LED       |  M | R |   | R | Y  | Y  |   | G | C  | C  | B |   |

This equals: 23:59:59 (hh:mm:ss)

| LED index |  0 | 1 | 2 | 3 | 4  | 5  | 6 | 7 | 8  | 9  | A | B | Color |
|-----------|----|---|---|---|----|----|---|---|----|----|---|---|------:|
| LED       |  M | R |   | R | Y  | Y  |   | G | C  | C  | B |   |
| seconds   |  1 | 2 |   | 8 | 16 | 32 |   |   |    |    |   |   | Red   |
| minutes   |    |   |   |   | 1  | 2  |   | 8 | 16 | 32 |   |   | Green |
| hours     | 16 |   |   |   |    |    |   |   | 1  | 2  | 4 | 8 | Blue  |

## Backside

![Pi Clock 5](NeoPixels/PiClock5.jpg)
![Pi Clock 6](NeoPixels/PiClock6.jpg)
![Pi Clock 7](NeoPixels/PiClock7.jpg)

## Prototype

![Pi Clock Prototype 1](NeoPixels/PiClockPrototype1.jpg)
![Pi Clock Prototype 2](NeoPixels/PiClockPrototype2.jpg)

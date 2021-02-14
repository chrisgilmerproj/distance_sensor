# Distance Sensor

The purpose of this project is to provide a distance sensor that changes color of an LED based on distance
from the sensor. The colors are Green, Yellow, Red like a stop-light.

## Parts

* [Arduino Uno](https://www.sparkfun.com/products/11021)
* [HC-SR04 Echo Distance Sensor](https://www.sparkfun.com/products/15569)
* [BlinkM MaxM](https://oldsite.thingm.com/products/blinkm-maxm/) or [SparkFun part](https://www.sparkfun.com/products/9000)
* [serLCD Display](https://www.sparkfun.com/products/16397)

## Code

Some code was borrowed to make this project work. Some but not all is listed here.

* [Arduino and HC-SR04](https://www.instructables.com/Simple-Arduino-and-HC-SR04-Example/)
* [Serial UART for serLCD](https://learn.sparkfun.com/tutorials/avr-based-serial-enabled-lcds-hookup-guide/serial-uart-example-code---basic)
* [BlinkM MaxM Examples](https://github.com/todbot/BlinkM-Examples)
* [Smoothing](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Smoothing)

## Libraries

This project relies on the BlinkM MaxM libary `BlinkM_funcs.h` which was not written by me but added to the
repository for compiling.


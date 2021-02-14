/*
* Ultrasonic Sensor HC-SR04 and Arduino Tutorial
*
* serLCD Firmware Help: https://learn.sparkfun.com/tutorials/avr-based-serial-enabled-lcds-hookup-guide/firmware-overview
*/

#include <SoftwareSerial.h>
#include "Wire.h"
#include "BlinkM_funcs.h"

const int blinkm_addr = 0;
const int rxPin = 6;    // Rx (Not Used) for serLCD display
const int txPin = 7;    // Tx for serLCD display
const int trigPin = 9;  // HC-SR04 Trigger Pin
const int echoPin = 10; // HC-SR04 Echo Pin
const int ledPin = 13;  // pin for the LED

byte contrast = 2; //Lower is more contrast. 0 to 5 works for most displays.
byte primaryBrightness = 157; // 128 - 157, lower is dimmer, higher is brighter
byte greenBrightness = 187;   // 158-187 

// defines variables
long duration;
int distance;

// Averaging variables
const int numReadings = 3; // Not many values are needed
int readings[numReadings];
int readIndex = 0;
int total = 0;
int average = 0;

SoftwareSerial OpenLCD(rxPin, txPin); // RX (not used), TX

void setup() {
  
  // LED Setup
  pinMode(ledPin, OUTPUT);    // sets the digital pin 13 as output
  BlinkM_beginWithPower();
  BlinkM_stopScript(blinkm_addr);  // turn off startup script

  // HC-SR04 Sensor Setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  // Starts the serial communication for debug
  // Serial.begin(9600);

  // serLCD Display Setup
  OpenLCD.begin(9600); //Start communication with OpenLCD

  OpenLCD.write('|'); // Put LCD into setting mode
  OpenLCD.write(24); // Send contrast command
  OpenLCD.write(contrast); // Update contrast

  OpenLCD.write('|'); //Put LCD into setting mode
  OpenLCD.write(primaryBrightness); // Update brightness
    
  //Send the reset command to the display - this forces the cursor to return to the beginning of the display
  OpenLCD.write('|'); //Send setting character
  OpenLCD.write('-'); //Send clear display character

  // Initialize the array to zero
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  // Delay for startup message on serLCD display
  delay(2000);
}

void loop() { 
  // Reset the position of the first character to ensure it doesn't scroll
  OpenLCD.write(254); // Send command character
  OpenLCD.write(128 + 0 + 0); //Change the position (128) of the cursor to 1st row (0), position 0 (0)

  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2; // speed of sound divided by 2

  // Print to serial before for debugging only
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  //Serial.print(" cm   ");
  
  // Send the clear command to the display - this returns the cursor to the beginning of the display
  OpenLCD.write('|'); //Setting character
  OpenLCD.write('-'); //Clear display
    
  // Avoid doing anything with bad data
  if (distance > 400 || distance < 2) {
    // This covers the known range
    // Quickly get another reading
    digitalWrite(13, LOW);  // sets the digital pin 13 off
    BlinkM_off( blinkm_addr );
    OpenLCD.print("Distance:       No Reading      "); //For 16x2 LCD
    delay(50);
  } else {

    // Average the values
    total = total - readings[readIndex]; // Remove last value from total
    readings[readIndex] = distance;      // Set current value in array
    total = total + readings[readIndex]; // Add new value to total
    readIndex++;                         // Move to next value in array

    // if we're at the end of the array wrap to the beginning
    if (readIndex >= numReadings) {
      readIndex = 0;
    }

    average = total / numReadings; // Average the values

    // Light control based on distance in cm
    if (average < 100) {
      digitalWrite(13, HIGH); // sets the digital pin 13 on
      BlinkM_fadeToRGB( blinkm_addr, 255, 0, 0);
    } else if (average < 200) {
      digitalWrite(13, LOW);  // sets the digital pin 13 off
      BlinkM_fadeToRGB( blinkm_addr, 255, 165, 0);
    } else if (average < 400) {
      digitalWrite(13, LOW);  // sets the digital pin 13 off
      BlinkM_fadeToRGB( blinkm_addr, 0, 255, 0);
    }
  
    // Print after changing light
    OpenLCD.print("Distance: "); //For 16x2 LCD
    OpenLCD.print(average);
    OpenLCD.print(" cm            "); // Extra spaces overwrite extra chars
  
    delay(500);
  }
  
}

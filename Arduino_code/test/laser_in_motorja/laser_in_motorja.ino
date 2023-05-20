#include <Arduino.h>
/*Example sketch to control a stepper motor with A4988 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */

// Define stepper motor connections and steps per revolution:
#define dirPin1 6
#define stepPin1 9
#define dirPin2 A2
#define stepPin2 A3


#include <Wire.h>
#include "Adafruit_VL6180X.h"

Adafruit_VL6180X vl = Adafruit_VL6180X();



void setup() {
  // Declare pins as output:
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(dirPin2, OUTPUT);


  Serial.begin(9600);

  // wait for serial port to open on native usb devices
  while (!Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL6180x test!");

}


void loop() {
  // Set the spinning direction clockwise:

  // Set the spinning direction counterclockwise:
  
  
  
  for ( int i = 0; i < 3200; i++){
    
  digitalWrite(dirPin1, LOW);

  digitalWrite(stepPin1, HIGH);
  delayMicroseconds(500);
  
  digitalWrite(stepPin1, LOW);
  delayMicroseconds(500);
  }
    
  for ( int i = 0; i < 3200; i++){
    
  digitalWrite(dirPin2, LOW);

  digitalWrite(stepPin2, HIGH);
  delayMicroseconds(500);
  
  digitalWrite(stepPin2, LOW);
  delayMicroseconds(500);
  }


  // Some error occurred, print it out!


}

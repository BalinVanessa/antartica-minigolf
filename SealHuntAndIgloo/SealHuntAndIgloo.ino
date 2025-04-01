#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h" //Contains #defines for common SCMD register names and values
#include "Wire.h"

// MOTOR
SCMD myMotorDriver; //This creates the main object of one motor driver and connected peripherals.
#define MOTOR_NUM 0

// BREAKBEAM SENSOR CONSTANTS
#define LEDPIN 13
#define SENSORPIN 4

// LED CONSTANTS
#define NUM_LEDS 30
#define DATA_PIN 6

// VARIABLES
int sensorState = 0, lastState=0;         // variable for reading the pushbutton status
bool triggerIglooLED = false;

// Create a NeoPixel object
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting sketch.");

  //***** Configure the Motor Driver's Settings *****//
  //  .commInter face is I2C_MODE 
  myMotorDriver.settings.commInterface = I2C_MODE;

  //  set address if I2C configuration selected with the config jumpers
  myMotorDriver.settings.I2CAddress = 0x5D; //config pattern is "1000" (default) on board for address 0x5D

  //  set chip select if SPI selected with the config jumpers
  myMotorDriver.settings.chipSelectPin = 10;

  //*****initialize the driver get wait for idle*****//
  while ( myMotorDriver.begin() != 0xA9 ) //Wait until a valid ID word is returned
  {
    Serial.println( "ID mismatch, trying again" );
    delay(500);
  }
  Serial.println( "ID matches 0xA9" );

  //  Check to make sure the driver is done looking for peripherals before beginning
  Serial.print("Waiting for enumeration...");
  while ( myMotorDriver.ready() == false );
  Serial.println("Done.");
  Serial.println();

  while ( myMotorDriver.busy() );
  myMotorDriver.enable(); //Enables the output driver hardware

    // initialize the LED pin as an output:
  pinMode(LEDPIN, OUTPUT);      
  // initialize the sensor pin as an input:
  pinMode(SENSORPIN, INPUT);     
  digitalWrite(SENSORPIN, HIGH); // turn on the pullup

  strip.begin(); // Initialize NeoPixel strip

}

void loop() {
  // put your main code here, to run repeatedly:
  //pass setDrive() a motor number, direction as 0(call 0 forward) or 1, and level from 0 to 255
  myMotorDriver.setDrive( MOTOR_NUM, 0, 255);

  //IGLOO LED CODE
  // read the state of the pushbutton value:
  sensorState = digitalRead(SENSORPIN);

  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:
  if (sensorState == LOW) {     
    // turn LED on:
    digitalWrite(LEDPIN, HIGH); 
    triggerIglooLED = !triggerIglooLED;
    updateLED();
  } 
  else {
    // turn LED off:
    digitalWrite(LEDPIN, LOW); 
  }
  
  if (sensorState && !lastState) {
    Serial.println("Unbroken");
  } 
  if (!sensorState && lastState) {
    Serial.println("Broken");
  }
  lastState = sensorState;
}

void updateLED() {
  if(triggerIglooLED) {
    IglooLED();
  }

  delay(3000);
  triggerIglooLED = false;
  turnOffLED();
}

void IglooLED() {
  // Set all LEDs to blue
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 255)); // Blue color
  }

  // Display the colors
  strip.show(); // Update the display
}

void turnOffLED()
{
  // Set all LEDs to off
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0)); // Blue color
  }

  // Display the colors
  strip.show(); // Update the display

}

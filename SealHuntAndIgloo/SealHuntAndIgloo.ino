#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <stdint.h>
#include "SCMD.h"
#include "SCMD_config.h" //Contains #defines for common SCMD register names and values
#include "Wire.h"

// SEAL HUNT MOTOR VARIABLES
SCMD sealHuntMotorDriver; //This creates the main object of one motor driver and connected peripherals.
#define MOTOR_NUM 0

// IGLOO BREAKBEAM SENSOR CONSTANTS
#define LEDPIN 13
#define IGLOO_SENSOR_PIN 4

// IGLOO LED CONSTANTS
#define IGLOO_NUM_LEDS 30
#define IGLOO_LED_PIN 6
#define IGLOO_LED_DELAY 3000;

// IGLOO VARIABLES
int iglooSensorState = 0, lastState=0;       
bool triggerIglooLED = false;
bool areIglooLightsOn = false;
long timeToStopIglooLED;

// Create a NeoPixel object
Adafruit_NeoPixel IGLOO_LED_STRIP = Adafruit_NeoPixel(IGLOO_NUM_LEDS, IGLOO_LED_PIN, NEO_GRB + NEO_KHZ800);

 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Starting sketch.");

  //***** Configure the Motor Driver's Settings *****//
  //  .commInter face is I2C_MODE 
  sealHuntMotorDriver.settings.commInterface = I2C_MODE;

  //  set address if I2C configuration selected with the config jumpers
  sealHuntMotorDriver.settings.I2CAddress = 0x5D; //config pattern is "1000" (default) on board for address 0x5D

  //  set chip select if SPI selected with the config jumpers
  sealHuntMotorDriver.settings.chipSelectPin = 10;

  //*****initialize the driver get wait for idle*****//
  while ( sealHuntMotorDriver.begin() != 0xA9 ) //Wait until a valid ID word is returned
  {
    Serial.println( "ID mismatch, trying again" );
    delay(500);
  }
  Serial.println( "ID matches 0xA9" );

  //  Check to make sure the driver is done looking for peripherals before beginning
  Serial.print("Waiting for enumeration...");
  while ( sealHuntMotorDriver.ready() == false );
  Serial.println("Done.");
  Serial.println();

  while ( sealHuntMotorDriver.busy() );
  sealHuntMotorDriver.enable(); //Enables the output driver hardware

    // initialize the LED pin as an output:
  pinMode(LEDPIN, OUTPUT);      
  // initialize the sensor pin as an input:
  pinMode(IGLOO_SENSOR_PIN, INPUT);     
  digitalWrite(IGLOO_SENSOR_PIN, HIGH); // turn on the pullup

  IGLOO_LED_STRIP.begin(); // Initialize NeoPixel strip

}

void loop() {
  // put your main code here, to run repeatedly:
  //pass setDrive() a motor number, direction as 0(call 0 forward) or 1, and level from 0 to 255
  sealHuntMotorDriver.setDrive( MOTOR_NUM, 0, 255);

  //IGLOO LED CODE
  // read the state of the pushbutton value:
  iglooSensorState = digitalRead(IGLOO_SENSOR_PIN);

  // check if the sensor beam is broken
  // if it is, the iglooSensorState is LOW:
  if (iglooSensorState == LOW) {     
    // turn LED on:
    digitalWrite(LEDPIN, HIGH); 
    turnOnIglooLED();
  } 
  else {
    // turn LED off:
    digitalWrite(LEDPIN, LOW); 
  }
  
  // serial prints to check if the igloo breakbeam sensor has been broken or not
  if (iglooSensorState && !lastState) {
    Serial.println("Unbroken");
  } 
  if (!iglooSensorState && lastState) {
    Serial.println("Broken");
  }

  lastState = iglooSensorState;
  updateIglooLED();
}

void updateIglooLED() {
  if(areIglooLightsOn && millis() >= timeToStopIglooLED) {
    turnOffIglooLED();
  }
}

void turnOnIglooLED() {
  areIglooLightsOn = true;

  // Set all LEDs to blue
  for (int i = 0; i < IGLOO_LED_STRIP.numPixels(); i++) {
    IGLOO_LED_STRIP.setPixelColor(i, IGLOO_LED_STRIP.Color(0, 0, 255)); // Blue color
  }

  // Display the colors
  IGLOO_LED_STRIP.show(); // Update the display
  timeToStopIglooLED = millis() + IGLOO_LED_DELAY;
}

void turnOffIglooLED()
{
  areIglooLightsOn = false;

  // Set all LEDs to off
  for (int i = 0; i < IGLOO_LED_STRIP.numPixels(); i++) {
    IGLOO_LED_STRIP.setPixelColor(i, IGLOO_LED_STRIP.Color(0, 0, 0)); // Blue color
  }

  // Display the colors
  IGLOO_LED_STRIP.show(); // Update the display

}
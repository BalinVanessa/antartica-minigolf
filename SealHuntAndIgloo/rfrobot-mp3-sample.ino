#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include "SparkFun_STHS34PF80_Arduino_Library.h"
#include <Wire.h>

// EGG HATCH MP3 PLAYER
SoftwareSerial DF1201SSerial(2, 3);
DFRobot_DF1201S HATCH_SOUND_MP3;

// EGG HATCH HUMAN SENSOR VARIABLES
STHS34PF80_I2C nestHandSensor;
int16_t nestHandPresenceVal = 0;
int16_t nestHandMotionVal = 0;
float nestHandTemperatureVal = 0;
bool handOverNest;

// EGG HATCH SERVO VARIABLES
#define PENGUIN_SERVO
bool ballInNest;

// Values to fill with presence and motion data
int16_t presenceVal = 0;
int16_t motionVal = 0;
float temperatureVal = 0;

void setup(){
  pinMode(13, OUTPUT);
  // Initialize MP3 player communications
  DF1201SSerial.begin(115200);
  while(!DF1201S.begin(DF1201SSerial)){
    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  DF1201S.setVol(14);  // set a low-ish volume (valid values from 0 to 30)
  delay(500);
  DF1201S.switchFunction(DF1201S.MUSIC); // be a music player, not a USB drive
  delay(500);
  DF1201S.setPlayMode(DF1201S.SINGLE); // tell it to play just one file at a time (do this after switchFunction)
  delay(500);

  Serial.begin(115200);
  Serial.println("STHS34PF80 Example 1: Basic Readings");

  // Begin I2C
  Wire.begin(); // Removed the comparison

  // Establish communication with the device
  if (mySensor.begin() == false)
  {
    Serial.println("Error setting up device - please check wiring.");
    while (1);
  }

  delay(1000);
}

void loop(){
  digitalWrite(13, LOW);
  int result = DF1201S.playFileNum(1); // turn on LED if it play says it worked
  if (result == 1) { // turn on Arduino LED if board says it worked
    digitalWrite(13, HIGH);
  }
  delay(4000);

  sths34pf80_tmos_drdy_status_t dataReady;
    mySensor.getDataReady(&dataReady);

    // Check whether the sensor has new data - run through the loop if data is ready
    if (dataReady.drdy == 1)
    {
        sths34pf80_tmos_func_status_t status;
        mySensor.getStatus(&status);

        // If the presence flag is high, then print data
        if (status.pres_flag == 1)
        {
            // Presence Units: cm^-1
            mySensor.getPresenceValue(&presenceVal);
            Serial.print("Presence: ");
            Serial.print(presenceVal);
            Serial.println(" cm^-1");
        }

        if (status.mot_flag == 1)
        {
            Serial.println("Motion Detected!");
        }

        if (status.tamb_shock_flag == 1)
        {
            mySensor.getTemperatureData(&temperatureVal);
            Serial.print("Temperature: ");
            Serial.print(temperatureVal);
            Serial.println(" °C");
        }
    }
}
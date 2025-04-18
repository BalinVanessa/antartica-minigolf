#include <Servo.h>
#include <SoftwareSerial.h>
#include <DFRobot_DF1201S.h>
#include <Wire.h>
#include <SparkFun_AK975X_Arduino_Library.h>

int ir1, ir2, ir3, ir4, temperature;
AK975X movementSensor;
int position = 10;
// MP3 module connection on pins 2 (RX), 3 (TX)
SoftwareSerial DF1201SSerial(2, 3);
DFRobot_DF1201S DF1201S;

// MP3 VARIABLES
#define HATCH_NOISE_LENGTH  3500; // 3.5 seconds
long timeTillHatchStop;
bool hatchNoiseIsPlaying = false;

void setup() {
  Serial.begin(9600);

  DF1201SSerial.begin(115200);
  while (!DF1201S.begin(DF1201SSerial)) {
    Serial.println("DF1201S not detected, retrying...");
    delay(1000);
  }

  Serial.println("DF1201S online");

  DF1201S.setVol(25); // Volume (0–30)
  delay(500);
  DF1201S.switchFunction(DF1201S.MUSIC);
  delay(500);
  DF1201S.setPlayMode(DF1201S.SINGLE); // Play once, don't loop
  delay(500);
  

  // Human Presence
  Wire.begin();
  if (movementSensor.begin() == false)
  {
    Serial.println("Device not found. Check wiring.");
    while (1);
  }
}

void loop() {
  if (movementSensor.available())
  {
    ir1 = movementSensor.getIR1();
    ir2 = movementSensor.getIR2();
    ir3 = movementSensor.getIR3();
    ir4 = movementSensor.getIR4();
    float tempF = movementSensor.getTemperatureF();

    movementSensor.refresh(); //Read dummy register after new data is read

    //Note: The observable area is shown in the silkscreen. We used these print statements to chek values.
    //If sensor 2 increases first, the human is on the left
    // Serial.print("1:DWN[");
    // Serial.print(ir1);
    // Serial.print("]\t2:LFT[");
    // Serial.print(ir2);
    // Serial.print("]\t3:UP[");
    // Serial.print(ir3);
    // Serial.print("]\t4:RGH[");
    // Serial.print(ir4);
    // Serial.print("]\ttempF[");
    // Serial.print(tempF);
    // Serial.print("]\tmillis[");
    // Serial.print(millis());
    // Serial.print("]");
    // Serial.println();
  }

  bool presenceDetected = (ir1 >  1000) || (ir2 > 1000) || (ir3 > 1000) || (ir4 > 1000);

  if (presenceDetected && millis() >= timeTillHatchStop) {
    DF1201S.playFileNum(1);
    Serial.println("Playing sound!");
    timeTillHatchStop = millis() + HATCH_NOISE_LENGTH;
  }
}
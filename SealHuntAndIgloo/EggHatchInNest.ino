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

Servo penguin;

bool triggered = false; // Only trigger once per detection

void setup() {
  Serial.begin(9600);

  penguin.attach(9);       // Servo on pin 9
  penguin.write(10);       // Initial resting position

  DF1201SSerial.begin(115200);
  while (!DF1201S.begin(DF1201SSerial)) {
    Serial.println("DF1201S not detected, retrying...");
    delay(1000);
  }

  Serial.println("DF1201S online");

  DF1201S.setVol(20); // Volume (0–30)
  DF1201S.setPlayMode(DF1201S.ALLCYCLE); // Play once, don't loop

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

    //Note: The observable area is shown in the silkscreen.
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

  bool presenceDetected = (ir1 > 10000) || (ir2 > 10000) || (ir3 > 10000) || (ir4 > 10000);

  if (presenceDetected) {
    DF1201S.start();      // Play MP3 file 1
    delay(100);
    position = 170;
    penguin.write(position);            // Rotate servo
    triggered = true;             // Lock trigger
  }
  else if (!presenceDetected) {
    DF1201S.pause();
    delay(100);
    position = 10;
    penguin.write(position);            // Return to resting
    triggered = false;            // Reset trigger flag
    
  }
}
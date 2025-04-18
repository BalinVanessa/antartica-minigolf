#include <Servo.h>

// ORCA OBSTACLES SERVOS
Servo ORCA_JUMP_SERVO_1;
Servo ORCA_JUMP_SERVO_2;
Servo ORCA_TAIL_SERVO;

#define ORCA_JUMP_SERVO_PIN_1   7
#define ORCA_JUMP_SERVO_PIN_2   6
#define ORCA_TAIL_SERVO_PIN     5

// CONSTANTS FOR TINY ORCAS JUMPING
#define TIME_TO_ORCA_JUMP       2000
#define ORCA_ONE_DOWN_ANGLE     130
#define ORCA_ONE_UP_ANGLE       70
#define ORCA_TWO_DOWN_ANGLE     45
#define ORCA_TWO_UP_ANGLE       70
// VARIABLES FOR TINY ORCAS JUMPING
bool orcaIsJumping = false;
bool orcaJumpsInForwardDirection = false;
long timeToStartOrcaJump;

// CONSTANTS FOR ORCA TAIL FLIPPING
#define TIME_TO_FLIP_TAIL       3000
#define TAIL_DOWN_ANGLE         90
#define TAIL_UP_ANGLE           60

// VARIABLES FOR ORCA TAIL FLIPPING
bool orcaTailIsFlipping = false;
bool orcaTailIsUp = false;
long timeToStartTailFlip;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // setup servos
  ORCA_JUMP_SERVO_1.attach(ORCA_JUMP_SERVO_PIN_1);
  ORCA_JUMP_SERVO_2.attach(ORCA_JUMP_SERVO_PIN_2);
  ORCA_TAIL_SERVO.attach(ORCA_TAIL_SERVO_PIN);

  // MOVE ORCAS INTO STARTING POSITION
  ORCA_JUMP_SERVO_1.write(ORCA_ONE_UP_ANGLE);
  ORCA_JUMP_SERVO_2.write(ORCA_TWO_UP_ANGLE);
  ORCA_TAIL_SERVO.write(TAIL_DOWN_ANGLE);

  // set states for orca jumping to false
  orcaIsJumping = false;
  orcaJumpsInForwardDirection = true;
  orcaTailIsFlipping = false;
  orcaTailIsUp = false;

  // start the timer to flip tail and make orcas jump
  timeToStartOrcaJump = millis() + TIME_TO_ORCA_JUMP;
  timeToStartTailFlip = millis() + TIME_TO_FLIP_TAIL;

}

void loop() {
  // // put your main code here, to run repeatedly:
  if(orcaIsJumping && orcaJumpsInForwardDirection) {
    orcaJumpsForward();
  }

  if(orcaIsJumping && !orcaJumpsInForwardDirection) {
    orcaJumpsBackwards();
  }

  if(orcaTailIsFlipping) {
    flipOrcaTail();
  }

  updateOrcaJump();
  updateOrcaTailFlip();
}

void orcaJumpsForward() {
  Serial.println("ORCAS JUMP FORWARD!!");
  ORCA_JUMP_SERVO_1.write(ORCA_ONE_DOWN_ANGLE);
  ORCA_JUMP_SERVO_2.write(ORCA_TWO_DOWN_ANGLE);
  
  resetOrcaJumpTime();
}

void orcaJumpsBackwards() {
  Serial.println("ORCAS JUMP BACKWARDS!!");
  ORCA_JUMP_SERVO_1.write(ORCA_ONE_UP_ANGLE);
  ORCA_JUMP_SERVO_2.write(ORCA_TWO_UP_ANGLE);

  resetOrcaJumpTime();
}

void resetOrcaJumpTime() {
  Serial.println("Resetting orca jump time");
  orcaIsJumping = false;
  timeToStartOrcaJump = millis() + TIME_TO_ORCA_JUMP;
}

void updateOrcaJump() {
  // when time is up, make the jumping orcas jump in the other directions
  if(millis() >= timeToStartOrcaJump && !orcaIsJumping) {
    Serial.println("Time for the orca to jump!");
    orcaJumpsInForwardDirection = !orcaJumpsInForwardDirection; // switch the direction for orca jumping
    orcaIsJumping = true;
  }
}

void flipOrcaTail() {
  if(orcaTailIsUp) {
    Serial.println("Orca tail is flipping up!!");
    ORCA_TAIL_SERVO.write(TAIL_UP_ANGLE);
  } else {
    Serial.println("Orca tail is flipping down!!");
    ORCA_TAIL_SERVO.write(TAIL_DOWN_ANGLE);
  }

  timeToStartTailFlip = millis() + TIME_TO_FLIP_TAIL;
  orcaTailIsFlipping = false; // tail is done moving
}

void updateOrcaTailFlip() {
  if(millis() >= timeToStartTailFlip) {
    Serial.println("Time for the orca tail to move!");
    orcaTailIsUp = !orcaTailIsUp;
    orcaTailIsFlipping = true;
  }
}

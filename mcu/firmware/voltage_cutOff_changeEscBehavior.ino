/*
 * Changes the behavior of motor according to the battery voltage and Li-Po cut-off voltage
 * Added a sigmoidal accel control to preserve the integrity of the mechanical transmission
 * Alexandre Nobuharu Sato, Ribeirão Pires-SP Brasil, 2022-04-13
 */
#include <Servo.h>

// Constants
#define escPin 9
#define voltagePin A4 // limit max 5v
#define receiverChannel0 2 // Your interface here
#define receiverChannel1 3 // Your interface here

// Tunes
const float thresholdVoltage = 3.54; // Adjust according to your voltage divider resistors
const float minVoltage = 3.43; // 1S cut-off Li-Po battery
const float secondsToMaxAccel = 7;
const float accelSlope = 3; // min is 1 and going to a linear ramp

// Global Variables
Servo ESC;  // create servo object to control a ESC
float voltage = 0;
float secondsCount = 0;
int lastDirection = 0; // Save the latest state to help trigger the reset of a ramp accel

// Prototypes
void setup(void);
void run(float percentual); // Changes the behavior of motor according to the battery voltage
float sigmoidalFunction(float secondsCount); // Returns a sigmoidal percentual behavior

void loop() {
  if (voltage > thresholdVoltage)
    run(1);
  else if (voltage <= thresholdVoltage && voltage > minVoltage)
    run(0.5);
  else
    run(0);
}


void setup() {
  ESC.attach(escPin);  // attaches the ESC on pin 9 to the servo object
  Serial.begin(9600); // setup serial for analog read
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(receiverChannel0, INPUT);
  pinMode(receiverChannel1, INPUT);
}


void run(float percentual) {
  if (digitalRead(receiverChannel0) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (lastDirection == 1) {
      secondsCount += 0.02;
    }
    else {
      lastDirection = 1;
      secondsCount = 0.02;
    }
    ESC.write(90 - (90 * percentual * sigmoidalFunction(secondsCount))); // normalizing because 90º is stopped
    delay(20);    
  }
  else if (digitalRead(receiverChannel1) == HIGH) {
    digitalWrite(LED_BUILTIN, HIGH);
    if (lastDirection == 2) {
      secondsCount += 0.02;
    }
    else {
      lastDirection = 2;
      secondsCount = 0.02;
    }
    ESC.write(90 + (90 * percentual * sigmoidalFunction(secondsCount))); // normalizing because 90º is stopped
    delay(20);
  }
  else {
    digitalWrite(LED_BUILTIN, LOW);
    ESC.write(90);
    voltage = analogRead(voltagePin);
    voltage = voltage * (5.0 / 1023.0); // Casting 1024 to float
    lastDirection = 0;
  }
}


float sigmoidalFunction(float secondsCount) {
  return 1 / (1 + exp(-accelSlope * (secondsCount - (secondsToMaxAccel / 2))));
}
 

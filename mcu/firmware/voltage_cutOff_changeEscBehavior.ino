/*
 * Changes the behavior of motor according to the battery voltage and Li-Po cut-off voltage
 * Alexandre Nobuharu Sato, Ribeirão Pires-SP Brasil, 2022-04-13
 */

#include <Servo.h>

// Constants
Servo ESC;  // create servo object to control a ESC
#define escPin 9
#define voltagePin A4 // limit max 5v
const float thresholdVoltage = 3.54; // Adjust according to your voltage divider resistors
const float minVoltage = 3.43; // 1S cut-off Li-Po battery
#define receiverChannel0 3 // Your interface here
#define receiverChannel1 2 // Your interface here

// Global Variables
int pos = 0; 
float voltage = 0;

// Prototypes
void setup(void);
void run(float percentual); // Changes the behavior of motor according to the battery voltage

void loop()
{
  if (voltage > thresholdVoltage)
    run(1);
  else if (voltage <= thresholdVoltage && voltage > minVoltage)
    run(0.5);
  else
    run(0);
}


void setup()
{
  ESC.attach(escPin);  // attaches the ESC on pin 9 to the servo object
  Serial.begin(9600); // setup serial for analog read
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(receiverChannel0, INPUT);
  pinMode(receiverChannel1, INPUT);
}


void run(float percentual)
{
  if (digitalRead(receiverChannel0) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    ESC.write(90 - (90 * percentual)); // normalizing because 90º is stopped
    delay(20);
  }
  else if (digitalRead(receiverChannel1) == HIGH)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    ESC.write(90 + (90 * percentual)); // normalizing because 90º is stopped
    delay(20);
  }
  else
  {
    digitalWrite(LED_BUILTIN, LOW);
    ESC.write(90);
    voltage = analogRead(voltagePin);
    voltage = voltage * (5.0 / 1023.0); // Casting 1024 to float
  }
 }

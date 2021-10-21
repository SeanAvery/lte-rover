#include <Servo.h>

Servo steeringServo;
Servo motorEsc;

int angle = 0;

void setup() {
  // put your setup code here, to run once:
  steeringServo.attach(9);
  motorEsc.attach(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (angle = 60; angle <= 120; angle += 1)
  {
    steeringServo.write(angle);
    motorEsc.write(angle);
    delay(15);
  }

  for (angle = 120; angle >= 60; angle -= 1)
  {
    steeringServo.write(angle);
    motorEsc.write(angle);
    delay(15);
  }
}

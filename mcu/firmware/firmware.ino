#include <stdlib.h>

#include <Servo.h>
#include <SoftwareSerial.h>

// servo instances
Servo steeringServo;
Servo motorEsc;

// global variables
int angle = 90;
int throttle = 90;
int minAngle = 70;
int maxAngle = 110;
int centerAngle = 90; 
int minThrottleForward = 90;
int maxThrottleFortward = 160;
int minThrottleaReverse = 90;
int maxThrottleReverse = 20;

char throttleIndex = 't';
char steeringIndex = 's';
char escapeIndex = '#';

void setup() {
  // setup serial communication over usb port
  Serial.begin(115200);
  // wait for serial port to connect
  while (!Serial) {
    ;
  }
  Serial.println("serial port is connected");
  // setup pwm pins for steering and motor control
  steeringServo.attach(6);
  motorEsc.attach(5);
  Serial.println("motor esc attatched");
}

bool steering_safe(int value)
{
  if (value > maxAngle) 
  {
    return false;
  }
  if (value < minAngle) 
  {
    return false;
  }
  return true;
}

bool throttle_safe(int value)
{
  if (value > maxThrottleFortward) 
  {
    return false;
  }
  if (value < maxThrottleReverse) 
  {
    return false;
  }
  return true;
}

void loop() {
  delay(10);
  if (Serial.available())
  {
    char serialMsg[7];
    size_t msgLength = Serial.readBytesUntil(escapeIndex, serialMsg, 20);

    if (msgLength == 6)
    {
      char header;
      int value;
      sscanf(serialMsg, "%c%5d", header, &value);

      if (serialMsg[0] == throttleIndex) {
        if (throttle_safe(value))
        {
          throttle = value;
        }
      }
      else if (serialMsg[0] == steeringIndex)
      {
        Serial.print("hit new steering");
        if (steering_safe(value))
        {
          angle = value;
        }
      }
    }
  }
  motorEsc.write(throttle);
  steeringServo.write(angle);
}

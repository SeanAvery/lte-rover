#include <stdlib.h>

#include <Servo.h>
#include <SoftwareSerial.h>

// servo instances
Servo steeringServo;
Servo motorEsc;

// usb port communication rx: 0, tx: 1
SoftwareSerial serialPort(0, 1);

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
  serialPort.begin(115200);
  Serial.println("serial port is connected");
  // setup pwm pins for steering and motor control
  steeringServo.attach(5);
  motorEsc.attach(6);
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
  if (serialPort.available())
  {
    char serialMsg[7];
    size_t msgLength = serialPort.readBytesUntil(escapeIndex, serialMsg, 6);

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


/* TESTS */

// steering test loop
//
//bool dir = true;
//void loop() {
//  delay(10);
//  if (dir)
//  {
//    angle = angle + 1;
//  }
//  else
//  {
//    angle = angle - 1;
//  }
//
//  if (steering_safe(angle)) 
//  {
//    Serial.print(angle);
//    steeringServo.write(angle);
//    motorEsc.write(94);
//  }
//  if (!steering_safe(angle))
//  {
//    dir = !dir;
//  }
//}

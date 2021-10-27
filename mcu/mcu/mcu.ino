#include <Servo.h>
#include <SoftwareSerial.h>

// servo instances
Servo steeringServo;
Servo motorEsc;

// usb port communication rx: 0, tx: 1
SoftwareSerial serialPort(0, 1);

// global variables
int angle = 0;
int throttle = 0;
int minAngle = 60;
int maxAngle = 120;
int centerAngle = 90;
int minThrottleForward = 100;
int maxThrottleFortward = 300;
int minThrottleaReverse = 70;
int maxThrottleReverse = 20;

void setup() {
  // setup serial communication over usb port
  Serial.begin(115200);
  // wait for serial port to connect
  while (!Serial) {
    ;
  }
  serialPort.begin(115200);
  Serial.println("serial port is setup");

  // setup pwm pins for steering and motor control
  steeringServo.attach(9);
  motorEsc.attach(10);
  Serial.println("motor esc attatched");
}

void loop() {
  if (serialPort.available())
  {
    // Serial.print("available \n");
    // Serial.write(serialPort.read());
    int throttleMsg = serialPort.parseInt();
    

    
    if (throttleMsg > 0)
    {
      throttle = throttleMsg;
    }
    Serial.println(throttle);
  }
  
  motorEsc.write(throttle);

//  // put your main code here, to run repeatedly:
////  for (angle = 60; angle <= 120; angle += 1)
////  {
////    steeringServo.write(angle);
////    motorEsc.write(angle);
////    delay(15);
////  }
////
////  for (angle = 120; angle >= 60; angle -= 1)
////  {
////    steeringServo.write(angle);
////    motorEsc.write(angle);
////    delay(15);
////  }
}
#include <Servo.h>
#include <SoftwareSerial.h>

//Servo steeringServo;
Servo motorEsc;

SoftwareSerial serialPort(0, 1);
//
int angle = 0;
int throttle = 0;

void setup() {
  // setup serial communication over usb port
  Serial.begin(115200);
  // wait for serial port to connect
  while (!Serial) {
    ;
  }
  serialPort.begin(115200);
  Serial.println("serial port is setup");

//  // setup pwm pins for steering and motor control
//  steeringServo.attach(9);
  motorEsc.attach(10);
  Serial.println("motor esc attatched");
}

void loop() {
  // Serial.write(serialPort.read());
//  // Serial.print("loop \n");
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

//  if (Serial.available())
//  {
//    Serial.print("Serial available");
//    serial.write(Serial.read());
//  }
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

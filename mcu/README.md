# mcu

This is the peripheral device that interacts with dc motor, steering servo, and other accessories on the rc car.

The goal of this microntroller is to keep things simple - just receive serial commands, perform safety checks, and execute controls.

### hardware

Currently using arduino nano dev board that comes with ATmega328P procesessor. Powerd through 5 volts from ESC. Controls pwm of esc and steering servo.

### messaging
Simple string prefixed delineation.

<char prefix><value><#>


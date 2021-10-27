# mcu

This is the peripheral device that interacts with dc motor, steering servo, and other accessories on the rc car.

The goal of this microntroller is to keep things simple - just receive serial commands, perform safety checks, and execute controls.

### hardware

Currently using arduino nano dev board that comes with ATmega328P procesessor.

### messaging
Todo: implement captnproto (this will require stm32).

Simple string prefixed delineation.

T: throttle
S: steering


import hid
import time
import serial
import sys

com_path = "/dev/cu.usbserial-11340"
baudrate = 115200

# connect to gamepad
gamepad = hid.device()
gamepad.open(1356, 2508)
gamepad.set_nonblocking(True)

# connect to serial
ser = serial.Serial(com_path, baudrate)

def scale_steering(value):
  normal = (2 * ((value - 0) / (256-0))) - 1
  return round(90 + (normal * 20))

def scale_throttle(value):
  print("value: ", value)
  normal = (2 * ((value - 0) / (256-0))) - 1
  print(normal)
  return round(90 + (-normal * 20))

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, "#")
  print(str_msg)
  return bytes(str_msg, 'UTF-8')

while True:
  time.sleep(0.001)
  report = gamepad.read(64)
  # left/right analog left [1]
  # up/down analog left [2]
  if report:
    # parse steering
    steering_val = scale_steering(report[1])
    steering_msg = format_msg("s", steering_val)
    # parse throttle
    throttle_val = scale_throttle(report[2])
    throttle_msg = format_msg("t", throttle_val)
    print(throttle_msg)
    # send steering
    ser.write(steering_msg)
    # send throttle
    ser.write(throttle_msg)


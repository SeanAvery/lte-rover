import serial
import struct
import time
import random

mcu_port = "/dev/cu.usbserial-1130"
throttle_index = "t"
steering_index = "s"
escape_char = "#"
min_steering = 60
max_steering = 120
min_throttle = 89
max_throttle= 160
msg_delay=0.05

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, escape_char)
  return bytes(str_msg, 'UTF-8')


def test_steering(ser):
  steering_val = 90;
  for x in range(5):
    while steering_val < max_steering:
      time.sleep(msg_delay)
      steering = format_msg(steering_index, steering_val)
      ser.write(steering)
      steering_val = steering_val + 1
    while steering_val > min_steering:
      time.sleep(msg_delay)
      steering = format_msg(steering_index, steering_val)
      ser.write(steering)
      steering_val = steering_val - 1

def test_throttle(ser):
  # test forward
  throttle_val = 90;
  for x in range(5):
    while throttle_val < max_throttle:
      time.sleep(msg_delay)
      throttle = format_msg(throttle_index, throttle_val)
      ser.write(throttle)
      throttle_val = throttle_val + 1
    while throttle_val > min_throttle:
      time.sleep(msg_delay)
      throttle = format_msg(throttle_index, throttle_val)
      ser.write(throttle)
      throttle_val = throttle_val + 1


if __name__ == "__main__":
  ser = serial.Serial(mcu_port, 115200);
  test_steering(ser)
  test_throttle(ser)


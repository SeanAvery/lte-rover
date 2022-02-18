import serial
import struct
import time
import random

mcu_port = "/dev/cu.usbserial-1120"
throttle_index = "t";
steering_index = "s";
escape_char = "#";
min_steering = 60;
max_steering = 120;
min_throttle = 20;
max_throttle= 160;
msg_delay=0.01

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, escape_char)
  return bytes(str_msg, 'UTF-8')


if __name__ == "__main__":
  ser = serial.Serial(mcu_port, 115200);

  # test steering
  steering_val = 90;
  for x in range(5):
    while steering_val < max_steering:
      time.sleep(0.05)
      steering = format_msg(steering_index, steering_val)
      ser.write(steering)
      steering_val = steering_val + 1
    while steering_val > min_steering:
      time.sleep(0.05)
      steering = format_msg(steering_index, steering_val)
      ser.write(steering)
      steering_val = steering_val - 1

  # test throttle
  # test sending command over the wire
 #  while True:
 #    # read serial port for logs
 #    log = ser.readline().decode('utf8')
 #    print(log)
 #    # update commands
 #    throttle = format_msg(throttle_index, random.randint(min_throttle, max_throttle))
 #    steering = format_msg(steering_index, random.randint(min_steering, max_steering))
 #    time.sleep(0.01)
 #    ser.write(steering)
 #    time.sleep(0.1)
 #    ser.write(throttle)


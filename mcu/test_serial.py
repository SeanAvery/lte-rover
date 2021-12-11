import serial
import struct
import time

mcu_port = "/dev/cu.usbserial-11310"

throttle_index = "t";
steering_index = "s";
escape_char = "#";

min_steering = 60;
max_steering = 120;

min_throttle = 90;
max_throttle=300;

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, escape_char)
  return bytes(str_msg, 'UTF-8')

if __name__ == "__main__":
  ser = serial.Serial(mcu_port, 115200);

  throttle_val = -300;
  steering_val = 90;

  # test sending command over the wire
  while True:
    # read serial port for logs
    # log = ser.readline().decode('utf8')
    # print(log)
    # update commands
    throttle_val += 10
    throttle = format_msg(throttle_index, 110)
    steering_val += 1
    steering = format_msg(steering_index, steering_val)
    time.sleep(0.1)
    # ser.write(steering)
    time.sleep(0.1)
    print(throttle_val)
    ser.write(throttle)


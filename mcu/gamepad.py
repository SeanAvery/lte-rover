import serial
import curses
import time

throttle_index = "t";
steering_index = "s";
escape_char = "#";

steering = 90;
throttle = 90;

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, escape_char)
  return bytes(str_msg, 'UTF-8')

if __name__ == "__main__":
  stdscr = curses.initscr()
  ser = serial.Serial("/dev/cu.usbserial-1130", 115200);
  print("connected to serial port")
  while True:
    key = stdscr.getch()
    if key == ord('w'):
      throttle += 1
    if key == ord('s'):
      throttle -= 1
    if key == ord('d'):
      steering += 1
    if key == ord('a'):
      steering -= 1
    steering_msg = format_msg(steering_index, steering)
    throttle_msg = format_msg(throttle_index, throttle)
    
    time.sleep(0.1)
    ser.write(steering_msg)
    time.sleep(0.1)
    ser.write(throttle_msg)
    

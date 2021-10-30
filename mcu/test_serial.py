import serial
import struct
import time

throttle_index = b"t";
steering_index = b"s";
escape_char = b"#";


min_steering = 60;
max_steering = 120;

min_throttle = 90;
max_throttle=300;

if __name__ == "__main__":
  ser = serial.Serial("/dev/cu.usbserial-1130", 115200);

  throttle = throttle_index + b' 300 ' + escape_char;
  steering = steering_index + b' 70 ' + escape_char;

  # test sending command over the wire
  while True:
    # read serial port for logs
    log = ser.readline().decode('utf8')
    print(log)
    # update commands
    ser.write(b't 300 #')
    time.sleep(0.1)
    ser.write(b's 70 #')
  

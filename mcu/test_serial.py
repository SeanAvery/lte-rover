import serial
import struct
import time

throttle_index = b"t";
steering_index = b"s";
escape_char = b"#";

if __name__ == "__main__":
  ser = serial.Serial("/dev/cu.usbserial-1130", 115200);

  throttle = struct.pack("cIc", throttle_index, 300, escape_char)
  steering = struct.pack("cIc", steering_index, 120, escape_char)

  # test sending command over the wire
  while True:
    ser.write(steering)

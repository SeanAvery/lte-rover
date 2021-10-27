import serial
import struct
import time

throttle_index = b"t";
steering_index = b"s";
escape_char = b"#";

if __name__ == "__main__":
  ser = serial.Serial("/dev/cu.usbserial-1130", 115200);

  throttle = struct.pack("c I c", throttle_index, 20, escape_char)
  # steering = struct.pack("cIc", 90)

  # test sending command over the wire
  while True:
    ser.write(throttle)

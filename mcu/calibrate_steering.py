import serial
import time

center = 90 # center angle
radius = 30 # left/right mobility

if __name__ == "__main__":
  ser = serial.Serial("/dev/cu.usbserial-1130", 115200);
  while True:
    log = ser.readline().decode('utf8')
    print(log)
    ser.write(b's 60 #')
   


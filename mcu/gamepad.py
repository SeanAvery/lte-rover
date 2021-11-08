import keyboard

steering = 90;
throttle = 90;

if __name__ == "__main__":
  while True:
    try:
      if keyboard.is_pressed('w'):
        throttle += 1
      if keyboard.is_pressed('s'):
        throttle -= 1
      if keyboard.is_pressed('d'):
        steering += 1
      if keyboard.is_pressed('a'):
        steering -= 1
    except ValueError as err:
      print("error")

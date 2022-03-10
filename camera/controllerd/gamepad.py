import pygame
import time

if __name__ == "__main__":
  time.sleep(2)
  pygame.joystick.init()
  time.sleep(2)
  controller = pygame.joystick.Joystick(0)
  # controller.init()


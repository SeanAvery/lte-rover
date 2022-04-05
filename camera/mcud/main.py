import cereal.messaging as messaging
import time

sm = messaging.SubMaster(['sensorEvents'])

while 1:
  sm.update()
  print(sm['sensorEvents'])

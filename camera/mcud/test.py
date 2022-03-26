import time
import cereal.messaging as messaging

pm = messaging.PubMaster(['controls'])

def format_msg(prefix, value):
  str_msg = "{}{:05d}{}".format(prefix, value, "#")
  return bytes(str_msg, 'UTF-8')

# steering tests
min_steering = 70
max_steering = 110
def test_steering():
  steering_val = 90
  for x in range(5):
    while steering_val < max_steering:
      time.sleep(0.05)
      steering_val = steering_val + 1
      msg = format_msg("s", steering_val)
      print(msg)
      pm.send('controls', msg)
    while steering_val > min_steering:
      time.sleep(0.05)
      steering_val = steering_val - 1
      msg = format_msg("s", steering_val)
      print(msg)
      pm.send('controls', msg)

test_steering()

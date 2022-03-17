import os
import sys

import cereal.messaging as messaging

if __name__ == "__main__":
  sm = messaging.SubMaster(["roadCameraState"])
  while 1:
    sm.update()
    if sm.updated["roadCameraState"]:
      bgr_raw = sm["roadCameraState"].image
      print("updated")

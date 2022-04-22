import os
import sys

import cereal.messaging as messaging
from cereal.visionipc.visionipc_pyx import VisionIpcClient, VisionStreamType

file_io = "/data/lte-rover/vid-io"

# os.environ["ZMQ"] = "1"

if __name__ == "__main__":
  # file io

  # mkfifo file 
  if not os.path.exists(file_io):
    print("creating mkfifo pipe file")
    os.mkfifo(file_io)

  print("opening file")
  # f = open(file_io, "wb") # read/write
  print("loop")

  # sub socket
  sm = messaging.SubMaster(["roadCameraState"])

  # visionipc
  print(VisionStreamType.VISION_STREAM_RGB_BACK)
  vipc_client = VisionIpcClient("camerad", VisionStreamType.VISION_STREAM_RGB_BACK, True)

  vipc_client.connect(True)

  while 1:
    sm.update()
    if sm.updated["roadCameraState"]:
      print("updated")
      bgr_raw = sm["roadCameraState"].image
      print(len(bgr_raw))
      f.write(bgr_raw)
      f.flush()

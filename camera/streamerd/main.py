import os
import sys
import numpy as np

import cereal.messaging as messaging
from cereal.visionipc.visionipc_pyx import VisionIpcClient, VisionStreamType

file_io = "/data/lte-rover/vid-io"

# os.environ["ZMQ"] = "1"

def extract_image(buf, w, h, stride):
  img = np.hstack([buf[i * stride:i * stride + 3 * w] for i in range(h)])
  b = img[::3].reshape(h, w)
  g = img[1::3].reshape(h, w)
  r = img[2::3].reshape(h, w)
  return np.dstack([b, g, r])

if __name__ == "__main__":
  # file io

  # mkfifo file
  # if not os.path.exists(file_io):
    # print("creating mkfifo pipe file")
    # os.mkfifo(file_io)

  print("opening file")
  # f = open(file_io, "wb") # read/write
  print("loop")

  # sub socket
  sm = messaging.SubMaster(["wideRoadCameraState"])

  # visionipc
  print(VisionStreamType.VISION_STREAM_RGB_BACK)
  vipc_client = VisionIpcClient("camerad", VisionStreamType.VISION_STREAM_RGB_BACK, True)

  vipc_client.connect(True)
  # exit()

  while 1:
    sm.update()
    if sm.updated["wideRoadCameraState"]:
      print("updated")
      bgr_raw = sm["wideRoadCameraState"].image
      print(len(bgr_raw))

      # frame = vipc_client.recv()
      print(len(frame))
      # print("width:", vipc_client.width)
      # print("height:", vipc_client.height)
      # print("stride:", vipc_client.stride)

      # f.write(extract_image(frame, vipc_client.width, vipc_client.height, vipc_client.stride))
      # f.write(frame)
      # f.flush()

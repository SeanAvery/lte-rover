#include <iostream>
#include <unistd.h>
#include "messaging/messaging.h"
#include "encoder/omx_encoder.cpp"
#include "visionipc/visionipc.h"
#include "visionipc/visionipc_client.h"
// #include <gst/gst.h>

void encoder_thread()
{
  // VisionIpcClient vipc_client = VisionIpcClient("camerad", VISION_STREAM_RGB_WIDE, false);
}

int main()
{
  std::cout << "streamerd" << std::endl;

  // setup omx encoder
  auto encoder = new OmxEncoder();
  const char* path = "/tmp/rtp_bridge";
  encoder->encoder_open(path);

  // encoder_thread();
  // camera stream subscriber
  Context *context = Context::create();
  SubSocket *subscriber = SubSocket::create(context, "roadCameraState");
  subscriber->setTimeout(100);

  while (true)
  {
    usleep(100000);
    Message *msg = subscriber->receive();
    std::cout << "received msg" << std::endl;

    auto data = msg->getData();

    std::cout << "data: " << msg->getSize() << std::endl;

    // parse image buffer
  }
  std::cout << "yoooo" << std::endl;
}

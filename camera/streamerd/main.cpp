#include <iostream>
#include <unistd.h>
#include "messaging/messaging.h"
#include "encoder/omx_encoder.cpp"
// #include <gst/gst.h>

int main()
{
  std::cout << "streamerd" << std::endl;
  // camera stream subscriber
  Context *context = Context::create();
  SubSocket *subscriber = SubSocket::create(context, "roadCameraState");
  subscriber->setTimeout(100);

  while (true)
  {
    usleep(100000);
    Message *msg = subscriber->receive();
  }
  std::cout << "yoooo" << std::endl;
}

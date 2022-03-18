#include <iostream>
#include <unistd.h>
#include "messaging/messaging.h"

// #include <gst/gst.h>

int main()
{
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

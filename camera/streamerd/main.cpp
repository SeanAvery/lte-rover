#include <iostream>
#include "messaging/messaging.h"

// #include <gst/gst.h>

int main()
{

  // camera stream subscriber
  Context *context = Context::create();
  SubSocket *subscriber = SubSocket::create(context, "roadCameraState");
  subscriber->setTimeout(100);
  std::cout << "yoooo" << std::endl;
}

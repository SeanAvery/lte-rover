#include <iostream>
#include <sstream>
#include <unistd.h>
#include <memory>
#include "usb/CH340G.cpp"
#include "messaging/messaging.h"

#define MCUD_FREQUENCY 10000 // 100 hertz

int main()
{
  int err;
  // init usb
  CH340 mcu;
  err = mcu.init();
  err = mcu.init_ch340();
  err = mcu.set_baud_rate();
  err = mcu.handshake();

  // wait for usb initialization
  usleep(2000);

  // init subscriber
  Context *context = Context::create();
  SubSocket * subscriber = SubSocket::create(context, "controls");
  subscriber->setTimeout(100);

  // poll sub socket
  while(1)
  {
    usleep(MCUD_FREQUENCY);
    // receive msg from subsocket
    Message *controller_msg = subscriber->receive();

    if (controller_msg == nullptr)
    {
      std::cout << "no message here" << std::endl;
      continue;
    }
    // parse message
    char * raw_command = controller_msg->getData();
    std::cout << "raw_command: " << raw_command << std::endl;
    unsigned char * command = reinterpret_cast<unsigned char *>(raw_command);
    // send command to mcu
    mcu.bulk_write(2, command, 7, 10);
  }

  return 0;
}


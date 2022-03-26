#include <iostream>
#include <unistd.h>
#include <memory>
#include "mcu.cpp"
#include "messaging/messaging.h"

int main()
{
  // init usb
  Mcu mcu;
  mcu.init();
  usleep(2000);

  // init subscriber
  Context *context = Context::create();
  SubSocket * subscriber = SubSocket::create(context, "controls");
  subscriber->setTimeout(100);


  // create message
  unsigned char ret_msg[100];

  while(true)
  {
    // dev handle 2
    usleep(10000);
    // receive msg from subsocket
    Message *controller_msg = subscriber->receive();

    if (controller_msg == nullptr)
    {
      std::cout << "no message here" << std::endl;
      continue;
    }
    char * raw_command = controller_msg->getData();
    unsigned char * command = reinterpret_cast<unsigned char *>(raw_command);
    mcu.usb_bulk_write(2, command, sizeof(command), 10);
    usleep(100);
    mcu.usb_bulk_read(130, ret_msg, 100, 100);
    std::string mytext(reinterpret_cast<char*>(ret_msg));
    std::cout << "return message: " << mytext << std::endl;
  }

  return 0;
}


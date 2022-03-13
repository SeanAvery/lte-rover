#include <iostream>
#include <unistd.h>
#include <memory>
#include "mcu.cpp"
#include "messaging/messaging.h"

Mcu *usb_connect()
{
  std::unique_ptr<Mcu> mcu;
  try
  {
    mcu = std::make_unique<Mcu>();
    std::string serial_port = "/temp";
    mcu->init(serial_port);
  }
  catch (std::exception &e)
  {
    std::cout << "exception in Mcu init" << std::endl;
    return nullptr;
  }
}

int main()
{
  // init usb
  Mcu mcu;
  std::string temp = "yooo";
  mcu.init(temp);

  // init subscribe
  Context *context = Context::create();
  SubSocket * subscriber = SubSocket::create(context, "controls");
  subscriber->setTimeout(100);


  // create message
  unsigned char msg[] = "s00110#";
  unsigned char ret_msg[100];

  while(true)
  {
    // dev handle 2
    usleep(10000);
    // receive msg from subsocket
    Message *controller_msg = subscriber->receive();
    std::cout << msg << std::endl;
    mcu.usb_bulk_write(2, msg, 8, 0);
    usleep(10000);
    mcu.usb_bulk_read(130, ret_msg, 100, 100);
    std::string mytext(reinterpret_cast<char*>(ret_msg));
    std::cout << "return message: " << mytext << std::endl;
  }

  return 0;
}


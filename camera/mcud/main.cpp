#include <iostream>
#include <sstream>
#include <unistd.h>
#include <memory>
#include "usb/CH340G.cpp"
#include "messaging/messaging.h"

int send(CH340 mcu, std::string msg)
{
  unsigned char *packet = (unsigned char *)msg.c_str();
  mcu.bulk_write(EP_DATA_OUT, packet, 7, 1000);
  return 0;
}

std::string receive(CH340 &mcu)
{
  std::string r;
  r.reserve(0x1000 + 0x40);
  unsigned char dat[0x40];
  while (r.length() < 0x1000) {
    // int len = mcu.usb_read(225, 1, 0, dat, sizeof(dat), 0);
    int len = mcu.bulk_read(EP_DATA_IN, dat, sizeof(dat), 0);
    if (len <= 0) break;
    r.append((char*)dat, len);
  }
  return r;
}

std::string format_msg(int val)
{
  std::stringstream ss;
  ss << "s000" << val << "#";
  return ss.str();
}

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

  // create message
  unsigned char ret_msg[1024];

  // poll sub socket
  while(true)
  {
    // dev handle 2
    usleep(100);
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
    usleep(100);
    // read mcu messages
    // mcu.usb_bulk_read(130, ret_msg, 100, 100);
    // std::string mytext(reinterpret_cast<char*>(ret_msg));
    // std::cout << "return message: " << mytext << std::endl;
  }

  return 0;
}


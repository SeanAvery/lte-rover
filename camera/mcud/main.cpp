#include <iostream>
#include <unistd.h>
#include <memory>
#include "usb/CH340G.cpp"
#include "messaging/messaging.h"

int send(CH340 mcu, std::string msg)
{

  int len = msg.length();
  const char *dat = msg.data();
  unsigned char a[0x20+1];
  a[0] = 1;
  for (int i=0; i<len; i+=0x20)
  {
    int ll = std::min(0x20, len-i);
    memcpy(&a[1], &dat[i], ll);
    mcu.bulk_write(2, a, ll+1, 100);
  }
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

int main()
{
  int err;
  // init usb
  CH340 mcu;
  err = mcu.init();
  err = mcu.init_ch340();
  err = mcu.set_baud_rate();
  err = mcu.handshake();
  // err = mcu.set_baud_rate();
  // std::string retmsg = receive(mcu);
  // std::cout << "return msg: " << retmsg << std::endl;
  // send(mcu, "s00110#");

  // test write
  std::cout << "write endpoint: " << EP_DATA_OUT << std::endl;
  unsigned char test_msg[]  = "s00090#";
  err = mcu.bulk_write(EP_DATA_OUT, test_msg, sizeof(test_msg), 1000);
  exit(0);
  // test write
  unsigned char ret_msg[1024];
  // read endpoint 130
  // 131
  std::cout << "read endpoint:" << EP_DATA_IN << std::endl;
  std::cout << "control endpoint" << CTRL_IN << std::endl;
  err = mcu.bulk_read(EP_DATA_IN, ret_msg, 1024, 1000);
  std::cout << "return message: " << ret_msg << std::endl;

  // set baudrate
  // mcu.usb_write(0xe4, 1, 51200, 0);
  exit(0);

  // wait for usb initialization
  usleep(2000);

  // init subscriber
  Context *context = Context::create();
  SubSocket * subscriber = SubSocket::create(context, "controls");
  subscriber->setTimeout(100);

  // create message
  // mcu.usb_bulk_read(130, ret_msg, 100, 100);
  // std::string mytext(reinterpret_cast<char*>(ret_msg));
  // std::cout << "return message: " << mytext << std::endl;
  exit(0);

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
    // mcu.usb_bulk_write(2, command, sizeof(command), 10);
    usleep(100);
    // read mcu messages
    // mcu.usb_bulk_read(130, ret_msg, 100, 100);
    // std::string mytext(reinterpret_cast<char*>(ret_msg));
    // std::cout << "return message: " << mytext << std::endl;
  }

  return 0;
}


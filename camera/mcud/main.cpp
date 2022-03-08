#include <iostream>
#include <unistd.h>
#include <memory>
#include "mcu.cpp"

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
  std::cout << "setting up mcu daemon" << std::endl;
  // Mcu *mcu = usb_connect();
  Mcu mcu;
  std::string temp = "yooo";
  mcu.init(temp);

  // create message
  unsigned char msg[] = "s00070#";
  std::cout << "msg size: " << sizeof(msg) << msg[0] << msg[6] << msg[7] << std::endl;
  // unsigned char msg[8] = "s00080#";
  // strcpy(static_cast <char*>(msg),"s00080#")
  // mcu.usb_bulk_write(1, msg, 8, 100);
  // mcu.usb_bulk_write(2, msg, 8, 5);
  // exit(0);


  std::cout << "libusb in endpoint" << LIBUSB_ENDPOINT_IN << std::endl;

  std::cout << "libusb out endpoint" << LIBUSB_ENDPOINT_OUT << std::endl;
  unsigned char ret_msg[100];
  // mcu.usb_bulk_read(130, ret_msg, 100, 100);
  // std::string str = reinterpret_cast<char*>(ret_msg);
  // std::cout << "return msg: " << str << ret_msg[0] << std::endl;
  // exit(0);
  while(true)
  {
    // dev handle 2
    // bRequest
    usleep(10000);
    std::cout << msg << std::endl;
    mcu.usb_bulk_write(2, msg, 8, 0);
    usleep(10000);
    mcu.usb_bulk_read(130, ret_msg, 100, 100);
    std::string mytext(reinterpret_cast<char*>(ret_msg));
    std::cout << "return message: " << mytext << std::endl;
  }

  return 0;
}


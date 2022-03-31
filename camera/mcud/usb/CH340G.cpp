#include "CH340G.h"

int CH340::init()
{
  int err;
  ssize_t num_devices;
  libusb_device **dev_list = NULL;

  err = libusb_init(&ctx);
  if (err != 0)
  {
    std::cout << "failed to init usb context" << std::endl;
    return err;
  }
  
  // find mcu device
  num_devices = libusb_get_device_list(ctx, &dev_list);

  if (num_devices < 0)
  {
    std::cout << "did not find any usb devices" << std::endl;
    return 1;
  }

  // find mcu device
  num_devices = libusb_get_device_list(ctx, &dev_list);
  if (num_devices < 0)
  {
    std::cout << "did not find any usb devices" << std::endl;
    return 1;
  }

  for (size_t i = 0; i < num_devices; i++)
  {
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev_list[i], &desc);
    if (desc.idVendor == VENDOR && desc.idProduct == PRODUCT)
    {
      std::cout << "found mcu usb connection. device: " << i << std::endl;
      libusb_open(dev_list[i], &dev_handle);
      if (dev_handle == NULL)
      {
        return 1;
      }
    }
  }
  return 0;
}

int CH340::setBaudRate()
{

  // TODO: check if dev handle is defined
  // TODO: check if usb open
  int err;
  static int baud[] = {2400, 0xd901, 0x0038, 4800, 0x6402,
                0x001f, 9600, 0xb202, 0x0013, 19200, 0xd902, 0x000d, 38400,
                            0x6403, 0x000a, 115200, 0xcc03, 0x0008};
  for (int i = 0; i < sizeof(baud)/sizeof(int) / 3; i++)
  {
    if (baud[i * 3] == BAUDRATE)
    {
      std::cout << "hit it" << std::endl;
      err = libusb_control_transfer(dev_handle, CTRL_OUT, 0x9a, 0x1312, baud[i * 3 + 1], NULL, 0, 1000);

      if (err < 0)
      {
        std::cout << "could not set baudrate step 1" << err << std::endl;
        return err;
      }

      err = libusb_control_transfer(dev_handle, CTRL_OUT, 0x9a, 0x0f2c, baud[i * 3 + 2], NULL, 0, 1000);
      if (err < 0)
      {
        std::cout << "could not set baudrate step 2 " << err << std::endl;

      }
    }
  }
  return 0;
}

int CH340::handshake()
{
  int err;
  // 1
  err = libusb_control_transfer(dev_handle, CTRL_OUT, 0xa1, 0, 0, NULL, 0, 1000);
  if (err < 0)
  {
    std::cout << "handshake step 1 failed" << std::endl;
    return err;
  }
  
  // 2
  err = libusb_control_transfer(dev_handle, CTRL_OUT, 0x9a, 0x2518, 0x0050, NULL, 0, 1000);
  if (err < 0)
  {
    std::cout << "handshake step 2 failed" << std::endl;
    return err;
  }

  // 3
  err = libusb_control_transfer(dev_handle, CTRL_OUT, 0xa1, 0x501f, 0xd90a, NULL, 0, 1000);
  if (err < 0)
  {
    std::cout << "handshake step 3 failed" << std::endl;
    return err;
  }


  // 4  
  err = libusb_control_transfer(dev_handle, CTRL_OUT, 0xa4, ~((dtr ? 1 << 5 : 0) | (rts ? 1 << 6 : 0)), 0, NULL, 0, 1000);

  if (err < 0)
  {
    std::cout << "could not perform handshake " << err << std::endl;
  }
  return 0;
}

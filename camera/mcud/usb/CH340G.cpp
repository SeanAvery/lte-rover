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

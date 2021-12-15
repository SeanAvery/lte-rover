#pragma once
#include "mcu.h"

#include <iostream>
#include <iterator>
#include <assert.h>

int Mcu::init(std::string serial)
{
  // init libusb
  std::cout << "initializing usb connection" << std::endl;
  ssize_t num_devices;
  libusb_device **dev_list = NULL;
  int err = init_usb_context(&ctx);
  if (err != 0)
  {
    std::cout << "failed to init usb context" << std::endl;
    return err;
  }

  // set libusb debug level
  libusb_set_debug(ctx, 3);

  // find mcu device
  num_devices = libusb_get_device_list(ctx, &dev_list);
  if (num_devices < 0)
  {
    std::cout << "did not find any usb devices" << std::endl;
    return 1;
  }
  std::cout << "num_devices: " << num_devices << std::endl;
  for (size_t i = 0; i < num_devices; i++)
  {
    libusb_device_descriptor desc;
    libusb_get_device_descriptor(dev_list[i], &desc);
    if (desc.idVendor == 6790 && desc.idProduct == 29987)
    {
      std::cout << "found mcu usb connection. device: " << i << std::endl;
      libusb_open(dev_list[i], &dev_handle);
      if (dev_handle == NULL)
      {
        std::cout << "dev_handle not found" << std::endl;
        return 1;
      }
      unsigned char desc_serial[100] = { 0 };
      // int ret = libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, desc_serial, std::size(desc_serial));
      // int ret = libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, desc_serial, std::size(desc_serial));
      // if (ret < 0)
      // {
      //   std::cout << "could not fetch serial number" << std::endl;
      // }
      // usb_serial = std::string((char *)desc_serial, ret).c_str();
      // std::cout << "serial number: " << usb_serial << std::endl;
      // libusb_close(dev_handle);
    }
  }
  // libusb_free_device_list(dev_list, 1);

  if (libusb_kernel_driver_active(dev_handle, 0) == 1)
  {
    std::cout << "kernel driver active" << std::endl;
    if (libusb_detach_kernel_driver(dev_handle, 0) == 0)
    {
      std::cout << "kernel driver detatched" << std::endl;
    }
  }
  err = libusb_set_configuration(dev_handle, 1);
  if (err != 0)
  {
    std::cout << "could not set libusb configuration" << std::endl;
    return 1;
  }
  return 0;
}

int Mcu::init_usb_context(libusb_context **context)
{
  assert(context != nullptr);

  int err = libusb_init(context);
  if (err != 0)
  {
     std::cout << "libusb initialization error" << std::endl;
     return err;
  }
  libusb_set_debug(*context, 3);
  return 0;
}

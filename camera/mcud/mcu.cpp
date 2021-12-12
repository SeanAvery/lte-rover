#pragma once
#include "mcu.h"

#include <iostream>
#include <assert.h>

int Mcu::init()
{
  ssize_t num_devices;
  libusb_device **dev_list = NULL;
  int err = init_usb_context(&ctx);
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
#pragma once
#include "mcu.h"

#include <iostream>
#include <iterator>
#include <mutex>
#include <assert.h>

#define ACM_CTRL_DTR   0x01
#define ACM_CTRL_RTS   0x02

static int ep_in_addr = 0x83;
static int ep_out_addr = 0x02;

int Mcu::init()
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

      std::cout << "dev_handle: " << &dev_handle << std::endl;
      // libusb_close(dev_handle);
      unsigned char desc_serial[26] = { 0 };
      int ret = libusb_get_string_descriptor_ascii(dev_handle, desc.iSerialNumber, desc_serial, std::size(desc_serial));

    }
  }
  // free device list
  libusb_free_device_list(dev_list, 1);
  dev_list = nullptr;

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
    return err;
  }
  err = libusb_claim_interface(dev_handle, 0);
  if (err != 0)
  {
    std::cout << "could not claim libusb interface" << std::endl;
    return err;
  }

  // find device endpoints
  
  // CDC-ACM configuration
  err = libusb_control_transfer(dev_handle, 0x21, 0x22, ACM_CTRL_DTR | ACM_CTRL_RTS, 0, NULL, 0, 0);
  if (err < 0)
  {
    std::cout << "could not set line state " << libusb_error_name(err) << std::endl;
  }

  unsigned char encoding[] = { 0x80, 0x25, 0x00, 0x00, 0x00, 0x00, 0x08 };
  
  err = libusb_control_transfer(dev_handle, 0x21, 0x20, 0, 0, encoding, sizeof(encoding), 0);
  if (err < 0)
  {
    std::cout << "could not set encoding" << std::endl;
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
  // libusb_set_debug(*context, 6);
  return 0;
}


int Mcu::usb_read(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout){
  int err;
  const uint8_t bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_DEVICE;
  // TODO: check if connected
  std::lock_guard lk(usb_lock);
  do
  {
    err = libusb_control_transfer(dev_handle, bmRequestType, bRequest, wValue, wIndex, data, wLength, timeout);
  }
  while (err < 0);
}

int usb_write(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned int timeout)
{
  return 0;
}


int Mcu::usb_bulk_read(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout)
{
  int err;
  int transferred = 0;
  std::lock_guard lk(usb_lock);
  // TODO: check if connected
  err = libusb_bulk_transfer(dev_handle, endpoint, data, length, &transferred, timeout);
  if (err != 0)
  {
    std::cout << "bulk read error: " << err << std::endl;
    if (err == LIBUSB_ERROR_TIMEOUT)
    {
      std::cout << "read transfer timed out" << std::endl;
    }
    if (err == LIBUSB_ERROR_PIPE)
    {
      std::cout << "read transfer endpoint halted" << std::endl;
    }
    if (err == LIBUSB_ERROR_OVERFLOW)
    {
      std::cout << "read transfer error. device offered too much data." << std::endl;
    }
    if (err == LIBUSB_ERROR_NO_DEVICE)
    {
      std::cout << "device has disconnected" << std::endl;
    }
    if (err == LIBUSB_ERROR_BUSY)
    {
      std::cout << "device is busy" << std::endl;
    }
    if (err == LIBUSB_ERROR_INVALID_PARAM)
    {
      std::cout << "transfer size limitation" << std::endl;
    }
  }
  return err;
}

int Mcu::usb_bulk_write(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout)
{
  std::lock_guard lock(usb_lock);
  int transferred = 0;
  int err = libusb_bulk_transfer(dev_handle, endpoint, data, length, &transferred, timeout);
  if (err != 0)
  {
    std::cout << "error in bulk write: " << err << std::endl;

    if (err == LIBUSB_ERROR_TIMEOUT)
    {
      std::cout << "write timed out" << std::endl;

    }
    else if (err == LIBUSB_ERROR_PIPE)
    {
      std::cout << "endpoint halted" << std::endl;
    }
    else if (err == LIBUSB_ERROR_OVERFLOW)
    {
      std::cout << "packet overflow" << std::endl;
    }
    else if (err == LIBUSB_ERROR_NO_DEVICE)
    {
      std::cout << "device is disconnected" << std::endl;
    }
    else if (err == LIBUSB_ERROR_BUSY)
    {
      std::cout << "device busy" << std::endl;
    }
    else if (err == LIBUSB_ERROR_INVALID_PARAM)
    {
      std::cout << "invalid param" << std::endl;
    }
  }
  return 0;
}


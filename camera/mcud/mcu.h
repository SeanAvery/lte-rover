#pragma once

#include "include/libusb.h"
#include <string>

#define TIMEOUT 0

class Mcu
{
  private:
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    std::mutex usb_lock;
    int init_usb_context(libusb_context **context);
    std::string usb_serial;
  public:
    int init(std::string serial);
    
    // communications
    int usb_write(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned int timeout);
    int usb_read(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
    int usb_bulk_read(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout=TIMEOUT);
    int usb_bulk_write(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout=TIMEOUT);
    
    // mcu functionality
};


#pragma once

#include "include/libusb.h"
#include <string>
#include <mutex>

#define TIMEOUT 0

#define EP_DATA_IN        (0x2|LIBUSB_ENDPOINT_IN)
#define EP_DATA_OUT       (0x2|LIBUSB_ENDPOINT_OUT)
#define CTRL_IN           (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT          (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define DEFAULT_BAUD_RATE 9600

class Mcu
{
  private:
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    std::mutex usb_lock;
    int init_usb_context(libusb_context **context);
    std::string usb_serial;
  public:
    int init();
    
    // communications
    int usb_write(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned int timeout);
    int usb_read(uint8_t bRequest, uint16_t wValue, uint16_t wIndex, unsigned char *data, uint16_t wLength, unsigned int timeout);
    int usb_bulk_read(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout=TIMEOUT);
    int usb_bulk_write(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout=TIMEOUT);
    
    // mcu functionality
};


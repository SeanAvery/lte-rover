#include <libusb-1.0/libusb.h>

class Mcu
{
  private:
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    int init_usb_context(libusb_context **context);
  public:
    int init();
};
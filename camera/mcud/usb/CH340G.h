#include "../include/libusb.h"


#define VENDOR 6790
#define PRODUCT 29987

#define EP_DATA_IN        (0x2|LIBUSB_ENDPOINT_IN)
#define EP_DATA_OUT       (0x2|LIBUSB_ENDPOINT_OUT)
#define CTRL_IN           (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT          (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define DEFAULT_BAUD_RATE 9600

class CH340
{
  private:
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;

  public:
    int init();
    int setBaudRate(int baudRate);
};

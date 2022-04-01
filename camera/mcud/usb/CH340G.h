#include "../include/libusb.h"


#define VENDOR 6790
#define PRODUCT 29987
#define BAUDRATE 9600
// #define BAUDRATE 115200

#define EP_DATA_IN        (0x2|LIBUSB_ENDPOINT_IN)
#define EP_DATA_OUT       (0x2|LIBUSB_ENDPOINT_OUT)
#define CTRL_IN           (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_IN)
#define CTRL_OUT          (LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_ENDPOINT_OUT)
#define DEFAULT_BAUD_RATE 9600

uint8_t dtr = 0;
uint8_t rts = 0;

class CH340
{
  private:
    libusb_context *ctx = NULL;
    libusb_device_handle *dev_handle = NULL;
    libusb_transfer *recv_bulk_transfer = NULL;

  public:
    int init();
    int init_ch340();
    int set_baud_rate();
    int handshake();
    int bulk_write();
    int bulk_read(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout);
    int bulk_write(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout);
    int async_bulk_read(unsigned char endpoint, unsigned char* data, int length, unsigned int timeout);
};

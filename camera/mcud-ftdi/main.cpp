#include <iostream>
#include "ftdi.h"

#define TXD 0x01  
#define RXD 0x02  
#define RTS 0x04  
#define CTS 0x08  
#define DTR 0x10
#define DSR 0x20
#define DCD 0x40
#define RI  0x80

int main()
{
  std::cout << "yooo" << std::endl;

  int ret;
  struct ftdi_context *ftdi;
  if ((ftdi = ftdi_new()) == 0)
  {
    fprintf(stderr, "ftdi_new failed\n");
    return EXIT_FAILURE;
  }

  struct ftdi_version_info version;

  version = ftdi_get_library_version();
  printf("Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
        version.version_str, version.major, version.minor, version.micro,
        version.snapshot_str);

  if ((ret = ftdi_usb_open(ftdi, 0x1a86, 0x7523)) < 0)
  {
    fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    return EXIT_FAILURE;
  }
}

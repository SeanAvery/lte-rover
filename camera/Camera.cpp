#include "Camera.h"

#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> 

int Camera::Init()
{
  // open subdevice files
  csi_fd = open(params::CSID_SUBSYSTEM, O_RDWR | O_NONBLOCK);
  csiphy_fd= open(params::CSIPHY_SUBSYSTEM, O_RDWR | O_NONBLOCK);

  sensor_fd= open(params::SENSOR_SUBSYSTEM, O_RDWR | O_NONBLOCK);

  actuator_fd = open(params::ACTUATOR_SUBSYSTEM, O_RDWR | O_NONBLOCK);

  // start camera stream
  // csid init
  
  
  return 0;
}

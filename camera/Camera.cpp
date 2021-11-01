#include "Camera.h"
#include "utils.h"

#include "include/msm_cam_sensor.h"
#include "include/msmb_camera.h"
#include "include/msmb_isp.h"
#include "include/msmb_ispif.h"

#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> 

int Camera::Init()
{
  // open subdevice files
  csid_fd = open(params::CSID_SUBSYSTEM, O_RDWR | O_NONBLOCK);
  csiphy_fd= open(params::CSIPHY_SUBSYSTEM, O_RDWR | O_NONBLOCK);
  sensor_fd= open(params::SENSOR_SUBSYSTEM, O_RDWR | O_NONBLOCK);
  actuator_fd = open(params::ACTUATOR_SUBSYSTEM, O_RDWR | O_NONBLOCK);

  struct csid_cfg_data csid_cfg_data = {};

  // shutdown camera stream
  // 1. release csiphy
  struct msm_camera_csi_lane_params csi_lane_params = {0};
  csi_lane_params.csi_lane_mask = 0x1f;
  csiphy_cfg_data csiphy_cfg_data = { .cfg.csi_lane_params = &csi_lane_params, .cfgtype = CSIPHY_RELEASE};
  int err = cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "release csiphy");

  // start camera stream
  // csid init
  csid_cfg_data.cfgtype = CSID_INIT;
  // cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "init csid");
  
  return 0;
}

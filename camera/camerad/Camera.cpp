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

int Camera::camera_init()
{
  // open subdevice files
  csid_fd = HANDLE_EINTR(open(params::CSID_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  csiphy_fd= HANDLE_EINTR(open(params::CSIPHY_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  isp_fd= HANDLE_EINTR(open(params::SENSOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  actuator_fd = HANDLE_EINTR(open(params::ACTUATOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));

  struct csid_cfg_data csid_cfg_data = {};
  struct msm_actuator_cfg_data actuator_cfg_data = {};

  // wait for camera sensor
  
  for (int i = 0; i < 10; i++)
  {
    sensor_fd = HANDLE_EINTR(open(params::SENSOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));
    if (sensor_fd >= 0)
    {
      break;
    }
    std::cout << "waiting for camera sensor..." << std::endl;
    sleep_for(1000);
  }

  // shutdown camera stream
  // release csiphy
  struct msm_camera_csi_lane_params csi_lane_params = {0};
  csi_lane_params.csi_lane_mask = 0x1f;
  csiphy_cfg_data csiphy_cfg_data = { .cfg.csi_lane_params = &csi_lane_params, .cfgtype = CSIPHY_RELEASE};
  int err = cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "release csiphy");

  // release csid
  csid_cfg_data.cfgtype = CSID_RELEASE;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "release csid");

  // power down camera sensor
  struct sensorb_cfg_data sensorb_cfg_data = {.cfgtype = CFG_POWER_DOWN};
  cam_ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg_data, "sensor power down");
  
  // actuator powerdown
  actuator_cfg_data.cfgtype = CFG_ACTUATOR_POWERDOWN;
  cam_ioctl(actuator_fd, VIDIOC_MSM_ACTUATOR_CFG, &actuator_cfg_data, "actuator powerdown");

  // start camera stream
  // csid init
  csid_cfg_data.cfgtype = CSID_INIT;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "init csid");

  // csiphy init
  csiphy_cfg_data = {.cfgtype = CSIPHY_INIT};
  cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "init csiphy");
}

void Camera::camera_open()
{
  // msm_fd = HANDLE_EINTR(open(params::MSM_SUBSYTEM, O_RDWR | O_NONBLOCK));
  // v4l_fd = HANDLE_EINTR(open(params::V4L_SUBSYTEM, O_RDWR | O_NONBLOCK));
  // v4l_fd = HANDLE_EINTR(open(params::ISPIF_SUBSYTEM, O_RDWR | O_NONBLOCK));
}

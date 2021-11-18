#include "Camera.h"

// #include "include/msm_camsensor_sdk.h"
// #include "sensor_i2c.h"
// #include "include/msm_cam_sensor.h"
// #include "include/msmb_camera.h"
// #include "include/msmb_isp.h"
// #include "include/msmb_ispif.h"

#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h> 
#include <iterator>
#include <assert.h>

void Camera::camera_init()
{
  msm_fd = HANDLE_EINTR(open(params::MSM_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  v4l_fd = HANDLE_EINTR(open(params::V4L_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  v4l_fd = HANDLE_EINTR(open(params::ISPIF_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  
  // open subdevice files
  csid_fd = HANDLE_EINTR(open(params::CSID_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  assert(csid_fd >= 0);
  csiphy_fd= HANDLE_EINTR(open(params::CSIPHY_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  isp_fd= HANDLE_EINTR(open(params::SENSOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  actuator_fd = HANDLE_EINTR(open(params::ACTUATOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));

  // struct csid_cfg_data csid_cfg_data = {};
  struct msm_actuator_cfg_data actuator_cfg_data = {};

  // sensor init
  sensorinit_fd =  HANDLE_EINTR(open(params::SENSORINIT_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  
  // slave info
  msm_camera_sensor_slave_info info = {
    .sensor_name = "ov8865_sunny",
    .eeprom_name = "ov8865_plus",
    .actuator_name = "",
    .ois_name = "",
    .flash_name = "",
    .camera_id = CAMERA_2,
    .slave_addr = 108,
    .i2c_freq_mode = I2C_FAST_MODE,
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .sensor_id_info = {.sensor_id_reg_addr = 12299, .sensor_id = 34917, .module_id = 2},
    .power_setting_array = {
      .power_setting_a = {
        {.seq_type = SENSOR_GPIO, .delay = 5},
        {.seq_type = SENSOR_VREG, .seq_val = 1},
        {.seq_type = SENSOR_VREG, .seq_val = 2},
        {.seq_type = SENSOR_VREG},
        {.seq_type = SENSOR_CLK, .config_val = 24000000, .delay = 1},
        {.seq_type = SENSOR_GPIO, .config_val = 2, .delay = 1},
      },
      .size = 6,
      .power_down_setting_a = {
        {.seq_type = SENSOR_GPIO, .delay = 5},
        {.seq_type = SENSOR_CLK, .delay = 1},
        {.seq_type = SENSOR_VREG},
        {.seq_type = SENSOR_VREG, .seq_val = 1},
        {.seq_type = SENSOR_VREG, .seq_val = 2, .delay = 1},
      },
      .size_down = 5,
    },
    .is_init_params_valid = 0,
    .sensor_init_params = {.modes_supported = 1, .position = FRONT_CAMERA_B, .sensor_mount_angle = 270},
    .output_format = MSM_SENSOR_BAYER,
  };
  info.power_setting_array.power_setting = &info.power_setting_array.power_setting_a[0];
  info.power_setting_array.power_down_setting = &info.power_setting_array.power_down_setting_a[0];
  sensor_init_cfg_data sensor_init_cfg = {.cfgtype = CFG_SINIT_PROBE, .cfg.setting = &info};

  int sensorinit_err = cam_ioctl(sensorinit_fd, VIDIOC_MSM_SENSOR_INIT_CFG, &sensor_init_cfg, "sensor init cfg");
  assert(sensorinit_err >= 0);

  // wait for camera sensor
  std::cout << "init sensor fd" << std::endl;
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
  assert(sensor_fd >= 0);

  // shutdown camera stream
  // release csiphy
  struct msm_camera_csi_lane_params csi_lane_params = {0};
  csi_lane_params.csi_lane_mask = 0x1f;
  csiphy_cfg_data csiphy_cfg_data = { .cfgtype = CSIPHY_RELEASE, .cfg.csi_lane_params = &csi_lane_params}; 
  int err = cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "release csiphy");
 

  // release csid
  // csid_cfg_data csid_cfg_data = { .cfgtype = CSID_RELEASE };

  // struct csid_cfg_data csid_cfg_data = {.cfgtype = CSID_RELEASE};
  struct csid_cfg_data csid_cfg_data = {};

  csid_cfg_data.cfgtype = CSID_RELEASE;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "release csid");

  // power down camera sensor
  std::cout << "sensor power down" << std::endl;
  struct sensorb_cfg_data sensorb_cfg_data = {.cfgtype = CFG_POWER_DOWN};
  cam_ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg_data, "sensor power down");

  // actuator powerdown
  actuator_cfg_data.cfgtype = CFG_ACTUATOR_POWERDOWN;
  cam_ioctl(actuator_fd, VIDIOC_MSM_ACTUATOR_CFG, &actuator_cfg_data, "actuator powerdown");

  // start camera stream
  // csid init
  std::cout << "csid init" << std::endl;
  csid_cfg_data.cfgtype = CSID_INIT;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "init csid");

  // // csiphy init
  std::cout << "csiphy init" << std::endl;
  csiphy_cfg_data = {.cfgtype = CSIPHY_INIT};
  cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "init csiphy");


  // sensor: stop stream
  std::cout << "sensor stop stream" << std::endl;
  struct msm_camera_i2c_reg_setting stop_settings = {
    .reg_setting = stop_reg_array,
    .size = std::size(stop_reg_array),
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = MSM_CAMERA_I2C_BYTE_DATA,
    .delay = 0
  };

  sensorb_cfg_data.cfgtype = CFG_SET_STOP_STREAM_SETTING;
  sensorb_cfg_data.cfg.setting = &stop_settings;
  cam_ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg_data, "stop stream");

  // sensor power up
  std::cout << "sensor power up" << std::endl;
  sensorb_cfg_data = {.cfgtype = CFG_POWER_UP};
  cam_ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg_data, "sensor power up");

  // send i2c configuration
  std::cout << "sending i2c configuration" << std::endl;
  err = sensor_write_regs(init_array_ov8865, std::size(init_array_ov8865), MSM_CAMERA_I2C_BYTE_DATA);
  std::cout << "i2c error: " << err << std::endl;

  // init actuator
  std::cout << "actuator powerup" << std::endl;
  actuator_cfg_data.cfgtype = CFG_ACTUATOR_POWERUP;
  cam_ioctl(actuator_fd, VIDIOC_MSM_ACTUATOR_CFG, &actuator_cfg_data, "actuator powerup");
  std::cout << "actuator init" << std::endl;
  actuator_cfg_data.cfgtype = CFG_ACTUATOR_INIT;
  cam_ioctl(actuator_fd, VIDIOC_MSM_ACTUATOR_CFG, &actuator_cfg_data, "actuator init");

  // configure csiphy
  std::cout << "configuring csiphy" << std::endl;
  struct msm_camera_csiphy_params csiphy_params = {};
  csiphy_params = {.lane_cnt = 4, .settle_cnt = 24, .lane_mask = 0x1f, .csid_core = 2};
  csiphy_cfg_data.cfgtype = CSIPHY_CFG;
  csiphy_cfg_data.cfg.csiphy_params = &csiphy_params;
  cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "csiphy configure");

  // configure csid
  std::cout << "configuring csid" << std::endl;
#define CSI_STATS 0x35
#define CSI_PD 0x36
  struct msm_camera_csid_params csid_params = {
    .lane_cnt = 4,
    .lane_assign = 0x4320,
    .phy_sel = (uint8_t)(0),
    .lut_params.num_cid = (uint8_t)(3),
    .lut_params.vc_cfg_a = {
      {.cid = 0, .dt = CSI_RAW10, .decode_format = CSI_DECODE_10BIT},
      {.cid = 1, .dt = CSI_PD, .decode_format = CSI_DECODE_10BIT},
      {.cid = 2, .dt = CSI_STATS, .decode_format = CSI_DECODE_10BIT},
    },
  };

  csid_params.lut_params.vc_cfg[0] = &csid_params.lut_params.vc_cfg_a[0];
  csid_params.lut_params.vc_cfg[1] = &csid_params.lut_params.vc_cfg_a[1];
  csid_params.lut_params.vc_cfg[2] = &csid_params.lut_params.vc_cfg_a[2];

  csid_cfg_data.cfgtype = CSID_CFG;
  csid_cfg_data.cfg.csid_params = &csid_params;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "csid configure");

  exit(0);
}

void Camera::camera_open()
{
    // std::cout << "sensor_fd: " << sensor_fd << std::endl;
}

void Camera::camera_run()
{
 
}

int Camera::sensor_write_regs(struct msm_camera_i2c_reg_array* arr, size_t size, msm_camera_i2c_data_type data_type)
{
  struct msm_camera_i2c_reg_setting out_settings = {
    .reg_setting = arr,
    .size = (uint16_t)size,
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = data_type,
    .delay = 0,
  };

  sensorb_cfg_data cfg_data = {.cfgtype = CFG_WRITE_I2C_ARRAY, .cfg.setting = &out_settings};
  return HANDLE_EINTR(ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &cfg_data));
}


#include <iostream>

#include "linux/videodev2.h"
#include "assert.h"

#include "camera.h"
#include "common.h"
#include "include/msm_cam_sensor.h"
#include "include/sensor_i2c.h"

// tools

int sensor_write_regs(CameraState *s, struct msm_camera_i2c_reg_array *arr, size_t size, msm_camera_i2c_data_type data_type) {
  int err = 0;
  struct msm_camera_i2c_reg_setting out_settings = {
    .reg_setting = arr,
    .size = (uint16_t)size,
    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
    .data_type = data_type,
    .delay = 0,
  };
  sensorb_cfg_data cfg_data = {.cfgtype = CFG_WRITE_I2C_ARRAY, .cfg.setting = &out_settings };
  std::cout << "sensor_fd: " << s->subdevices.sensor_fd << std::endl;
  err = cam_ioctl(s->subdevices.sensor_fd, VIDIOC_MSM_SENSOR_CFG, &cfg_data, "starting camera");
  return err;
}

// control

// init

static int camera_init() {
  int err = 0;
  return err;
}

// open

static int camera_open(struct CameraState *camera) {
  int err = 0;
  camera->subdevices.sensor_init_fd = open("/dev/v4l-subdev11", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.sensor_init_fd > 0);
  camera->subdevices.sensor_fd = open("/dev/v4l-subdev17", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.sensor_fd > 0);
  camera->subdevices.csid_fd = open("/dev/v4l-subdev3", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.csid_fd > 0);
  camera->subdevices.csiphy_fd = open("/dev/v4l-subdev0", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.csiphy_fd > 0);
  camera->subdevices.isp_fd = open("/dev/v4l-subdev13", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.isp_fd > 0);
  return err;
}

// start

static int camera_start(struct CameraState *camera) {
  int err = 0;
  err = sensor_write_regs(camera, start_reg_array, std::size(start_reg_array), MSM_CAMERA_I2C_BYTE_DATA);
  return err;
}

// test
int main() {
  int err = 0;
  std::cout << "starting camera capture" << std::endl;
  CameraState camera = {};
  // init
  // open
  err = camera_open(&camera);
  // err = camera_start(&camera);
}

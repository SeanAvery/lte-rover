#include <iostream>
#include "camera.h"
#include "common.h"
#include "include/msm_cam_sensor.h"
#include "include/sensor_i2c.h"

// tools

int sensor_write_regs(CameraState *s, struct msm_camera_i2c_reg_array *arr, size_t size, msm_camera_i2c_data_type data_type) {
  return 0;
}

// control

static int camera_init() {
  int err = 0;
  return err;
}

static int camera_open(struct CameraState *camera) {
  int err = 0;
  camera->subdevices.sensor_init_fd = open("/dev/v4l-subdev11", O_RDWR | O_NONBLOCK);
  camera->subdevices.sensor_fd = open("/dev/v4l-subdev17", O_RDWR | O_NONBLOCK);
  return err;
}

static int camera_start(CameraState camera) {
  int err = 0;
  err = sensor_write_regs(&camera, start_reg_array, std::size(start_reg_array), MSM_CAMERA_I2C_BYTE_DATA);
  return err;
}

// test
int main() {
  std::cout << "starting camera capture" << std::endl;
  CameraState camera = {};
  // init
  // open
  camera_open(&camera);
  camera_start(camera);

  std::cout << camera.subdevices.sensor_fd << std::endl;
}

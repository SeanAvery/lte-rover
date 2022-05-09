#include <iostream>
#include "camera.h"
#include "common.h"
#include "include/msm_cam_sensor.h"

static int camera_init() {
  int err = 0;
  return err;
}

static int camera_open(CameraState camera) {
  int err = 0;
  return err;
}

static int camera_start(CameraState camera) {
  int err = 0;
  return err;
}

// test
int main() {
  std::cout << "starting camera capture" << std::endl;
  CameraState camera = {};
  // init
  // open
  camera_open(camera);
  camera_start(camera);
}

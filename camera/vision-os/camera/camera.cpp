#include <iostream>

#include "linux/videodev2.h"
#include "unistd.h"
#include "assert.h"

#include "camera.h"
#include "common.h"

// TOOLS

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
  err = cam_ioctl(s->subdevices.sensor_fd, VIDIOC_MSM_SENSOR_CFG, &cfg_data, "init camera");
  return err;
}

// CONTROL

// init

static int camera_init() {
  int err = 0;
  return err;
}

// open

static int camera_shutdown(struct CameraState *camera) {
  int err = 0;

  struct msm_camera_csi_lane_params csi_lane_params = { 0 };
  csi_lane_params.csi_lane_mask = 0x1f;
  csiphy_cfg_data csiphy_cfg = { .cfg.csi_lane_params = &csi_lane_params, .cfgtype = CSIPHY_RELEASE };
  err = cam_ioctl(camera->subdevices.csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg, "release csiphy");

  struct csid_cfg_data csid_cfg = { .cfgtype = CSID_RELEASE };
  err = cam_ioctl(camera->subdevices.csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg, "release csid");

  struct sensorb_cfg_data sensorb_cfg = { .cfgtype = CFG_POWER_DOWN };
  err = cam_ioctl(camera->subdevices.sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg, "sensor power down");

  return err;
}

static int camera_up(struct CameraState *camera) {
  int err = 0;

  struct csid_cfg_data csid_cfg = { .cfgtype = CSID_INIT };
  err = cam_ioctl(camera->subdevices.csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg, "init csid");

  struct csiphy_cfg_data csiphy_cfg = { .cfgtype = CSIPHY_INIT };
  err = cam_ioctl(camera->subdevices.csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg, "init csiphy");

  struct sensorb_cfg_data sensorb_cfg = { .cfgtype = CFG_POWER_UP };
  err = cam_ioctl(camera->subdevices.sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg, "sensor power up");

  err = sensor_write_regs(camera, init_array_imx298, std::size(init_array_imx298), MSM_CAMERA_I2C_BYTE_DATA);

  err = sensor_write_regs(camera, mode_setting_array_imx298, std::size(mode_setting_array_imx298), MSM_CAMERA_I2C_BYTE_DATA);

  csid_params.lut_params.vc_cfg[0] = &csid_params.lut_params.vc_cfg_a[0];
  csid_params.lut_params.vc_cfg[1] = &csid_params.lut_params.vc_cfg_a[1];
  csid_params.lut_params.vc_cfg[2] = &csid_params.lut_params.vc_cfg_a[2];
  csid_cfg.cfgtype = CSID_CFG;
  csid_cfg.cfg.csid_params = &csid_params;
  cam_ioctl(camera->subdevices.csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg, "csid configure");

  return err;
}

static int camera_configure_stream(struct CameraState *camera) {
  int err = 0;
  msm_vfe_smmu_attach_cmd smmu_cmd = { .security_mode = 0, .iommu_attach_mode = IOMMU_ATTACH };
  err = cam_ioctl(camera->subdevices.isp_fd, VIDIOC_MSM_ISP_SMMU_ATTACH, &smmu_cmd, "isp smmu attach");
  return err;
}

static void sensors_init(struct CameraState *camera) {
  cam_info.power_setting_array.power_setting = &cam_info.power_setting_array.power_setting_a[0];
  cam_info.power_setting_array.power_down_setting = &cam_info.power_setting_array.power_down_setting_a[0];
  sensor_init_cfg_data sensor_init_cfg = {.cfgtype = CFG_SINIT_PROBE, .cfg.setting = &cam_info};
  int err = cam_ioctl(camera->subdevices.sensor_init_fd, VIDIOC_MSM_SENSOR_INIT_CFG, &sensor_init_cfg, "sensor init cfg");
  // allow time for sensor to init
  for (int i = 0; i < 10; i++) {
    camera->subdevices.sensor_fd = open("/dev/v4l-subdev17", O_RDWR | O_NONBLOCK);
    if (camera->subdevices.sensor_fd > 0) break;
    std::cout << "waiting for sensor susbsytem init" << std::endl;
    usleep(1000000); // sleep for 1 second
  }
}

static int camera_open(struct CameraState *camera) {
  int err = 0;

  // connect to v4l subsystem files (IMX298)
  camera->subdevices.ispif_fd = open("/dev/v4l-subdev15", O_RDWR | O_NONBLOCK);

  camera->subdevices.sensor_init_fd = open("/dev/v4l-subdev11", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.sensor_init_fd > 0);
  camera->subdevices.csid_fd = open("/dev/v4l-subdev3", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.csid_fd > 0);
  camera->subdevices.csiphy_fd = open("/dev/v4l-subdev0", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.csiphy_fd > 0);
  camera->subdevices.isp_fd = open("/dev/v4l-subdev13", O_RDWR | O_NONBLOCK);
  assert(camera->subdevices.isp_fd > 0);

  sensors_init(camera);

  assert(camera->subdevices.sensor_fd > 0);

  camera_shutdown(camera);
  camera_up(camera);
  camera_configure_stream(camera);

  return err;
}

// start

static int camera_start(struct CameraState *camera) {
  int err = 0;
  err = sensor_write_regs(camera, start_reg_array, std::size(start_reg_array), MSM_CAMERA_I2C_BYTE_DATA);
  return err;
}

// TEST

int main() {
  int err = 0;
  std::cout << "starting camera capture" << std::endl;
  CameraState camera = {};
  // init
  // open
  err = camera_open(&camera);
  // err = camera_start(&camera);
}

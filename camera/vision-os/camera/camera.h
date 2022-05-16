#include <fstream>

#include "include/msm_cam_sensor.h"
#include "include/sensor_i2c.h"
#include "include/msmb_isp.h"
// #include "visionipc/visionbuf.h"

typedef struct SubDevices {
  int csid_fd;
  int csiphy_fd;
  int isp_fd;
  int sensor_fd;
  int sensor_init_fd;
} SubDevices;

typedef struct CameraState {
  int type;
  int fps;
  bool on;
  SubDevices subdevices;
} CameraState;

// slave info
msm_camera_sensor_slave_info cam_info = {
  .sensor_name = "imx298",
  .eeprom_name = "sony_imx298",
  .actuator_name = "dw9800w",
  .ois_name = "",
  .flash_name = "pmic",
  .camera_id = CAMERA_0,
  .slave_addr = 32,
  .i2c_freq_mode = I2C_FAST_MODE,
  .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
  .sensor_id_info = {.sensor_id_reg_addr = 22, .sensor_id = 664, .module_id = 9, .vcm_id = 6},
  .power_setting_array = {
    .power_setting_a = {
      {.seq_type = SENSOR_GPIO, .delay = 1},
      {.seq_type = SENSOR_VREG, .seq_val = 2},
      {.seq_type = SENSOR_GPIO, .seq_val = 5, .config_val = 2},
      {.seq_type = SENSOR_VREG, .seq_val = 1},
      {.seq_type = SENSOR_VREG, .seq_val = 3, .delay = 1},
      {.seq_type = SENSOR_CLK, .config_val = 24000000, .delay = 1},
      {.seq_type = SENSOR_GPIO, .config_val = 2, .delay = 10},
    },
    .size = 7,
    .power_down_setting_a = {
      {.seq_type = SENSOR_CLK, .delay = 1},
      {.seq_type = SENSOR_GPIO, .delay = 1},
      {.seq_type = SENSOR_VREG, .seq_val = 1},
      {.seq_type = SENSOR_GPIO, .seq_val = 5},
      {.seq_type = SENSOR_VREG, .seq_val = 2},
      {.seq_type = SENSOR_VREG, .seq_val = 3, .delay = 1},
    },
    .size_down = 6,
  },
  .is_init_params_valid = 0,
  .sensor_init_params = {.modes_supported = 1, .position = BACK_CAMERA_B, .sensor_mount_angle = 90},
  .output_format = MSM_SENSOR_BAYER,
};

#define CSI_STATS 0x35
#define CSI_PD 0x36

struct msm_camera_csid_params csid_params = {
  .lane_cnt = 4,
  .lane_assign = 0x4320,
  .phy_sel = (uint8_t)0,
  .lut_params.num_cid = (uint8_t)3,
  .lut_params.vc_cfg_a = {
    {.cid = 0, .dt = CSI_RAW10, .decode_format = CSI_DECODE_10BIT},
    {.cid = 1, .dt = CSI_PD, .decode_format = CSI_DECODE_10BIT},
    {.cid = 2, .dt = CSI_STATS, .decode_format = CSI_DECODE_10BIT},
  },
};




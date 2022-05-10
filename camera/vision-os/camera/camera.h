#include <fstream>

#include "include/msm_cam_sensor.h"
#include "include/sensor_i2c.h"

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




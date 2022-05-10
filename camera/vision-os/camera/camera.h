#include <fstream>

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


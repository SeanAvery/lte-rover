#include <string>

namespace params
{
  std::string CSID_SUBSYSTEM = "/dev/v4l-subdev3";
  std::string CSIPHY_SUBSYSTEM = "/dev/v4l-subdev0";
  std::string SENSOR_SUBSYSTEM = "/dev/v4l-subdev13";
  std::string ACTUATOR_SUBSYSTEM = "v4l-subdev7";
}

class Camera
{
  private:
    int BringUp();

  public:
    int Init();
};

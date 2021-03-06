#include "Camera.h"

// #include "include/msm_camsensor_sdk.h"
// #include "sensor_i2c.h"
// #include "include/msm_cam_sensor.h"
// #include "include/msmb_camera.h"
// #include "include/msmb_isp.h"
// #include "include/msmb_ispif.h"

#include <linux/media.h>


#include <iostream>
#include <fstream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <iterator>
#include <assert.h>
#include <string.h>
#include <poll.h>


void Camera::camera_open()
{
  std::cout << std::endl;
  std::cout << "opening subsystem files" << std::endl;
  msm_fd = HANDLE_EINTR(open(params::MSM_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  actuator_fd = HANDLE_EINTR(open(params::ACTUATOR_SUBSYSTEM, O_RDWR | O_NONBLOCK));

  // open subdevice files
  csid_fd = HANDLE_EINTR(open(params::CSID_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  assert(csid_fd >= 0);

  csiphy_fd= HANDLE_EINTR(open(params::CSIPHY_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  isp_fd= HANDLE_EINTR(open(params::ISP_SUBSYSTEM, O_RDWR | O_NONBLOCK));

  camera_init();

  ispif_fd = HANDLE_EINTR(open(params::ISPIF_SUBSYSTEM, O_RDWR | O_NONBLOCK));
  v4l_fd = HANDLE_EINTR(open(params::V4L_SUBSYSTEM, O_RDWR | O_NONBLOCK));
}

void Camera::camera_init()
{
    // struct csid_cfg_data csid_cfg_data = {};
  struct msm_actuator_cfg_data actuator_cfg_data = {};
  struct  v4l2_event_subscription sub = {};

  // sensor init
  sensorinit_fd =  HANDLE_EINTR(open(params::SENSORINIT_SUBSYSTEM, O_RDWR | O_NONBLOCK));

  // slave info
  msm_camera_sensor_slave_info info = { // road camera;
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
  std::cout << std::endl;
  std::cout << "shutdown camera" << std::endl;
  // release csiphy
  std::cout << "release csiphy" << std::endl;
  struct msm_camera_csi_lane_params csi_lane_params = {0};
  csi_lane_params.csi_lane_mask = 0x1f;
  csiphy_cfg_data csiphy_cfg_data = { .cfgtype = CSIPHY_RELEASE, .cfg.csi_lane_params = &csi_lane_params}; 
  int err = cam_ioctl(csiphy_fd, VIDIOC_MSM_CSIPHY_IO_CFG, &csiphy_cfg_data, "release csiphy");
 

  // release csid
  struct csid_cfg_data csid_cfg_data = {};

  csid_cfg_data.cfgtype = CSID_RELEASE;

  std::cout << "release csid" << std::endl;
  cam_ioctl(csid_fd, VIDIOC_MSM_CSID_IO_CFG, &csid_cfg_data, "release csid");

  // power down camera sensor
  std::cout << "sensor power down" << std::endl;
  struct sensorb_cfg_data sensorb_cfg_data = {.cfgtype = CFG_POWER_DOWN};
  cam_ioctl(sensor_fd, VIDIOC_MSM_SENSOR_CFG, &sensorb_cfg_data, "sensor power down");

  // actuator powerdown
  actuator_cfg_data.cfgtype = CFG_ACTUATOR_POWERDOWN;
  cam_ioctl(actuator_fd, VIDIOC_MSM_ACTUATOR_CFG, &actuator_cfg_data, "actuator powerdown");

  // start camera stream

  std::cout << std::endl;
  std::cout << "starting camera stream" << std::endl;

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

  // smmu attach
  std::cout << "attaching smmu " << std::endl;
  msm_vfe_smmu_attach_cmd smmu_attach_cmd = {.security_mode = 0, .iommu_attach_mode = IOMMU_ATTACH};
  cam_ioctl(isp_fd, VIDIOC_MSM_ISP_SMMU_ATTACH, &smmu_attach_cmd, "isp smmu attach");

  std::cout << "configuring input" << std::endl;
  struct msm_vfe_input_cfg input_cfg = {};

  for (int i = 0; i < 1; i++)
  {
    std::cout << std::endl;
    std::cout << "configuring stream " << i << std::endl;
    StreamState *s = &ss[i];
    memset(&input_cfg, 0, sizeof(struct msm_vfe_input_cfg));
    input_cfg.input_src = (msm_vfe_input_src)(VFE_RAW_0 + 1);
    input_cfg.input_pix_clk = pixel_clock;
    input_cfg.d.rdi_cfg.cid = 0;
    input_cfg.d.rdi_cfg.frame_based = 1;
    err = ioctl(isp_fd, VIDIOC_MSM_ISP_INPUT_CFG, &input_cfg);
    std::cout << "configure error: " << err << std::endl;

    // configure isp stream
    s->stream_req.axi_stream_handle = 0;
    s->stream_req.session_id = 2;
    s->stream_req.stream_id = ISP_NATIVE_BUF_BIT | (1 + i);
    if (i == 0)
    {
      s->stream_req.output_format = v4l2_fourcc('R', 'G', '1', '0');
    } 
    else
    {
      s->stream_req.output_format = v4l2_fourcc('Q', 'M', 'E', 'T');
    }

    s->stream_req.stream_src = (msm_vfe_axi_stream_src)(RDI_INTF_0 + i);
    s->stream_req.frame_base = 1;
    s->stream_req.frame_base = 1;
    s->stream_req .buf_divert = 1;

    std::cout << "configuring isp stream" << std::endl;
    err = cam_ioctl(isp_fd, VIDIOC_MSM_ISP_REQUEST_STREAM, &s->stream_req, "configure stream");
    std::cout << "stream request error: " << err << std::endl;

    s->buf_request.session_id = s->stream_req.session_id;
    s->buf_request.stream_id = s->stream_req.stream_id;
    s->buf_request.num_buf = FRAME_BUF_COUNT;
    s->buf_request.buf_type = ISP_PRIVATE_BUF;
    s->buf_request.handle = 0;

    // isp request_buf
    cam_ioctl(isp_fd, VIDIOC_MSM_ISP_REQUEST_BUF, &s->buf_request, "isp request buf");
    std::cout << "got buf handle: " << s->buf_request.handle << std::endl;

    // enqueue buffers
    for (int j = 0; j < ss->buf_request.num_buf; j++)
    {
      s->qbuf_info[j].handle = s->buf_request.handle;
      s->qbuf_info[j].buf_idx = j;
      s->qbuf_info[j].buffer.num_planes = 1;
      // s->qbuf_info[j].buffer.planes[0].addr = s->bufs[j].fd;
      // ss->qbuf_info[j].buffer.planes[0].length = ss->bufs[j].len;
      err = ioctl(isp_fd, VIDIOC_MSM_ISP_ENQUEUE_BUF, &ss->qbuf_info[j]);
    }
    // update stream
    struct msm_vfe_axi_stream_update_cmd update_cmd = {};
    update_cmd.num_streams = 1;
    update_cmd.update_info[0].user_stream_id = s->stream_req.stream_id;
    update_cmd.update_info[0].stream_handle = s->stream_req.axi_stream_handle;
    update_cmd.update_type = UPDATE_STREAM_ADD_BUFQ;
    cam_ioctl(isp_fd, VIDIOC_MSM_ISP_UPDATE_STREAM, &update_cmd, "isp update stream");
  }

  // start streams

  // subscribe event
  sub.id = 0;
  sub.type = 0x1ff;
  std::cout << "isp subscribe" << std::endl;
  cam_ioctl(isp_fd, VIDIOC_SUBSCRIBE_EVENT, &sub, "isp subscribe");

  stream_cfg.cmd = START_STREAM;
  stream_cfg.num_streams = 1;
  for (int i = 0; i < stream_cfg.num_streams; i++)
  {
    stream_cfg.stream_handle[i] = ss[i].stream_req.axi_stream_handle;
  }
  cam_ioctl(isp_fd, VIDIOC_MSM_ISP_CFG_STREAM, &stream_cfg, "isp start stream");
}

void Camera::camera_run()
{
  // start camera process thread
  std::cout << "start camera process thread" << std::endl;
  std::thread thread = std::thread(&Camera::camera_process, this);

  std::cout << std::endl;
  std::cout << "camera_run" << std::endl;
  while(true)
  {
    std::cout << 0 << std::endl;
    struct pollfd fds[1] = {{ .fd = isp_fd, .events = POLLPRI }};
    int ret = poll(fds, std::size(fds), 1000);
    if (ret != 0)
    {
      std::cout << "poll error: " << ret << std::endl;
      break;
    }

    std::cout << 1 << std::endl;

    std::cout << "fd: " << fds[0].fd << std::endl;
    std::cout << "events: " << fds[0].events << std::endl;
    std::cout << "revents: " << fds[0].revents << std::endl;
    // if (!fds[0].revents) continue;

    std::cout << 2 << std::endl;
    struct v4l2_event ev = {};
    ret = HANDLE_EINTR(ioctl(isp_fd, VIDIOC_DQEVENT, &ev));
    const msm_isp_event_data *isp_event_data = (const msm_isp_event_data *)ev.u.data;

    std::cout << 3 << std::endl;

    if (ev.type = ISP_EVENT_BUF_DIVERT)
    {
      std::cout << "ISP_EVENT_BUF_DIVERT" << std::endl;
      std::cout << "isp buf" << &ev.u.data << std::endl;
      const int buf_idx = isp_event_data->u.buf_done.buf_idx;
      const int buffer = (isp_event_data->u.buf_done.stream_id & 0xFFFF) - 1;
      if (buffer == 0)
      {
        std::cout << "buffer is 0"  << std::endl;
      }
      else
      {
        if (buffer == 1)
        {
          std::cout << "buffer is 1" << std::endl;
        }
        auto &s = ss[buffer];
        s.qbuf_info[buf_idx].dirty_buf = 1;
        cam_ioctl(isp_fd, VIDIOC_MSM_ISP_ENQUEUE_BUF, &s.qbuf_info[buf_idx]);
        std::cout << s.qbuf_info << std::endl;

      }
    }
    else if (ev.type == ISP_EVENT_EOF)
    {
      std::cout << "ISP_EVENT_EOF" << std::endl;

    }
    else if (ev.type == ISP_EVENT_ERROR)
    {
      std::cout << "ISP_EVENT_ERROR" << std::endl;
    }
  }
}

void Camera::camera_process()
{
  std::cout << "camera process thread here" << std::endl;
  while (true)
  {
    usleep(100000);
    std::cout << "process thread hit" << std::endl;
  }
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

//msm_camera_sensor_slave_info info = {
//    .sensor_name = "ov8865_sunny",
//    .eeprom_name = "ov8865_plus",
//    .actuator_name = "",
//    .ois_name = "",
//    .flash_name = "",
//    .camera_id = CAMERA_2,
//    .slave_addr = 108,
//    .i2c_freq_mode = I2C_FAST_MODE,
//    .addr_type = MSM_CAMERA_I2C_WORD_ADDR,
//    .sensor_id_info = {.sensor_id_reg_addr = 12299, .sensor_id = 34917, .module_id = 2},
//    .power_setting_array = {
//      .power_setting_a = {
//        {.seq_type = SENSOR_GPIO, .delay = 5},
//        {.seq_type = SENSOR_VREG, .seq_val = 1},
//        {.seq_type = SENSOR_VREG, .seq_val = 2},
//        {.seq_type = SENSOR_VREG},
//        {.seq_type = SENSOR_CLK, .config_val = 24000000, .delay = 1},
//        {.seq_type = SENSOR_GPIO, .config_val = 2, .delay = 1},
//      },
//      .size = 6,
//      .power_down_setting_a = {
//        {.seq_type = SENSOR_GPIO, .delay = 5},
//        {.seq_type = SENSOR_CLK, .delay = 1},
//        {.seq_type = SENSOR_VREG},
//        {.seq_type = SENSOR_VREG, .seq_val = 1},
//        {.seq_type = SENSOR_VREG, .seq_val = 2, .delay = 1},
//      },
//      .size_down = 5,
//    },
//    .is_init_params_valid = 0,
//    .sensor_init_params = {.modes_supported = 1, .position = FRONT_CAMERA_B, .sensor_mount_angle = 270},
//    .output_format = MSM_SENSOR_BAYER,
//  };
//

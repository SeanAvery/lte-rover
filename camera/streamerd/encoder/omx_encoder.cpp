#include "omx_encoder.h"

#define PORT_INDEX_IN 0
#define PORT_INDEX_OUT 1
#define BITRATE 5000000
#define FPS 20

// Check the OMX error code and assert if an error occurred.
#define OMX_CHECK(_expr)              \
  do {                                \
    assert(OMX_ErrorNone == (_expr)); \
  } while (0)

static const char* omx_color_fomat_name(uint32_t format) __attribute__((unused));
static const char* omx_color_fomat_name(uint32_t format) {
  switch (format) {
  case OMX_COLOR_FormatUnused: return "OMX_COLOR_FormatUnused";
  case OMX_COLOR_FormatMonochrome: return "OMX_COLOR_FormatMonochrome";
  case OMX_COLOR_Format8bitRGB332: return "OMX_COLOR_Format8bitRGB332";
  case OMX_COLOR_Format12bitRGB444: return "OMX_COLOR_Format12bitRGB444";
  case OMX_COLOR_Format16bitARGB4444: return "OMX_COLOR_Format16bitARGB4444";
  case OMX_COLOR_Format16bitARGB1555: return "OMX_COLOR_Format16bitARGB1555";
  case OMX_COLOR_Format16bitRGB565: return "OMX_COLOR_Format16bitRGB565";
  case OMX_COLOR_Format16bitBGR565: return "OMX_COLOR_Format16bitBGR565";
  case OMX_COLOR_Format18bitRGB666: return "OMX_COLOR_Format18bitRGB666";
  case OMX_COLOR_Format18bitARGB1665: return "OMX_COLOR_Format18bitARGB1665";
  case OMX_COLOR_Format19bitARGB1666: return "OMX_COLOR_Format19bitARGB1666";
  case OMX_COLOR_Format24bitRGB888: return "OMX_COLOR_Format24bitRGB888";
  case OMX_COLOR_Format24bitBGR888: return "OMX_COLOR_Format24bitBGR888";
  case OMX_COLOR_Format24bitARGB1887: return "OMX_COLOR_Format24bitARGB1887";
  case OMX_COLOR_Format25bitARGB1888: return "OMX_COLOR_Format25bitARGB1888";
  case OMX_COLOR_Format32bitBGRA8888: return "OMX_COLOR_Format32bitBGRA8888";
  case OMX_COLOR_Format32bitARGB8888: return "OMX_COLOR_Format32bitARGB8888";
  case OMX_COLOR_FormatYUV411Planar: return "OMX_COLOR_FormatYUV411Planar";
  case OMX_COLOR_FormatYUV411PackedPlanar: return "OMX_COLOR_FormatYUV411PackedPlanar";
  case OMX_COLOR_FormatYUV420Planar: return "OMX_COLOR_FormatYUV420Planar";
  case OMX_COLOR_FormatYUV420PackedPlanar: return "OMX_COLOR_FormatYUV420PackedPlanar";
  case OMX_COLOR_FormatYUV420SemiPlanar: return "OMX_COLOR_FormatYUV420SemiPlanar";
  case OMX_COLOR_FormatYUV422Planar: return "OMX_COLOR_FormatYUV422Planar";
  case OMX_COLOR_FormatYUV422PackedPlanar: return "OMX_COLOR_FormatYUV422PackedPlanar";
  case OMX_COLOR_FormatYUV422SemiPlanar: return "OMX_COLOR_FormatYUV422SemiPlanar";
  case OMX_COLOR_FormatYCbYCr: return "OMX_COLOR_FormatYCbYCr";
  case OMX_COLOR_FormatYCrYCb: return "OMX_COLOR_FormatYCrYCb";
  case OMX_COLOR_FormatCbYCrY: return "OMX_COLOR_FormatCbYCrY";
  case OMX_COLOR_FormatCrYCbY: return "OMX_COLOR_FormatCrYCbY";
  case OMX_COLOR_FormatYUV444Interleaved: return "OMX_COLOR_FormatYUV444Interleaved";
  case OMX_COLOR_FormatRawBayer8bit: return "OMX_COLOR_FormatRawBayer8bit";
  case OMX_COLOR_FormatRawBayer10bit: return "OMX_COLOR_FormatRawBayer10bit";
  case OMX_COLOR_FormatRawBayer8bitcompressed: return "OMX_COLOR_FormatRawBayer8bitcompressed";
  case OMX_COLOR_FormatL2: return "OMX_COLOR_FormatL2";
  case OMX_COLOR_FormatL4: return "OMX_COLOR_FormatL4";
  case OMX_COLOR_FormatL8: return "OMX_COLOR_FormatL8";
  case OMX_COLOR_FormatL16: return "OMX_COLOR_FormatL16";
  case OMX_COLOR_FormatL24: return "OMX_COLOR_FormatL24";
  case OMX_COLOR_FormatL32: return "OMX_COLOR_FormatL32";
  case OMX_COLOR_FormatYUV420PackedSemiPlanar: return "OMX_COLOR_FormatYUV420PackedSemiPlanar";
  case OMX_COLOR_FormatYUV422PackedSemiPlanar: return "OMX_COLOR_FormatYUV422PackedSemiPlanar";
  case OMX_COLOR_Format18BitBGR666: return "OMX_COLOR_Format18BitBGR666";
  case OMX_COLOR_Format24BitARGB6666: return "OMX_COLOR_Format24BitARGB6666";
  case OMX_COLOR_Format24BitABGR6666: return "OMX_COLOR_Format24BitABGR6666";

  case OMX_COLOR_FormatAndroidOpaque: return "OMX_COLOR_FormatAndroidOpaque";
  case OMX_TI_COLOR_FormatYUV420PackedSemiPlanar: return "OMX_TI_COLOR_FormatYUV420PackedSemiPlanar";
  case OMX_QCOM_COLOR_FormatYVU420SemiPlanar: return "OMX_QCOM_COLOR_FormatYVU420SemiPlanar";
  case OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar64x32Tile2m8ka: return "OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar64x32Tile2m8ka";
  case OMX_SEC_COLOR_FormatNV12Tiled: return "OMX_SEC_COLOR_FormatNV12Tiled";
  case OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar32m: return "OMX_QCOM_COLOR_FormatYUV420PackedSemiPlanar32m";

  // case QOMX_COLOR_FormatYVU420SemiPlanar: return "QOMX_COLOR_FormatYVU420SemiPlanar";
  case QOMX_COLOR_FormatYVU420PackedSemiPlanar32m4ka: return "QOMX_COLOR_FormatYVU420PackedSemiPlanar32m4ka";
  case QOMX_COLOR_FormatYUV420PackedSemiPlanar16m2ka: return "QOMX_COLOR_FormatYUV420PackedSemiPlanar16m2ka";
  // case QOMX_COLOR_FormatYUV420PackedSemiPlanar64x32Tile2m8ka: return "QOMX_COLOR_FormatYUV420PackedSemiPlanar64x32Tile2m8ka";
  // case QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m: return "QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m";
  case QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView: return "QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mMultiView";
  case QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mCompressed: return "QOMX_COLOR_FORMATYUV420PackedSemiPlanar32mCompressed";
  case QOMX_COLOR_Format32bitRGBA8888: return "QOMX_COLOR_Format32bitRGBA8888";
  case QOMX_COLOR_Format32bitRGBA8888Compressed: return "QOMX_COLOR_Format32bitRGBA8888Compressed";

  default:
    return "unkn";
  }
}

// OMX callbakcs

void OmxEncoder::wait_for_state(OMX_STATETYPE state) {
  std::unique_lock lk(this->state_lock);
  while (this->state != state) {
    this->state_cv.wait(lk);
  }
};

OMX_ERRORTYPE OmxEncoder::event_handler(OMX_HANDLETYPE component, OMX_PTR app_data, OMX_EVENTTYPE event,
                                   OMX_U32 data1, OMX_U32 data2, OMX_PTR event_data) {
  OmxEncoder *e = (OmxEncoder*)app_data;
  if (event == OMX_EventCmdComplete) {
    assert(data1 == OMX_CommandStateSet);
    // LOG("set state event 0x%x", data2);
    {
      std::unique_lock lk(e->state_lock);
      e->state = (OMX_STATETYPE)data2;
    }
    e->state_cv.notify_all();
  } else if (event == OMX_EventError) {
    // LOGE("OMX error 0x%08x", data1);
  } else {
    // LOGE("OMX unhandled event %d", event);
    assert(false);
  }

  return OMX_ErrorNone;
}

OMX_ERRORTYPE OmxEncoder::empty_buffer_done(OMX_HANDLETYPE component, OMX_PTR app_data,
                                                   OMX_BUFFERHEADERTYPE *buffer) {
  OmxEncoder *e = (OmxEncoder*)app_data;
  e->free_in.push(buffer);
  return OMX_ErrorNone;
}

OMX_ERRORTYPE OmxEncoder::fill_buffer_done(OMX_HANDLETYPE component, OMX_PTR app_data,
                                                  OMX_BUFFERHEADERTYPE *buffer) {
  OmxEncoder *e = (OmxEncoder*)app_data;
  e->done_out.push(buffer);
  return OMX_ErrorNone;
}

static OMX_CALLBACKTYPE omx_callbacks = {
  .EventHandler = OmxEncoder::event_handler,
  .EmptyBufferDone = OmxEncoder::empty_buffer_done,
  .FillBufferDone = OmxEncoder::fill_buffer_done,
};

OmxEncoder::OmxEncoder()
{
  // this->width = 1156;
  this->width = 1632;
  this->height = 1224;
  // this->fps = FPS;
  this->fps = 20;

  // h264
  auto component = (OMX_STRING)("OMX.qcom.video.encoder.avc");
  
  int err = OMX_GetHandle(&this->handle, component, this, &omx_callbacks);
  if (err != OMX_ErrorNone)
  {
    std::cout << "error getting omx codec" << std::endl;
  }
  assert(err == OMX_ErrorNone);

  std::cout << "handle: " << this->handle << std::endl;

  // setup input port
  OMX_PARAM_PORTDEFINITIONTYPE in_port = {0};
  in_port.nSize = sizeof(in_port);
  in_port.nPortIndex = (OMX_U32) PORT_INDEX_IN;
  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR) &in_port));

  in_port.format.video.nFrameWidth = this->width;
  in_port.format.video.nFrameHeight = this->height;
  in_port.format.video.nStride = VENUS_Y_STRIDE(COLOR_FMT_NV12, this->width);
  in_port.format.video.nSliceHeight = this->height;
  // in_port.nBufferSize = (this->width * this->height * 3) / 2;
  in_port.nBufferSize = VENUS_BUFFER_SIZE(COLOR_FMT_NV12, this->width, this->height);
  in_port.format.video.xFramerate = (this->fps * 65536);
  in_port.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
  // in_port.format.video.eColorFormat = OMX_COLOR_FormatYUV420SemiPlanar;
  in_port.format.video.eColorFormat = (OMX_COLOR_FORMATTYPE)QOMX_COLOR_FORMATYUV420PackedSemiPlanar32m;

  OMX_CHECK(OMX_SetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR) &in_port));

  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR) &in_port));

  this->in_buf_headers.resize(in_port.nBufferCountActual);


  // setup output port
  OMX_PARAM_PORTDEFINITIONTYPE out_port = {0};
  out_port.nSize = sizeof(out_port);
  out_port.nPortIndex = (OMX_U32) PORT_INDEX_OUT;
  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR)&out_port));
  out_port.format.video.nFrameWidth = this->width;
  out_port.format.video.nFrameHeight = this->height;
  out_port.format.video.xFramerate = 0;
  out_port.format.video.nBitrate = BITRATE;
  // h264
  out_port.format.video.eCompressionFormat = OMX_VIDEO_CodingAVC;

  out_port.format.video.eColorFormat = OMX_COLOR_FormatUnused;

  OMX_CHECK(OMX_SetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR) &out_port));

  
  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamPortDefinition, (OMX_PTR) &out_port));

  this->out_buf_headers.resize(out_port.nBufferCountActual);

  OMX_VIDEO_PARAM_BITRATETYPE bitrate_type = {0};
  bitrate_type.nSize = sizeof(bitrate_type);
  bitrate_type.nPortIndex = (OMX_U32) PORT_INDEX_OUT;
  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamVideoBitrate, (OMX_PTR) &bitrate_type));
  bitrate_type.eControlRate = OMX_Video_ControlRateVariable;
  bitrate_type.nTargetBitrate = BITRATE;

  OMX_CHECK(OMX_SetParameter(this->handle, OMX_IndexParamVideoBitrate, (OMX_PTR) &bitrate_type));

  // setup h264 stuff
  // setup h264
  OMX_VIDEO_PARAM_AVCTYPE avc = { 0 };
  avc.nSize = sizeof(avc);
  avc.nPortIndex = (OMX_U32) PORT_INDEX_OUT;
  OMX_CHECK(OMX_GetParameter(this->handle, OMX_IndexParamVideoAvc, &avc));

  avc.nBFrames = 0;
  avc.nPFrames = 15;

  avc.eProfile = OMX_VIDEO_AVCProfileHigh;
  avc.eLevel = OMX_VIDEO_AVCLevel31;

  avc.nAllowedPictureTypes |= OMX_VIDEO_PictureTypeB;
  avc.eLoopFilterMode = OMX_VIDEO_AVCLoopFilterEnable;

  avc.nRefFrames = 1;
  avc.bUseHadamard = OMX_TRUE;
  avc.bEntropyCodingCABAC = OMX_TRUE;
  avc.bWeightedPPrediction = OMX_TRUE;
  avc.bconstIpred = OMX_TRUE;

  OMX_CHECK(OMX_SetParameter(this->handle, OMX_IndexParamVideoAvc, &avc));

  OMX_CHECK(OMX_SendCommand(this->handle, OMX_CommandStateSet, OMX_StateIdle, NULL));

  for (auto &buf : this->in_buf_headers) {
    OMX_CHECK(OMX_AllocateBuffer(this->handle, &buf, PORT_INDEX_IN, this,
                             in_port.nBufferSize));
  }

  for (auto &buf : this->out_buf_headers) {
    OMX_CHECK(OMX_AllocateBuffer(this->handle, &buf, PORT_INDEX_OUT, this,
                             out_port.nBufferSize));
  }

  wait_for_state(OMX_StateIdle);

  std::cout << "omx setate idle" << std::endl;

  OMX_CHECK(OMX_SendCommand(this->handle, OMX_CommandStateSet, OMX_StateExecuting, NULL));

  wait_for_state(OMX_StateExecuting);

  std::cout << "omx state executing" << std::endl;

  // give omx all the output buffers
  for (auto &buf : this->out_buf_headers) {
    // printf("fill %p\n", this->out_buf_headers[i]);
    OMX_CHECK(OMX_FillThisBuffer(this->handle, buf));
  }

  // fill the input free queue
  for (auto &buf : this->in_buf_headers) {
    this->free_in.push(buf);
  }

}

#include "omx_encoder.h"


// OMX callbakcs

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
  this->width = 1156;
  this->height = 771;
  // h264
  auto component = (OMX_STRING)("OMX.qcom.video.encoder.avc");
  
  int err = OMX_GetHandle(&this->handle, component, this, &omx_callbacks);
}

#include <OMX_Component.h>
#include <OMX_Core.h>
#include <OMX_QCOMExtns.h>
#include "include/msm_media_info.h"
#include "queue.h"

struct OmxBuffer {
  OMX_BUFFERHEADERTYPE header;
  OMX_U8 data[];
};

class OmxEncoder {
  public:
    OmxEncoder();
    ~OmxEncoder();
    int encode_frame();

    // OMX callbacks
    static OMX_ERRORTYPE event_handler(OMX_HANDLETYPE component, OMX_PTR app_data, OMX_EVENTTYPE event, OMX_U32 data1, OMX_U32 data2, OMX_PTR event_data);
   static OMX_ERRORTYPE empty_buffer_done(OMX_HANDLETYPE component, OMX_PTR app_data, OMX_BUFFERHEADERTYPE *buffer);
   static OMX_ERRORTYPE fill_buffer_done(OMX_HANDLETYPE component, OMX_PTR app_data, OMX_BUFFERHEADERTYPE *buffer);

  private:
    const char* filename;
    int width, height, fps;

    std::mutex state_lock;
    std::condition_variable state_cv;
    OMX_STATETYPE state = OMX_StateLoaded;
    OMX_HANDLETYPE handle;

    SafeQueue<OMX_BUFFERHEADERTYPE *> free_in;
    SafeQueue<OMX_BUFFERHEADERTYPE *> done_out;
    SafeQueue<OmxBuffer *> to_write;
};

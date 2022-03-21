#include <OMX_Component.h>

class OmxEncoder {
  public:
    OmxEncoder();
    ~OmxEncoder();
    int encode_frame();
};

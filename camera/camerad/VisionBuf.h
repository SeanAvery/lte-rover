#include <stdint.h>
#include <string>

class VisionBuf
{
  public:
    size_t len =  0;
    void *addr = nullptr;
    uint64_t *frame_id;
    int fd = 0;
};
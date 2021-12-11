#pragma once

#include <stdint.h>
#include <cstring>

enum VisionStreamType {
  VISION_STREAM_RGB_FRONT,
  VISION_STREAM_RGB_BACK,
};

class VisionBuf {
  public:
    void *addr = nullptr;
    uint64_t *fram_id;
    int fd = 0;

    size_t width = 0;
    size_t height = 0;
    size_t stride = 0;

    // ion
    int handle;
    
    void allocate(size_t len);
    void init_rgb(size_t width, size_t height, size_t tride);
    int free();
};
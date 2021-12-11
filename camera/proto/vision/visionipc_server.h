#pragma once

#include <string>
#include <thread>
#include "visionbuf.h"

std::string get_endpoint_name(std::string name, VisionStreamType typ);

class VisionIpcServer {
  private:
    std::string name;
    std::thread listener_thread;

    void listener();

  public:
    VisionIpcServer(std::string name);
    ~VisionIpcServer();
    
    void create_bufferes(VisionStreamType type, size_t num_buffers, size_t width, size_t height);
    void send(VisionBuf *buf);
    void start_listener();
};
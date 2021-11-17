#include <iostream>
#include "Camera.cpp"

int main() {
  std::cout << "Camera process starting" << std::endl;

  Camera cam;
  cam.camera_init();
  // cam.camera_open();
  
  std::thread camera_thread(&Camera::camera_run, &cam);
  camera_thread.join();
  return 0;
}

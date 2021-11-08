#include <iostream>
#include "Camera.cpp"

int main() {
  std::cout << "Camera process starting" << std::endl;

  Camera cam;
  cam.camera_init();
  return 0;
}

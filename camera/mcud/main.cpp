#include <iostream>
#include <memory>
#include "mcu.cpp"

Mcu *usb_connect()
{
  std::unique_ptr<Mcu> mcu;
  try
  {
    mcu = std::make_unique<Mcu>();
  }
  catch (std::exception &e)
  {
    return nullptr;
  }
}

int main()
{
  std::cout << "setting up mcu daemon" << std::endl;
  
  return 0;
}



#pragma once

#include <vector>

#include "AimoKeyboardDriver.hpp"

class ORAKD {
public:
  ORAKD();
  ~ORAKD();
  std::vector<AimoKeyboardDriver *> getDevices() { return device_list; }

private:
  std::vector<AimoKeyboardDriver *> device_list = {};
};
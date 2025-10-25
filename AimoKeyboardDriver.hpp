#pragma once

#include <hidapi.h>

#include <map>
#include <string>

#include "ReadCallback.hpp"

#define ROCCAT_VULCAN_TKL_PRO_PID 0x311A

struct AimoKeyboardConfig {
  uint8_t protocol_version;
};

class AimoKeyboardDriver {
public:
  struct RGBColor {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
  };

  AimoKeyboardDriver(){};
  AimoKeyboardDriver(std::string name, std::vector<hid_device*> hiddev, uint16_t pid);
  ~AimoKeyboardDriver();

  AimoKeyboardConfig config;
  hid_device *ctrl_device;
  hid_device *event_device;
  hid_device *led_device;
  std::string name;

private:
  ReadCallback *cb;
  std::vector<uint8_t> await_response(uint8_t *command, uint8_t command_length);
};

static std::map<uint16_t, AimoKeyboardConfig> aimo_keyboard_config = {
    {ROCCAT_VULCAN_TKL_PRO_PID, {2}}};

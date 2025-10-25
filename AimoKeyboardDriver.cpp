#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <cstring>
#include <vector>
#include <algorithm>

#include <hidapi.h>

#include "AimoKeyboardDriver.hpp"
#include "ReadCallback.hpp"

AimoKeyboardDriver::AimoKeyboardDriver (std::string name, std::vector<hid_device*> hiddev, uint16_t pid) {
    this->name = name;
    ctrl_device = hiddev[0];
    event_device = hiddev[1];
    led_device = hiddev[2];
	config = aimo_keyboard_config[pid];

    // cb = new ReadCallback(hiddev[1]);
}

AimoKeyboardDriver::~AimoKeyboardDriver() {
    delete cb;
    hid_close(ctrl_device);
    hid_close(event_device);
    hid_close(led_device);
}
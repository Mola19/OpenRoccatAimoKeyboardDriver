#include "ORAKD.hpp"

#include <hidapi.h>

#include <cstdio>
#include <cstring>
#include <iostream>

#include "AimoKeyboardDriver.hpp"
#include "Devices.hpp"

ORAKD::ORAKD() {
	int hid_ret = hid_init();

	if (hid_ret == -1) {
		std::cout << "HIDAPI could not init" << std::endl;
		return;
	}

	hid_device_info *dev_list = hid_enumerate(0, 0);

	if (dev_list == NULL) {
		std::cout << "HIDAPI could not fetch devices" << std::endl;
		return;
	}

	// this algorithm only allows for one device of the same type
	// it isn't comlicated to change it to allow multiple,
	// but i want to change to a per usb based enumeration anyway

	for (unsigned int device_index = 0; device_index < hid_devices.size(); device_index++) {
		std::vector<hid_device *> hiddevs = {};
		for (unsigned int usage_index = 0; usage_index < hid_devices[device_index].usages.size();
			 usage_index++) {
			for (auto local_dev_list = dev_list; local_dev_list;
				 local_dev_list = local_dev_list->next) {
				if (hid_devices[device_index].compare(local_dev_list, 0)) {
					hid_device *dev = hid_open_path(local_dev_list->path);

					if (dev == NULL) {
						printf("HIDAPI could not open device \"%s\"",
							   hid_devices[device_index].name.c_str());
						continue;
					}

					hiddevs.push_back(dev);
				}
			}
		}

		if (hiddevs.size() == hid_devices[device_index].usages.size()) {
			AimoKeyboardDriver *mouse = new AimoKeyboardDriver(
				hid_devices[device_index].name, hiddevs, hid_devices[device_index].pid);
			device_list.push_back(mouse);
		}
	}

	hid_free_enumeration(dev_list);

	hid_exit();
}

ORAKD::~ORAKD() {
	for (AimoKeyboardDriver *mouse : device_list) {
		mouse->~AimoKeyboardDriver();
	}
}

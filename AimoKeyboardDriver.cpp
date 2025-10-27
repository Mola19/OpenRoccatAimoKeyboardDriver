#include "AimoKeyboardDriver.hpp"

#include <hidapi.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <expected>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "ReadCallback.hpp"

AimoKeyboardDriver::AimoKeyboardDriver(std::string name, std::vector<hid_device*> hiddev,
									   uint16_t pid) {
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

AimoKeyboardDriver::Error<AimoKeyboardDriver::DeviceInfo> AimoKeyboardDriver::get_device_info() {
	uint8_t packet_length = (config.protocol_version == 1) ? 8 : 9;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0F : 0x09;

	uint8_t* buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");
	
	if (buf[0] != report_id || buf[1] != packet_length)
		return std::unexpected("packet header is malformed");

	if (config.protocol_version == 1 && !check_checksum(buf, packet_length, 1))
		return std::unexpected("checksum didn't match");

	char version[5];
	snprintf(version, 5, "%d.%02d", buf[2] / 100, buf[2] % 100);

	if (config.protocol_version == 1) {
		return AimoKeyboardDriver::DeviceInfo{
			.version_string = version,
			.minor_version = static_cast<uint8_t>(buf[2] % 100),
			.major_version = static_cast<uint8_t>(buf[2] / 100),
			.physical_layout = buf[5],
			.visual_layout = buf[6],
			._unknown3 = buf[3],
			._unknown4 = buf[4],
		};
	} else {
		return AimoKeyboardDriver::DeviceInfo{
			.version_string = version,
			.minor_version = static_cast<uint8_t>(buf[2] % 100),
			.major_version = static_cast<uint8_t>(buf[2] / 100),
			.physical_layout = buf[6],
			.visual_layout = buf[7],
			._unknown3 = buf[3],
			._unknown4 = buf[4],
			._unknown5 = buf[5]};
	}

    delete[] buf;
}

AimoKeyboardDriver::Error<uint8_t> AimoKeyboardDriver::get_busy_state() {
	uint8_t packet_length = (config.protocol_version == 1) ? 3 : 4;

	uint8_t* buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = 0x04;
    int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");
	
	// this packet doesn't send packet length 
	if (buf[0] != 0x04)
		return std::unexpected("packet header is malformed");

    uint8_t state = buf[1];
	delete[] buf;
	return state;
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::wait_until_ready() {
	// not waiting for at least 2 miliseconds will yield relatively frequent errors
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	for(uint8_t i = 0; i < 100; i++)
    {
		auto res = get_busy_state();
		if (!res) {
			return res.error();
		} else if (res.value() == AimoKeyboardDriver::READY) {
			return std::nullopt;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
	return "Timeout";
}

bool AimoKeyboardDriver::check_checksum(uint8_t* buf, int size, uint8_t checksum_size) {
	int sum = 0;

	for (int i = 0; i < size - checksum_size; i++) {
		sum += buf[i];
	}

	sum = sum % (1 << (checksum_size * 8));

	int checksum = 0;

	for (int i = size - checksum_size; i < size; i++) {
		checksum += buf[i] * (1 << ((size - i - 1) * 8));
	}

	return checksum == sum;
}

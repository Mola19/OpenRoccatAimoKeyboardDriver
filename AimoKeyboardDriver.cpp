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

#include "KeyMaps.hpp"
#include "ReadCallback.hpp"

AimoKeyboardDriver::AimoKeyboardDriver(
	std::string name, std::vector<hid_device *> hiddev, uint16_t pid
) {
	this->name = name;
	this->ctrl_device = hiddev[0];
	this->event_device = hiddev[1];
	this->led_device = hiddev[2];
	this->config = aimo_keyboard_config[pid];
	this->pid = pid;

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

	uint8_t *buf = new uint8_t[packet_length];
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
			// ._unknown3 = buf[3],
			// ._unknown4 = buf[4],
		};
	} else {
		return AimoKeyboardDriver::DeviceInfo{
			.version_string = version,
			.minor_version = static_cast<uint8_t>(buf[2] % 100),
			.major_version = static_cast<uint8_t>(buf[2] / 100),
			.physical_layout = buf[6],
			.visual_layout = buf[7],
			// ._unknown3 = buf[3],
			// ._unknown4 = buf[4],
			// ._unknown5 = buf[5]
		};
	}

	delete[] buf;
}

AimoKeyboardDriver::Error<uint8_t> AimoKeyboardDriver::get_busy_state() {
	uint8_t packet_length = (config.protocol_version == 1) ? 3 : 4;

	uint8_t *buf = new uint8_t[packet_length];
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
	for (uint8_t i = 0; i < 100; i++) {
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

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_page_to_read(uint8_t profile, uint8_t page_or_key, bool is_macro) {
	if (config.protocol_version == 2)
		return "can't use this function with gen 2";

	uint8_t buf[4] = {0x04, profile, page_or_key, is_macro};
	int written = hid_send_feature_report(ctrl_device, buf, 4);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::ProfileInfo> AimoKeyboardDriver::get_profile_info() {
	uint8_t buf[4] = {};
	memset(buf, 0x00, 4);

	buf[0] = 0x05;
	int read = hid_get_feature_report(ctrl_device, buf, 4);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != 0x05 || buf[1] != 0x04)
		return std::unexpected("packet header is malformed");

	return AimoKeyboardDriver::ProfileInfo{
		.active_profile = buf[2],
		.amount_profiles = buf[3],
	};
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_profile_info(AimoKeyboardDriver::ProfileInfo info) {
	return set_profile_info(info.active_profile, info.amount_profiles);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_profile_info(uint8_t active_profile, uint8_t amount_profiles) {
	uint8_t buf[4] = {0x05, 0x04, active_profile, amount_profiles};
	int written = hid_send_feature_report(ctrl_device, buf, 4);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::SoftwareStateGen1>
AimoKeyboardDriver::get_software_state_gen1() {
	if (config.protocol_version == 2)
		return std::unexpected("can't use this function with gen 2");

	uint8_t buf[3] = {};
	memset(buf, 0x00, 3);

	buf[0] = 0x15;
	int read = hid_get_feature_report(ctrl_device, buf, 3);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	// this packet doesn't send packet length
	if (buf[0] != 0x15)
		return std::unexpected("packet header is malformed");

	return AimoKeyboardDriver::SoftwareStateGen1{
		.mute_light_on = (bool)buf[1],
		.software_control_enabled = (bool)buf[2],
	};
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_software_state_gen1(AimoKeyboardDriver::SoftwareStateGen1 state) {
	return set_software_state_gen1(state.mute_light_on, state.software_control_enabled);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_software_state_gen1(bool mute_light_on, bool software_control_enabled) {
	uint8_t buf[3] = {0x15, mute_light_on, software_control_enabled};
	int written = hid_send_feature_report(ctrl_device, buf, 3);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::SoftwareStateGen2>
AimoKeyboardDriver::get_software_state_gen2() {
	if (config.protocol_version == 1)
		return std::unexpected("can't use this function with gen 1");

	uint8_t buf[16] = {};
	memset(buf, 0x00, 16);

	buf[0] = 0x0D;
	int read = hid_get_feature_report(ctrl_device, buf, 16);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	// this packet doesn't send packet length
	if (buf[0] != 0x0D || buf[1] != 0x10)
		return std::unexpected("packet header is malformed");

	return AimoKeyboardDriver::SoftwareStateGen2{
		.mute_light_on = (bool)buf[2],
		.sleep_enabled = (bool)buf[5],
		.minutes_until_sleep = buf[5],
	};
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_software_state_gen2(AimoKeyboardDriver::SoftwareStateGen2 state) {
	return set_software_state_gen2(
		state.mute_light_on, state.sleep_enabled, state.minutes_until_sleep
	);
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_software_state_gen2(
	bool mute_light_on, bool sleep_enabled, uint8_t minutes_until_sleep
) {
	uint8_t sleep_val = (sleep_enabled) ? minutes_until_sleep : 0;

	// this packet contains a lot of values that are unknown
	// they make no bvious difference, so i just took the values swarm sends,
	// but it also works with any values
	uint8_t buf[16] = {0x0D, 0x10, mute_light_on, 0x00, 0x02, sleep_val, 0x45, 0x00,
					   0x00, 0x00, 0x00,          0x00, 0x00, 0x00,      0x00, 0x00};
	int written = hid_send_feature_report(ctrl_device, buf, 16);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<bool> AimoKeyboardDriver::get_software_control_state_gen2() {
	if (config.protocol_version == 1)
		return std::unexpected("can't use this function with gen 1");

	uint8_t buf[5] = {};
	memset(buf, 0x00, 5);

	buf[0] = 0x0E;
	int read = hid_get_feature_report(ctrl_device, buf, 5);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != 0x0E || buf[1] != 0x05)
		return std::unexpected("packet header is malformed");

	return buf[2];
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_software_control_state_gen2(bool software_control_enabled) {
	uint8_t buf[5] = {0x0E, 0x05, software_control_enabled, 0x00, 0x00};
	int written = hid_send_feature_report(ctrl_device, buf, 5);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::SoftwareState>
AimoKeyboardDriver::get_software_state() {
	if (config.protocol_version == 1) {
		auto res = get_software_state_gen1();
		if (!res)
			return std::unexpected(res.error());

		return SoftwareState{
			.software_control_enabled = res->software_control_enabled,
			.mute_light_on = res->mute_light_on
		};
	} else {
		auto res1 = get_software_control_state_gen2();
		if (!res1)
			return std::unexpected(res1.error());

		auto res2 = get_software_state_gen2();
		if (!res2)
			return std::unexpected(res2.error());

		return SoftwareState{
			.software_control_enabled = res1.value(),
			.mute_light_on = res2->mute_light_on,
			.sleep_enabled = res2->sleep_enabled,
			.minutes_until_sleep = res2->minutes_until_sleep
		};
	}
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_software_state(AimoKeyboardDriver::SoftwareState state) {
	return set_software_state(
		state.software_control_enabled, state.mute_light_on, state.sleep_enabled,
		state.minutes_until_sleep
	);
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_software_state(
	bool software_control_enabled, bool mute_light_on, std::optional<bool> sleep_enabled,
	std::optional<uint8_t> minutes_until_sleep
) {
	if (config.protocol_version == 1) {
		return set_software_state_gen1(mute_light_on, software_control_enabled);
	} else {
		if (!sleep_enabled || !minutes_until_sleep)
			return "sleep variables need to be set for gen 2";

		auto err = set_software_control_state_gen2(software_control_enabled);
		if (err)
			return err;

		auto err2 = wait_until_ready();
		if (err2)
			return err2;

		return set_software_state_gen2(
			mute_light_on, sleep_enabled.value(), minutes_until_sleep.value()
		);
	}
};

AimoKeyboardDriver::Error<bool> AimoKeyboardDriver::get_lighting_state() {
	if (config.protocol_version == 2)
		return std::unexpected("can't use this function with gen 2");

	uint8_t buf[8] = {};
	memset(buf, 0x00, 8);

	buf[0] = 0x13;
	int read = hid_get_feature_report(ctrl_device, buf, 8);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != 0x13 || buf[0] != 0x13)
		return std::unexpected("packet header is malformed");

	return (bool)buf[4];
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_lighting_state(bool off) {
	// no idea if the rest of does anything,
	// [5] is not even returned when getting information ([2] though)
	uint8_t buf[8] = {0x13, 0x08, 0x00, 0x00, off, 0x45, 0x00, 0x00};
	int written = hid_send_feature_report(ctrl_device, buf, 8);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::LightingInfo> AimoKeyboardDriver::get_lighting() {
	uint16_t packet_length = 0;
	uint16_t block_size = 0;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0D : 0x11;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
			packet_length = 443;
			block_size = 12;
			break;
		default:
			return std::unexpected("This device is not supported by the function");
	}

	uint8_t header_length = (packet_length > 255) ? 2 : 1;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	// this packet doesn't send packet length
	if (header_length == 1) {
		if (buf[0] != report_id || buf[1] != packet_length)
			return std::unexpected("packet header is malformed");
	} else {
		if (buf[0] != report_id || buf[1] != packet_length % 0x100 ||
			buf[2] != packet_length / 0x100)
			return std::unexpected("packet header is malformed");
	}

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	uint8_t brightness_index = (config.protocol_version == 1) ? 5 : 4;
	uint8_t colours_start_index = 7 + header_length;

	std::vector<RGBColor> colors;
	colors.resize(config.led_length);

	for (const auto &[key, value] : AimoKeyMaps::Vulcan100) {
		int block = (int)(key / block_size) * block_size;
		int offset = block * 3 + key % block_size + colours_start_index;

		colors[key] = {
			.red = buf[offset],
			.green = buf[offset + block_size],
			.blue = buf[offset + block_size * 2],
		};
	}

	LightingInfo info = {
		.profile = (config.protocol_version == 1) ? std::nullopt
												  : std::make_optional(buf[1 + header_length]),
		.mode = buf[2 + header_length],
		.speed = buf[3 + header_length],
		.brightness = buf[brightness_index + header_length],
		.theme = (uint8_t)(buf[6 + header_length] & 0b0111'1111),
		.is_custom_color = !(bool)(buf[6 + header_length] & 0b1000'0000),
		.colors = colors,
	};

	delete[] buf;

	return info;
}

bool AimoKeyboardDriver::check_checksum(uint8_t *buf, int size, uint8_t checksum_size) {
	int sum = 0;

	for (int i = 0; i < size - checksum_size; i++) {
		sum += buf[i];
	}

	sum = sum % (1 << (checksum_size * 8));

	int checksum = 0;

	for (int i = size - checksum_size; i < size; i++) {
		uint8_t checksum_index = i - (size - checksum_size);
		checksum += buf[i] * (1 << (checksum_index * 8));
	}

	return checksum == sum;
}

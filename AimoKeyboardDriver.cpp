#include "AimoKeyboardDriver.hpp"

#include <format>
#include <hidapi.h>

#include <cstdint>
#include <cstring>
#include <expected>
#include <optional>
#include <string>
#include <thread>
#include <vector>

AimoKeyboardDriver::AimoKeyboardDriver(
	std::string name, std::vector<hid_device *> hiddev, uint16_t pid
) {
	this->name = name;
	this->ctrl_device = hiddev[0];
	this->event_device = hiddev[1];
	this->led_device = hiddev[2];
	this->config = aimo_keyboard_config[pid];
	this->pid = pid;

	event = new EventListener(hiddev[1], config.protocol_version, pid);
}

AimoKeyboardDriver::~AimoKeyboardDriver() {
	delete event;
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
		} else if (res.value() == AimoKeyboardDriver::ERROR) {
			return "unkown error with sent packet";
		} else if (res.value() == AimoKeyboardDriver::CHECKSUM_WRONG) {
			return "error with checksum of sent packet";
		}
		// loop if AimoKeyboardDriver::BUSY
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
	return "Timeout while waiting for packet";
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_page_to_read(uint8_t profile, uint8_t page_or_key, bool is_macro) {
	if (config.protocol_version == 1)
		return "can't use this function with gen 1";

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

AimoKeyboardDriver::VoidError AimoKeyboardDriver::reset_all_profiles() {
	if (config.protocol_version == 1) {
		uint8_t buf[3] = {0x11, 0x03, 0x04};
		int written = hid_send_feature_report(ctrl_device, buf, 3);

		if (written == -1)
			return "HIDAPI Error";

		return std::nullopt;
	} else {
		uint8_t buf[9] = {0x09, 0x09, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
		int written = hid_send_feature_report(ctrl_device, buf, 9);

		if (written == -1)
			return "HIDAPI Error";

		return std::nullopt;
	}
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
		case ROCCAT_VULCAN_TKL_PID:
			packet_length = 443;
			block_size = 12;
			break;
		case ROCCAT_VULCAN_TKL_PRO_PID:
			packet_length = 299;
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

	std::vector<RGBColor> colors(config.led_length);

	for (const auto &[key, value] : config.led_map) {
		int block = (int)(value / block_size) * block_size;
		int offset = block * 3 + value % block_size + colours_start_index;

		colors[value] = {
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

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_lighting(LightingInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting lighting";

	return set_lighting(
		info.profile.value(), info.mode, info.speed, info.brightness, info.theme,
		info.is_custom_color, info.colors
	);
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_lighting(
	uint8_t profile, uint8_t mode, uint8_t speed, uint8_t brightness, uint8_t theme,
	uint8_t is_custom_color, std::vector<RGBColor> colors
) {
	if (speed < 1 || speed > 11)
		return std::format("speed must be between 1 and 11, but is {}", speed);

	if (brightness < 1 || brightness > 69)
		return std::format("brightness must be between 1 and 69, but is {}", brightness);

	if (colors.size() != config.led_length)
		return std::format(
			"colors vector size is {}, but it must be {} for this device", colors.size(),
			config.led_length
		);

	uint16_t packet_length = 0;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0D : 0x11;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
		case ROCCAT_VULCAN_TKL_PID:
			packet_length = 443;
			break;
		case ROCCAT_VULCAN_TKL_PRO_PID:
			packet_length = 299;
			break;
		default:
			return "This device is not supported by the function";
	}

	uint8_t header_length = (packet_length > 255) ? 2 : 1;

	uint8_t brightness_index = (config.protocol_version == 1) ? 5 : 4;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;

	if (header_length == 1) {
		buf[1] = (uint8_t)packet_length;
	} else {
		buf[1] = packet_length % 256;
		buf[2] = packet_length / 256;
	}

	buf[1 + header_length] = profile;
	buf[2 + header_length] = mode;
	buf[3 + header_length] = speed;
	buf[brightness_index + header_length] = brightness;
	buf[6 + header_length] = (!is_custom_color << 7) + theme % 0b0111'1111;

	auto color_bytes = generate_color_bytes(colors);
	memcpy(buf + 7 + header_length, color_bytes.data(), config.led_length * 3);

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	delete[] buf;
	return std::nullopt;
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_direct_lighting(std::vector<RGBColor> colors) {
	uint16_t total_length;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
		case ROCCAT_VULCAN_TKL_PID:
			total_length = 436;
			break;
		case ROCCAT_VULCAN_TKL_PRO_PID:
			total_length = 308;
			break;
		default:
			return "This device is not supported by the function";
	}

	auto color_bytes = generate_color_bytes(colors);
	color_bytes.push_back(0xDE);

	int i = 1;
	uint16_t offset = 0;
	uint8_t buf[65] = {};
	while (config.led_length * 3 > offset) {
		memset(buf, 0x00, 65);

		uint8_t bytes_to_send = 0;

		if (i == 1) {
			buf[0] = 0x00;
			buf[1] = 0xA1;
			buf[2] = 0x01;

			if (config.protocol_version == 1) {
				// big endian
				buf[3] = total_length / 256;
				buf[4] = total_length % 256;
			} else {
				// little endian
				buf[3] = total_length % 256;
				buf[4] = total_length / 256;
			}

			bytes_to_send = 60;
		} else {
			buf[0] = 0x00;
			if (config.protocol_version == 2) {
				buf[1] = 0xA1;
				buf[2] = i;

				bytes_to_send = 60;
			} else {
				bytes_to_send = 64;
			}
		}

		uint8_t packet_offset = (65 - bytes_to_send);
		uint8_t actual_bytes =
			std::min((uint16_t)(config.led_length * 3 - offset), (uint16_t)bytes_to_send);
		memcpy(buf + packet_offset, color_bytes.data() + offset, actual_bytes);

		hid_write(led_device, buf, 65);

		i++;
		offset += actual_bytes;
	}

	return std::nullopt;
}

std::vector<uint8_t> AimoKeyboardDriver::generate_color_bytes(std::vector<RGBColor> colors) {
	uint16_t block_size = 0;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0D : 0x11;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
		case ROCCAT_VULCAN_TKL_PRO_PID:
		case ROCCAT_VULCAN_TKL_PID:
			block_size = 12;
			break;
	}

	std::vector<uint8_t> buf(config.led_length * 3);

	for (const auto &[key, value] : config.led_map) {
		int block = (value / block_size) * block_size;
		int offset = block * 3 + value % block_size;

		buf[offset] = colors[value].red;
		buf[offset + block_size] = colors[value].green;
		buf[offset + block_size * 2] = colors[value].blue;
	}

	return buf;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::GamemodeRemapInfo>
AimoKeyboardDriver::get_gamemode_remap() {
	uint16_t packet_length = 0;
	uint8_t report_id = 0x06;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
		case ROCCAT_VULCAN_TKL_PID:
			packet_length = 133;
			break;
		case ROCCAT_VULCAN_TKL_PRO_PID:
			packet_length = 134;
			break;
		default:
			return std::unexpected("This device is not supported by the function");
	}

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != packet_length)
		return std::unexpected("packet header is malformed");

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	uint8_t offset = (config.protocol_version == 1) ? 3 : 4;

	std::vector<uint8_t> values(buf + offset, buf + offset + config.remap_length);

	GamemodeRemapInfo info = {
		.profile = (config.protocol_version == 1) ? std::nullopt : std::make_optional(buf[2]),
		.values = values
	};

	delete[] buf;

	return info;
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_gamemode_remap(GamemodeRemapInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting remap";

	return set_gamemode_remap(info.profile.value(), info.values);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_gamemode_remap(uint8_t profile, std::vector<uint8_t> values) {
	if (values.size() != config.remap_length)
		return std::format(
			"values vector size is {}, but it must be {} for this device", values.size(),
			config.remap_length
		);

	uint16_t packet_length = 0;
	uint8_t report_id = 0x06;

	switch (pid) {
		case ROCCAT_VULCAN_100_AIMO_PID:
		case ROCCAT_VULCAN_TKL_PID:
			packet_length = 133;
			break;
		case ROCCAT_VULCAN_TKL_PRO_PID:
			packet_length = 134;
			break;
		default:
			return "This device is not supported by the function";
	}

	uint8_t header_length = (packet_length > 255) ? 4 : 3;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	buf[1] = packet_length;
	buf[2] = profile;

	// for some reasom gen 2 has two different remnapr packet, but 0 does nothing
	// you can get 0, it seems like it's just the default (non gamemode) map, that is unchangeable
	if (header_length == 4) {
		buf[3] = 0x01;
	}

	memcpy(buf + header_length, values.data(), config.remap_length);

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	delete[] buf;
	return std::nullopt;
}

AimoKeyboardDriver::Error<bool> AimoKeyboardDriver::get_easyshift() {
	uint8_t report_id = (config.protocol_version == 1) ? 0x16 : 0x10;

	uint8_t buf[16] = {};
	memset(buf, 0x00, 16);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, 16);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != 0x10)
		return std::unexpected("packet header is malformed");

	return (bool)buf[3];
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_easyshift(bool active) {
	uint8_t report_id = (config.protocol_version == 1) ? 0x16 : 0x10;

	uint8_t buf[16] = {report_id, 0x10, 0x00, active, 0x00, 0x00, 0x00, 0x00,
					   0x00,      0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00};
	int written = hid_send_feature_report(ctrl_device, buf, 16);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::LongRemapInfo>
AimoKeyboardDriver::get_easyshift_remap() {
	uint8_t code_size = (config.protocol_version == 1) ? 3 : 4;
	uint16_t packet_length = (config.protocol_version == 1) ? 65 : 85;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0B : 0x0C;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != packet_length)
		return std::unexpected("packet header is malformed");

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	auto values = le_array_to_uint_vec(buf + 3, 20, code_size, config.protocol_version != 1);

	LongRemapInfo info = {
		.profile = (config.protocol_version == 1) ? std::nullopt : std::make_optional(buf[2]),
		.values = values
	};

	delete[] buf;

	return info;
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_easyshift_remap(LongRemapInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting remap";

	return set_easyshift_remap(info.profile.value(), info.values);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_easyshift_remap(uint8_t profile, std::vector<uint32_t> values) {
	if (values.size() != 20)
		return std::format(
			"values vector size is {}, but it must be 20 for this device", values.size()
		);

	uint8_t code_size = (config.protocol_version == 1) ? 3 : 4;
	uint16_t packet_length = (config.protocol_version == 1) ? 65 : 85;
	uint8_t report_id = (config.protocol_version == 1) ? 0x0B : 0x0C;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	buf[1] = packet_length;
	buf[2] = profile;

	auto temp = uint_vec_to_le_array(values, code_size, config.protocol_version != 1);

	memcpy(buf + 3, temp.data(), 20 * code_size);

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	delete[] buf;
	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::LongRemapInfo> AimoKeyboardDriver::get_fn_remap() {
	uint8_t code_size = (config.protocol_version == 1) ? 3 : 4;
	uint16_t packet_length = config.fn_map.size() * code_size + 5;
	uint8_t report_id = (config.protocol_version == 1) ? 0x07 : 0x0A;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != packet_length)
		return std::unexpected("packet header is malformed");

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	auto values = le_array_to_uint_vec(
		buf + 3, config.fn_map.size(), code_size, config.protocol_version != 1
	);

	LongRemapInfo info = {
		.profile = (config.protocol_version == 1) ? std::nullopt : std::make_optional(buf[2]),
		.values = values
	};

	delete[] buf;

	return info;
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_fn_remap(LongRemapInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting remap";

	return set_fn_remap(info.profile.value(), info.values);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_fn_remap(uint8_t profile, std::vector<uint32_t> values) {
	if (values.size() != config.fn_map.size())
		return std::format(
			"values vector size is {}, but it must be {} for this device", values.size(),
			config.fn_map.size()
		);

	uint8_t code_size = (config.protocol_version == 1) ? 3 : 4;
	uint16_t packet_length = config.fn_map.size() * code_size + 5;
	uint8_t report_id = (config.protocol_version == 1) ? 0x07 : 0x0A;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	buf[1] = packet_length;
	buf[2] = profile;

	auto temp = uint_vec_to_le_array(values, code_size, config.protocol_version != 1);

	memcpy(buf + 3, temp.data(), config.fn_map.size() * code_size);

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	delete[] buf;
	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::LongRemapInfo>
AimoKeyboardDriver::get_fn_extra_remap() {
	if (!config.fn_extra_map)
		return std::unexpected("can't use this function with this keyboard");

	uint8_t code_size = 3;
	uint16_t packet_length = 43;
	uint8_t report_id = 0x09;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != packet_length)
		return std::unexpected("packet header is malformed");

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	auto values = le_array_to_uint_vec(
		buf + 3, config.fn_extra_map.value().size(), code_size, config.protocol_version != 1
	);

	LongRemapInfo info = {
		.profile = (config.protocol_version == 1) ? std::nullopt : std::make_optional(buf[2]),
		.values = values
	};

	delete[] buf;

	return info;
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_fn_extra_remap(LongRemapInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting remap";

	return set_fn_remap(info.profile.value(), info.values);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_fn_extra_remap(uint8_t profile, std::vector<uint32_t> values) {
	if (!config.fn_extra_map)
		return "can't use this function with this keyboard";

	if (values.size() != config.fn_extra_map.value().size())
		return std::format(
			"values vector size is {}, but it must be {} for this device", values.size(),
			config.fn_extra_map.value().size()
		);

	// this packet has a bunch of garbage bytes that can be ignored
	uint8_t code_size = 3;
	uint16_t packet_length = 43;
	uint8_t report_id = 0x09;

	uint8_t *buf = new uint8_t[packet_length];
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	buf[1] = packet_length;
	buf[2] = profile;

	auto temp = uint_vec_to_le_array(values, code_size, config.protocol_version != 1);

	memcpy(buf + 3, temp.data(), config.fn_extra_map.value().size() * code_size);

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	delete[] buf;
	return std::nullopt;
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::CapslockRemapInfo>
AimoKeyboardDriver::get_capslock_remap() {
	if (config.protocol_version == 1) {
		uint8_t report_id = 0x0A;
		const uint8_t packet_length = 8;

		uint8_t buf[packet_length] = {};
		memset(buf, 0x00, packet_length);

		buf[0] = report_id;
		int read = hid_get_feature_report(ctrl_device, buf, packet_length);

		if (read == -1)
			return std::unexpected("HIDAPI Error");

		if (buf[0] != report_id || buf[1] != packet_length)
			return std::unexpected("packet header is malformed");

		if (!check_checksum(buf, packet_length, 2))
			return std::unexpected("checksum didn't match");

		CapslockRemapInfo info = {.profile = std::nullopt, .capslock_value = buf[3]};

		return info;
	} else {
		uint8_t report_id = 0x07;
		const uint8_t packet_length = 11;

		uint8_t buf[packet_length] = {};
		memset(buf, 0x00, packet_length);

		buf[0] = report_id;
		int read = hid_get_feature_report(ctrl_device, buf, packet_length);

		if (read == -1)
			return std::unexpected("HIDAPI Error");

		if (buf[0] != report_id || buf[1] != packet_length)
			return std::unexpected("packet header is malformed");

		if (!check_checksum(buf, packet_length, 2))
			return std::unexpected("checksum didn't match");

		CapslockRemapInfo info = {
			.profile = buf[2],
			.capslock_value = 0 + ((uint32_t)buf[6] << 24) + ((uint32_t)buf[5] << 16) +
							  ((uint32_t)buf[4] << 8) + buf[3]
		};

		return info;
	}
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_capslock_remap(CapslockRemapInfo info) {
	if (!info.profile)
		return "profile needs to be set when setting remap";

	return set_capslock_remap(info.profile.value(), info.capslock_value);
}

AimoKeyboardDriver::VoidError
AimoKeyboardDriver::set_capslock_remap(uint8_t profile, uint32_t capslock_value) {
	if (config.protocol_version == 1) {
		uint8_t report_id = 0x0A;
		const uint8_t packet_length = 8;

		uint8_t buf[packet_length] = {report_id, packet_length,
									  profile,   (uint8_t)capslock_value /*deliberate overflow*/,
									  0x00,      0x00,
									  0x00,      0x00};

		generate_checksum(buf, packet_length, 2);

		int written = hid_send_feature_report(ctrl_device, buf, packet_length);

		if (written == -1)
			return "HIDAPI Error";

		return std::nullopt;
	} else {
		uint8_t report_id = 0x07;
		const uint8_t packet_length = 11;

		uint8_t buf[packet_length] = {
			report_id,
			packet_length,
			profile,
			(uint8_t)capslock_value, /*deliberate overflow*/
			(uint8_t)(capslock_value >> 8),
			(uint8_t)(capslock_value >> 16),
			(uint8_t)(capslock_value >> 24),
			0x00,
			0x00,
			0x00,
			0x00
		};

		generate_checksum(buf, packet_length, 2);

		int written = hid_send_feature_report(ctrl_device, buf, packet_length);

		if (written == -1)
			return "HIDAPI Error";

		return std::nullopt;
	}
}

AimoKeyboardDriver::Error<AimoKeyboardDriver::MacroInfo> AimoKeyboardDriver::get_macro() {
	if (config.protocol_version == 1)
		return std::unexpected("can't use this function with gen 1");

	const uint16_t packet_length = 1044;
	uint8_t report_id = 0x08;

	uint8_t buf[packet_length] = {};
	memset(buf, 0x00, packet_length);

	buf[0] = report_id;
	int read = hid_get_feature_report(ctrl_device, buf, packet_length);

	if (read == -1)
		return std::unexpected("HIDAPI Error");

	if (buf[0] != report_id || buf[1] != packet_length % 256 || buf[2] != packet_length >> 8)
		return std::unexpected("packet header is malformed");

	if (!check_checksum(buf, packet_length, 2))
		return std::unexpected("checksum didn't match");

	std::vector<MacroStep> steps;

	for (int i = 0x52; i < 1041; i += 2) {
		uint32_t delay = 0;

		if (buf[i] == 0x00) {
			// step is empty, no more steps should follow
			if (buf[i + 1] == 0x00)
				break;
			// step with long duration will follow
			else if (buf[i + 1] == 0x01) {
				if (i > 1037)
					return std::unexpected("macro in malformed");

				delay = (buf[i + 2] + (buf[i + 3] << 8)) * 20;
				i += 4;
			}
		}

		steps.push_back(
			{.is_release = (bool)(buf[i] >> 7),
			 .keycode = buf[i + 1],
			 .delay = static_cast<uint32_t>(delay + (buf[i] & 0b01111111))}
		);
	}

	return MacroInfo{
		.profile = buf[3],
		.key_id = buf[4],
		.foldername_utf8 = std::string(buf + 0x06, buf + 0x2E),
		.macroname_utf8 = std::string(buf + 0x2E, buf + 0x4E),
		.repeat = buf[0x50],
		.steps = steps
	};
}

AimoKeyboardDriver::VoidError AimoKeyboardDriver::set_macro(
	uint8_t profile, uint8_t key_id, std::string foldername_utf8, std::string macroname_utf8,
	uint8_t repeat, std::vector<MacroStep> steps
) {
	if (config.protocol_version == 1)
		return "can't use this function with gen 1";

	const uint16_t packet_length = 1044;
	uint8_t report_id = 0x08;

	uint8_t buf[packet_length] = {};
	memset(buf, 0x00, packet_length);

	buf[0x00] = report_id;
	buf[0x01] = packet_length % 256;
	buf[0x02] = packet_length >> 8;
	buf[0x03] = profile;
	buf[0x04] = key_id;
	buf[0x05] = 0x01; // unkown meaning

	memcpy(buf + 0x06, foldername_utf8.data(), 40);
	memcpy(buf + 0x2E, macroname_utf8.data(), 32);

	buf[0x4E] = 0x01; // unkown meaning
	buf[0x4F] = 0x00;
	buf[0x50] = repeat;
	buf[0x51] = 0x00;

	uint16_t offset = 0x52;

	for (auto step : steps) {
		if (step.delay > 1310827)
			return "delay is bigger than 1310827ms";

		uint32_t delay = step.delay;

		if (delay > 127) {
			if (offset > 1036)
				throw "macro has too many steps";

			uint8_t small = delay % 20;
			uint16_t big = delay / 20;

			if (small <= 7) {
				delay = small + 120;
				big -= 6;
			} else {
				delay = small + 100;
				big -= 5;
			}

			buf[offset] = 0x00;
			buf[offset + 1] = 0x01;
			buf[offset + 2] = big % 0x100;
			buf[offset + 3] = big >> 8;
			offset += 4;
		}

		if (offset > 1040)
			throw "macro has too many steps";

		buf[offset] = (step.is_release << 7) + delay;
		buf[offset + 1] = step.keycode;
		offset += 2;
	}

	generate_checksum(buf, packet_length, 2);

	int written = hid_send_feature_report(ctrl_device, buf, packet_length);

	if (written == -1)
		return "HIDAPI Error";

	return std::nullopt;
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

void AimoKeyboardDriver::generate_checksum(uint8_t *buf, int size, uint8_t checksum_size) {
	int sum = 0;

	for (int i = 0; i < size - checksum_size; i++) {
		sum += buf[i];
	}

	for (int i = size - checksum_size; i < size; i++) {
		uint8_t checksum_index = i - (size - checksum_size);
		// intentional overflow
		buf[i] = sum >> (checksum_index * 8);
	}
}

std::vector<uint8_t>
AimoKeyboardDriver::uint_vec_to_le_array(std::vector<uint32_t> vec, uint8_t bytes, bool is_le) {
	std::vector<uint8_t> arr(vec.size() * bytes);

	for (size_t i = 0; i < vec.size(); i++) {
		if (is_le) {
			for (uint8_t j = 0; j < bytes; j++) {
				// intentional overflow
				arr[i * bytes + j] = vec[i] >> (j * 8);
			}
		} else {
			for (uint8_t j = bytes - 1; j < bytes; j--) {
				// intentional overflow
				arr[i * bytes + bytes - 1 - j] = vec[i] >> (j * 8);
			}
		}
	}

	return arr;
}

std::vector<uint32_t>
AimoKeyboardDriver::le_array_to_uint_vec(uint8_t *buf, int out_size, uint8_t bytes, bool is_le) {
	std::vector<uint32_t> arr(out_size);

	for (size_t i = 0; i < out_size; i++) {
		if (is_le) {
			for (uint8_t j = 0; j < bytes; j++) {
				// intentional overflow
				arr[i] += buf[i * bytes + j] << (j * 8);
			}
		} else {
			for (uint8_t j = bytes - 1; j < bytes; j--) {
				// intentional overflow
				arr[i] += buf[i * bytes + bytes - 1 - j] << (j * 8);
			}
		}
	}

	return arr;
}

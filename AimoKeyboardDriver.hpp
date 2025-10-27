#pragma once

#include <hidapi.h>

#include <cstdint>
#include <expected>
#include <map>
#include <string>

#include "ReadCallback.hpp"


#define ROCCAT_VULCAN_100_AIMO_PID 0x307A
#define ROCCAT_VULCAN_TKL_PRO_PID 0x311A

class AimoKeyboardDriver {
  public:
	struct RGBColor {
		uint8_t red;
		uint8_t green;
		uint8_t blue;
	};

	struct Config {
		uint8_t protocol_version;
	};

	enum PhysicalLayout {
		ANSI = 0,
		ISO = 1
	};

	// this list only contains layouts known layouts
	// Japan and Korea exist based on their website, but the id is unknown
	enum VisualLayout {
		US = 0, // United States (ANSI)
		DE = 1, // Germany (ISO)
		UK = 2, // United Kingdom (ISO)
		FR = 3, // France (ISO)
	};

	struct DeviceInfo {
		std::string version_string;
		uint8_t minor_version;
		uint8_t major_version;
		uint8_t physical_layout;
		uint8_t visual_layout;
		uint8_t _unknown3;
		uint8_t _unknown4;
		std::optional<uint8_t> _unknown5;
	};

	enum BusyState {
		READY = 1,
		ERROR = 2,
		BUSY = 3,
	};

	template <class T>
	using Error = std::expected<T, std::string>;

	using VoidError = std::optional<std::string>;

	AimoKeyboardDriver(){};
	AimoKeyboardDriver(std::string name, std::vector<hid_device *> hiddev, uint16_t pid);
	~AimoKeyboardDriver();

	Error<DeviceInfo> get_device_info ();
	Error<uint8_t> get_busy_state ();
	VoidError wait_until_ready ();

	Config config;
	hid_device *ctrl_device;
	hid_device *event_device;
	hid_device *led_device;
	std::string name;

  private:
	ReadCallback *cb;
	std::vector<uint8_t> await_response(uint8_t *command, uint8_t command_length);
	bool check_checksum(uint8_t* buf, int size, uint8_t checksum_size);
};

static std::map<uint16_t, AimoKeyboardDriver::Config> aimo_keyboard_config = {
	{ROCCAT_VULCAN_100_AIMO_PID, {1}},
	{ROCCAT_VULCAN_TKL_PRO_PID, {2}}};

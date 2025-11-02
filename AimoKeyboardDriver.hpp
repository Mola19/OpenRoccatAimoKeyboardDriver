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
		bool has_adjustable_time_to_sleep;
		bool has_lighting_toggle;
		// note: this is not the led amount, this also contains the gaps
		// it's primarily important for the size of the colours array
		uint8_t led_length;
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
	};

	enum BusyState {
		READY = 1,
		ERROR = 2,
		BUSY = 3,
	};

	enum Page {
		DEFAULT_LAYOUT = 0xA0,
		GAMEMODE_REMAP = 0xA1,
		CAPSLOCK_REMAP = 0xA2,
		FN_REMAP = 0xA3,
		EASYSHIFT = 0xA5,
		LIGHTING = 0xB0,
		// used by swarm but return gibberish
		// UNKNOWN = 0xC0,
	};

	struct ProfileInfo {
		uint8_t active_profile;
		uint8_t amount_profiles;
	};

	struct SoftwareStateGen1 {
		bool mute_light_on;
		bool software_control_enabled;
	};

	struct SoftwareStateGen2 {
		bool mute_light_on;
		bool sleep_enabled;
		uint8_t minutes_until_sleep;
	};

	struct SoftwareState {
		bool software_control_enabled;
		bool mute_light_on;
		std::optional<bool> sleep_enabled;
		std::optional<uint8_t> minutes_until_sleep;
	};

	struct LightingInfo {
		// when getting of gen 1 keyboards, the profile number is not returned,
		// you can only fetch data for the current profile
		std::optional<uint8_t> profile;
		uint8_t mode;
		uint8_t speed;
		uint8_t brightness;
		uint8_t theme;
		uint8_t is_custom_color;
		std::vector<RGBColor> colors;
	};

	template <class T> using Error = std::expected<T, std::string>;

	using VoidError = std::optional<std::string>;

	AimoKeyboardDriver(){};
	AimoKeyboardDriver(std::string name, std::vector<hid_device *> hiddev, uint16_t pid);
	~AimoKeyboardDriver();

	Error<DeviceInfo> get_device_info();
	Error<uint8_t> get_busy_state();
	VoidError wait_until_ready();
	VoidError set_page_to_read(uint8_t profile, uint8_t page_or_key, bool is_macro);

	Error<ProfileInfo> get_profile_info();
	VoidError set_profile_info(ProfileInfo info);
	VoidError set_profile_info(uint8_t active_profile, uint8_t amount_profiles);

	// this is a bit clunky, as the versions differ, for convenience use the wrapper function
	Error<SoftwareStateGen1> get_software_state_gen1();
	VoidError set_software_state_gen1(SoftwareStateGen1 state);
	VoidError set_software_state_gen1(bool mute_light_on, bool software_control_enabled);

	Error<bool> get_software_control_state_gen2();
	VoidError set_software_control_state_gen2(bool software_control_enabled);

	Error<SoftwareStateGen2> get_software_state_gen2();
	VoidError set_software_state_gen2(SoftwareStateGen2 state);
	VoidError
	set_software_state_gen2(bool mute_light_on, bool sleep_enabled, uint8_t minutes_until_sleep);

	// wrapper
	Error<SoftwareState> get_software_state();
	VoidError set_software_state(SoftwareState state);
	VoidError set_software_state(
		bool software_control_enabled, bool mute_light_on, std::optional<bool> sleep_enabled,
		std::optional<uint8_t> minutes_until_sleep
	);

	Error<bool> get_lighting_state();
	VoidError set_lighting_state(bool off);

	Error<LightingInfo> get_lighting();

	Config config;
	uint16_t pid;
	hid_device *ctrl_device;
	hid_device *event_device;
	hid_device *led_device;
	std::string name;

  private:
	ReadCallback *cb;
	std::vector<uint8_t> await_response(uint8_t *command, uint8_t command_length);
	bool check_checksum(uint8_t *buf, int size, uint8_t checksum_size);
};

static std::map<uint16_t, AimoKeyboardDriver::Config> aimo_keyboard_config = {
	{ROCCAT_VULCAN_100_AIMO_PID, {1, false, true, 144}},
	{ROCCAT_VULCAN_TKL_PRO_PID, {2, true, false, 96}}
};

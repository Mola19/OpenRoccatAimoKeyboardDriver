#pragma once

#include <hidapi.h>

#include <cstdint>
#include <expected>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "EventListener.hpp"
#include "KeyMaps.hpp"

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
		// same
		uint8_t remap_length;
		std::unordered_map<std::string, uint8_t> led_map;
		std::unordered_map<std::string, uint8_t> fn_map;
		std::optional<std::unordered_map<std::string, uint8_t>> fn_extra_map;
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
		CHECKSUM_WRONG = 6, // not 100% sure
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

	// only hardware modes will work without sending direct colors
	enum LightingModes {
		AIMO = 0x09,          // software mode
		WAVE = 0x0A,          // hardware mode
		SNAKE = 0x06,         // s
		FULLY_LIT = 0x01,     // h (essentially static)
		HEARTBEAT_2_0 = 0x08, // s
		BREATHING_2_0 = 0x07, // s
		FADE_FX = 0x11,       // s
		RIPPLE_FX = 0x12,     // s
		CUSTOM = 0x0B,        // s (use this for custom software modes)

		/*-------------------------------------------------------------------*\
		|  This mode is not a real mode, it's just the default mode when      |
		|  a mode is software generated, but Swarm is inactive, hence it has  |
		|  no id. Unfortunately 0 is refused by some keyboards, so 2 seems    |
		|  like a good choice as it is not used anywhere else                 |
		\*-------------------------------------------------------------------*/
		DEFAULT = 0x02,
	};

	// these do not actually change the color, they are just an indactor for the software
	// the names are are made up, based on what i think describes it based
	// but the actual color varies greatly between keyboards and from the in software preview
	enum Themes {
		RED_TO_YELLOW = 0x00,
		BLUE_TO_LAVANDER = 0x01,
		BLUE_TO_VIOLET = 0x02,
		VIOLET_TO_PINK = 0x03,
		YELLOW_TO_ORANGE = 0x04,
		YELLOW_TO_GREEN = 0x05,
		GREEN_TO_CYAN = 0x06,
		ROSE_TO_YELLOW = 0x07,
		BLUE_TO_GREEN = 0x08,
		PINK_TO_RED = 0x09
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

	struct GamemodeRemapInfo {
		// when getting of gen 1 keyboards, the profile number is not returned,
		// you can only fetch data for the current profile
		std::optional<uint8_t> profile;
		std::vector<uint8_t> values;
	};

	struct LongRemapInfo {
		// when getting of gen 1 keyboards, the profile number is not returned,
		// you can only fetch data for the current profile
		std::optional<uint8_t> profile;
		std::vector<uint32_t> values;
	};

	struct CapslockRemapInfo {
		// when getting of gen 1 keyboards, the profile number is not returned,
		// you can only fetch data for the current profile
		std::optional<uint8_t> profile;
		uint32_t capslock_value;
	};

	// max 1310827ms delay
	struct MacroStep {
		bool is_release;
		uint8_t keycode;
		uint32_t delay;
	};

	struct MacroInfo {
		uint8_t profile;
		uint8_t key_id;
		std::string foldername_utf8;
		std::string macroname_utf8;
		uint8_t repeat;
		std::vector<MacroStep> steps;
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
	VoidError reset_all_profiles();

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
	VoidError set_lighting(LightingInfo info);
	VoidError set_lighting(
		uint8_t profile, uint8_t mode, uint8_t speed, uint8_t brightness, uint8_t theme,
		uint8_t is_custom_color, std::vector<RGBColor> colors
	);
	VoidError set_direct_lighting(std::vector<RGBColor> colors);

	Error<GamemodeRemapInfo> get_gamemode_remap();
	VoidError set_gamemode_remap(GamemodeRemapInfo info);
	VoidError set_gamemode_remap(uint8_t profile, std::vector<uint8_t> values);

	Error<bool> get_easyshift();
	VoidError set_easyshift(bool active);

	Error<LongRemapInfo> get_easyshift_remap();
	VoidError set_easyshift_remap(LongRemapInfo info);
	VoidError set_easyshift_remap(uint8_t profile, std::vector<uint32_t> values);

	Error<LongRemapInfo> get_fn_remap();
	VoidError set_fn_remap(LongRemapInfo info);
	VoidError set_fn_remap(uint8_t profile, std::vector<uint32_t> values);

	Error<LongRemapInfo> get_fn_extra_remap();
	VoidError set_fn_extra_remap(LongRemapInfo info);
	VoidError set_fn_extra_remap(uint8_t profile, std::vector<uint32_t> values);

	Error<CapslockRemapInfo> get_capslock_remap();
	VoidError set_capslock_remap(CapslockRemapInfo info);
	VoidError set_capslock_remap(uint8_t profile, uint32_t capslock_value);

	Error<MacroInfo> get_macro();
	VoidError set_macro(MacroInfo info);
	VoidError set_macro(
		uint8_t profile, uint8_t key_id, std::string foldername_utf8, std::string macroname_utf8,
		uint8_t repeat, std::vector<MacroStep> steps
	);

	Config config;
	uint16_t pid;
	hid_device *ctrl_device;
	hid_device *event_device;
	hid_device *led_device;
	std::string name;
	EventListener *event;

  private:
	std::vector<uint8_t> generate_color_bytes(std::vector<RGBColor> colors);
	bool check_checksum(uint8_t *buf, int size, uint8_t checksum_size);
	void generate_checksum(uint8_t *buf, int size, uint8_t checksum_size);
	std::vector<uint8_t> uint_vec_to_le_array(std::vector<uint32_t> vec, uint8_t bytes, bool is_le);
	std::vector<uint32_t>
	le_array_to_uint_vec(uint8_t *buf, int out_size, uint8_t bytes, bool is_le);
};

inline std::unordered_map<uint16_t, AimoKeyboardDriver::Config> aimo_keyboard_config = {
	{ROCCAT_VULCAN_100_AIMO_PID,
	 {1, false, true, 144, 126, AimoKeyMaps::Vulcan100LED, AimoKeyMaps::FNRemapGen1FullGen1,
	  AimoKeyMaps::FNRemapExtra}},
	{ROCCAT_VULCAN_TKL_PRO_PID,
	 {2, true, false, 96, 126, AimoKeyMaps::VulcanTKLLED, AimoKeyMaps::FNRemapGenTKL, std::nullopt}}
};

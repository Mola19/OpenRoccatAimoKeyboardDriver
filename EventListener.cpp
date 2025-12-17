#include "EventListener.hpp"

#include <cstring>
#include <optional>

EventListener::EventListener(hid_device *dev, uint8_t gen, uint16_t pid) {
	hiddev = dev;
	this->gen = gen;
	this->pid = pid;

	read_thread = new std::thread(&EventListener::read_thread_fn, this);
}

EventListener::~EventListener() {
	kill_read_thread = true;
	read_thread->join();
}

void EventListener::register_profile_handler(std::function<void(uint8_t)> profile_handler) {
	this->profile_handler = profile_handler;
}

void EventListener::unregister_profile_handler() {
	this->profile_handler = std::nullopt;
}

void EventListener::register_state_handler(std::function<void(StateEvent)> state_handler) {
	this->state_handler = state_handler;
}

void EventListener::unregister_state_handler() {
	this->state_handler = std::nullopt;
}

void EventListener::register_keypress_handler(std::function<void(KeypressEvent)> keypress_handler) {
	this->keypress_handler = keypress_handler;
}

void EventListener::unregister_keypress_handler() {
	this->keypress_handler = std::nullopt;
}

void EventListener::register_osfn_handler(std::function<void(OSFnEvent)> osfn_handler) {
	this->osfn_handler = osfn_handler;
}

void EventListener::unregister_osfn_handler() {
	this->osfn_handler = std::nullopt;
}

void EventListener::register_multimedia_handler(std::function<void(MMEvent)> mm_handler) {
	this->mm_handler = mm_handler;
}

void EventListener::unregister_multimedia_handler() {
	this->mm_handler = std::nullopt;
}

void EventListener::register_lighting_handler(std::function<void(LightingEvent)> lighting_handler) {
	this->lighting_handler = lighting_handler;
}

void EventListener::unregister_lighting_handler() {
	this->lighting_handler = std::nullopt;
}

void EventListener::register_reset_handler(std::function<void(bool)> reset_handler) {
	this->reset_handler = reset_handler;
}

void EventListener::unregister_reset_handler() {
	this->reset_handler = std::nullopt;
}

void EventListener::register_global_easyshift_handler(std::function<void(bool)> ges_handler) {
	this->ges_handler = ges_handler;
}

void EventListener::unregister_global_easyshift_handler() {
	this->ges_handler = std::nullopt;
}

void EventListener::register_dpi_handler(std::function<void(bool)> dpi_handler) {
	this->dpi_handler = dpi_handler;
}

void EventListener::unregister_dpi_handler() {
	this->dpi_handler = std::nullopt;
}

void EventListener::register_wheel_handler(std::function<void(WheelEvent)> wheel_handler) {
	this->wheel_handler = wheel_handler;
}

void EventListener::unregister_wheel_handler() {
	this->wheel_handler = std::nullopt;
}

void EventListener::register_macro_handler(std::function<void(MacroEvent)> macro_handler) {
	this->macro_handler = macro_handler;
}

void EventListener::unregister_macro_handler() {
	this->macro_handler = std::nullopt;
}

std::unordered_map<uint8_t, uint8_t> Gen1OSFntoGen2 = {
	// open app, open swarm, start timer, stop timer are all extra apckets
	{0x01, 0x08}, {0x02, 0x09}, {0x03, 0x0A}, {0x04, 0x0B}, {0x05, 0x0C}, {0x06, 0x0D},
	{0x07, 0x0E}, {0x08, 0x0F}, {0x09, 0x10}, {0x0A, 0x11}, {0x0B, 0x12}, {0x1C, 0x23},
	{0x1D, 0x24}, {0x1E, 0x25}, {0x1F, 0x26}, {0x20, 0x27}, {0x21, 0x28}, {0x22, 0x29},
	{0x23, 0x2A}, {0x24, 0x2B}, {0x25, 0x2C}, {0x26, 0x2D}, {0x27, 0x2E}, {0x28, 0x2F},
	{0x29, 0x30}, {0x2A, 0x31}, {0x2B, 0x32}, {0x2C, 0x33}, {0x2D, 0x34}, {0x2E, 0x35},
	{0x2F, 0x36}, {0x30, 0x37}, {0x31, 0x05}, {0x32, 0x06}, {0x33, 0x07},
};

struct Gen1OSKeytoGen2Struct {
	uint8_t modifier;
	uint8_t key_id;
};

std::unordered_map<uint8_t, Gen1OSKeytoGen2Struct> VulcanTKLKeytoGen2 = {
	{0x00, { 0x01, 0x03 }},
	{0x01, { 0x01, 0x09 }},
	{0x02, { 0x01, 0x12 }},
	{0x03, { 0x01, 0x13 }},
	{0x04, { 0x01, 0x1A }},
	{0x05, { 0x01, 0x0A }},
	{0x06, { 0x01, 0x0B }},
	{0x07, { 0x01, 0x14 }},
	{0x08, { 0x01, 0x1B }},
	{0x09, { 0x01, 0x23 }},
	{0x0A, { 0x01, 0x0C }},
	{0x0B, { 0x01, 0x15 }},
	{0x0C, { 0x01, 0x1C }},
	{0x0D, { 0x01, 0x25 }},
	{0x0E, { 0x01, 0x24 }},
	{0x0F, { 0x01, 0x16 }},
	{0x10, { 0x01, 0x1D }},
	{0x11, { 0x01, 0x1C }},
	{0x12, { 0x01, 0x26 }},
	{0x13, { 0x01, 0x2E }},

	{0x1E, { 0x03, 0x00 }},
	{0x1F, { 0x03, 0x08 }},
	{0x20, { 0x03, 0x11 }},
	{0x21, { 0x03, 0x10 }},
	{0x22, { 0x03, 0x18 }},
	{0x23, { 0x03, 0x20 }},
	{0x24, { 0x03, 0x28 }},
	{0x25, { 0x03, 0x29 }},
	{0x26, { 0x03, 0x30 }},
	{0x27, { 0x03, 0x38 }},
	{0x28, { 0x03, 0x40 }},
	{0x29, { 0x03, 0x41 }},
	{0x2A, { 0x03, 0x49 }},
	{0x2B, { 0x03, 0x51 }},
	{0x2C, { 0x03, 0x59 }},
	{0x2D, { 0x03, 0x4A }},
	{0x2E, { 0x03, 0x52 }},
	{0x2F, { 0x03, 0x5A }},
	{0x30, { 0x03, 0x5C }},
	{0x31, { 0x03, 0x5D }},
	{0x32, { 0x03, 0x65 }},
	{0x33, { 0x03, 0x6D }},
	{0x34, { 0x03, 0x77 }},

	{0x35, { 0x02, 0x00 }},
	{0x36, { 0x02, 0x08 }},
	{0x37, { 0x02, 0x11 }},
	{0x38, { 0x02, 0x10 }},
	{0x39, { 0x02, 0x18 }},
	{0x3A, { 0x02, 0x20 }},
	{0x3B, { 0x02, 0x28 }},
	{0x3C, { 0x02, 0x29 }},
	{0x3D, { 0x02, 0x30 }},
	{0x3E, { 0x02, 0x38 }},
	{0x3F, { 0x02, 0x40 }},
	{0x40, { 0x02, 0x41 }},
	{0x41, { 0x02, 0x49 }},
	{0x42, { 0x02, 0x4A }},
	{0x43, { 0x02, 0x51 }},
	{0x44, { 0x02, 0x52 }},
	{0x45, { 0x02, 0x59 }},
	{0x46, { 0x02, 0x5A }},
	{0x47, { 0x02, 0x5C }},
	{0x48, { 0x02, 0x5D }},
	{0x49, { 0x02, 0x65 }},
	{0x4A, { 0x02, 0x6D }},
	{0x4B, { 0x02, 0x77 }},
	
};

std::unordered_map<uint8_t, Gen1OSKeytoGen2Struct> Vulcan1XXKeytoGen2 = {
	{0x00, { 0x01, 0x03 }},
	{0x01, { 0x01, 0x09 }},
	{0x02, { 0x01, 0x12 }},
	{0x03, { 0x01, 0x13 }},
	{0x04, { 0x01, 0x1A }},
	{0x05, { 0x01, 0x0A }},
	{0x06, { 0x01, 0x0B }},
	{0x07, { 0x01, 0x14 }},
	{0x08, { 0x01, 0x1B }},
	{0x09, { 0x01, 0x23 }},
	{0x0A, { 0x01, 0x0C }},
	{0x0B, { 0x01, 0x15 }},
	{0x0C, { 0x01, 0x1C }},
	{0x0D, { 0x01, 0x25 }},
	{0x0E, { 0x01, 0x24 }},
	{0x0F, { 0x01, 0x16 }},
	{0x10, { 0x01, 0x1D }},
	{0x11, { 0x01, 0x1C }},
	{0x12, { 0x01, 0x26 }},
	{0x13, { 0x01, 0x2E }},
	
	// twice ???
	{0x13, { 0x03, 0x49 }},
	{0x14, { 0x03, 0x51 }},
	{0x15, { 0x03, 0x59 }},
	{0x16, { 0x03, 0x4A }},
	{0x17, { 0x03, 0x52 }},
	{0x18, { 0x03, 0x5A }},

	{0x1E, { 0x03, 0x00 }},
	{0x1F, { 0x03, 0x08 }},
	{0x20, { 0x03, 0x11 }},
	{0x21, { 0x03, 0x10 }},
	{0x22, { 0x03, 0x18 }},
	{0x23, { 0x03, 0x20 }},
	{0x24, { 0x03, 0x28 }},
	{0x25, { 0x03, 0x29 }},
	{0x26, { 0x03, 0x30 }},
	{0x27, { 0x03, 0x38 }},
	{0x28, { 0x03, 0x40 }},
	{0x29, { 0x03, 0x41 }},
	{0x2A, { 0x03, 0x49 }},
	{0x2B, { 0x03, 0x51 }},
	{0x2C, { 0x03, 0x59 }},

	{0x2D, { 0x02, 0x00 }},
	{0x2E, { 0x02, 0x08 }},
	{0x2F, { 0x02, 0x11 }},
	{0x30, { 0x02, 0x10 }},
	{0x31, { 0x02, 0x18 }},
	{0x32, { 0x02, 0x20 }},
	{0x33, { 0x02, 0x28 }},
	{0x34, { 0x02, 0x29 }},
	{0x35, { 0x02, 0x30 }},
	{0x36, { 0x02, 0x38 }},
	{0x37, { 0x02, 0x40 }},
	{0x38, { 0x02, 0x41 }},
	{0x39, { 0x02, 0x49 }},
	{0x3A, { 0x02, 0x51 }},
	{0x3B, { 0x02, 0x59 }},
};

void EventListener::read_thread_fn() {
	while (!kill_read_thread) {
		if (gen == 2) {
			uint8_t res[8];
			memset(res, 0x00, 8);
			int ret = hid_read_timeout(hiddev, res, 8, 100);

			if (ret < 1)
				continue;

			switch (res[2]) {
				case 0x01:
					if (profile_handler)
						profile_handler.value()(res[3]);
					break;
				case 0x08:
					if (reset_handler)
						reset_handler.value()(res[3]);
					break;
				case 0x0B:
					if (mm_handler)
						mm_handler.value()(
							{.function = res[3], .released = static_cast<bool>(res[4])}
						);
					break;
				case 0x0C:
					if (lighting_handler) {
						std::optional<bool> change;

						switch (res[4]) {
							case 0x01:
								change = true;
								break;
							case 0xFF:
								change = false;
								break;
							default:
								change = std::nullopt;
						}

						lighting_handler.value()(
							{.type = 1, .value = res[3], .positive_change = change}
						);
					}
					break;
				case 0x21:
					if (state_handler)
						state_handler.value()({.state = 2, .active = static_cast<bool>(res[3])});
					break;
				case 0x22:
					if (state_handler)
						state_handler.value()({.state = 3, .active = static_cast<bool>(res[3])});
					break;
				// at least for the vulcan tkl pro this always returns not active
				// i have no good solutions so it just stays there
				case 0x23:
					if (state_handler)
						state_handler.value()({.state = 4, .active = static_cast<bool>(res[3])});
					break;
				case 0xCA:
					if (dpi_handler)
						dpi_handler.value()(res[3] == 0x01);
					break;
				case 0xCC:
					if (wheel_handler)
						wheel_handler.value()(
							{.clockwise = static_cast<bool>(res[3] == 1), .volume_mode = true}
						);
					break;
				case 0xCE:
					if (lighting_handler)
						lighting_handler.value()(
							{.type = 0, .value = res[3], .positive_change = std::nullopt}
						);
					break;
				case 0xCF:
					if (state_handler)
						state_handler.value()(
							{.state = 1, .active = static_cast<bool>(res[3] == 2)}
						);
					break;
				case 0xF1:
					if (osfn_handler)
						osfn_handler.value()(
							{.function = res[3],
							 .modifier = res[5],
							 .key = static_cast<uint8_t>(res[4] - 0x10),
							 .released = static_cast<bool>(res[6])}
						);
					break;
				case 0xFB:
					if (keypress_handler)
						keypress_handler.value()(
							{.key = static_cast<uint8_t>(res[3] - 0x10),
							 .released = static_cast<bool>(!res[4])}
						);
					break;
				case 0xFD:
					if (state_handler)
						state_handler.value()({.state = 0, .active = static_cast<bool>(res[3])});
					break;
				case 0xFF:
					if (ges_handler)
						// technically this also reports the key, but this can only come from
						// capslock
						ges_handler.value()(res[4]);
			}
		} else {
			uint8_t res[5];
			memset(res, 0x00, 5);
			int ret = hid_read_timeout(hiddev, res, 5, 100);

			if (ret < 1)
				continue;

			switch (res[2]) {
				case 0x01:
					if (profile_handler)
						profile_handler.value()(res[3]);
					break;
				// profile switch too, no idea what the difference is
				// case 0x01:
				// 	if (profile_handler)
				// 		profile_handler.value()(res[3]);
				// 	break;
				case 0x03:
					if (macro_handler) {
						if (VulcanTKLKeytoGen2.count(res[3] - 1) == 0) continue;
						auto val = VulcanTKLKeytoGen2[res[3] - 1];

						macro_handler.value()(
							{.key_id = val.key_id, .released = static_cast<bool>(res[4])}
						);
					}
					break;
				case 0x11:
					if (osfn_handler) {
						if (pid == 0x2FEE) {
							if (VulcanTKLKeytoGen2.count(res[3]) == 0) continue;
							auto val = VulcanTKLKeytoGen2[res[3]];
							osfn_handler.value()(
								{.function = Gen1OSFntoGen2[res[1]],
								 .modifier = val.modifier,
								 .key = val.key_id,
								 .released = static_cast<bool>(res[4])});
						} else {
							if (Vulcan1XXKeytoGen2.count(res[3]) == 0) continue;
							auto val = Vulcan1XXKeytoGen2[res[3]];
							osfn_handler.value()(
								{.function = Gen1OSFntoGen2[res[1]],
								 .modifier = val.modifier,
								 .key = val.key_id,
								 .released = static_cast<bool>(res[4])});
						}
					}
					break;
				case 0xEF:
					if (state_handler)
						if (res[3] != 1) continue;
						state_handler.value()(
							{.state = 1, .active = static_cast<bool>(res[3] == 2)}
						);
					break;
				case 0xFD:
					if (state_handler)
						state_handler.value()({.state = 0, .active = static_cast<bool>(res[3])});
					break;
			}
		}
	}
}

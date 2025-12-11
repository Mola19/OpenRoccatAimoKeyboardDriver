#include "EventListener.hpp"

#include <cstring>
#include <optional>

EventListener::EventListener(hid_device *dev, uint8_t gen) {
	hiddev = dev;
	this->gen = gen;

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
				case 0xCA:
					if (dpi_handler)
						dpi_handler.value()(res[3] == 0x01);
					break;
				case 0xCC:
					if (wheel_handler)
						wheel_handler.value()({.clockwise = static_cast<bool>(res[3] == 1), .volume_mode = true});
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
						// technically this also reports the key, but this can only come from capslock
						ges_handler.value()(res[4]);
			}
		} else {
			uint8_t res[5];
			memset(res, 0x00, 5);
			int ret = hid_read_timeout(hiddev, res, 5, 100);

			if (ret < 1)
				continue;

			switch (res[2]) {
				case 0xFD:
					if (state_handler)
						state_handler.value()({.state = 0, .active = static_cast<bool>(res[3])});
					break;
			}
		}
	}
}

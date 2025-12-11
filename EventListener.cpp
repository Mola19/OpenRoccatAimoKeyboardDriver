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
				case 0x21:
					if (state_handler)
						state_handler.value()({.state = 2, .active = static_cast<bool>(res[3])});
					break;
				case 0x22:
					if (state_handler)
						state_handler.value()({.state = 3, .active = static_cast<bool>(res[3])});
					break;
				case 0xCF:
					if (state_handler)
						state_handler.value()(
							{.state = 1, .active = static_cast<bool>(res[3] == 0)}
						);
					break;
				case 0xFB:
					if (keypress_handler)
						keypress_handler.value()({.key = static_cast<uint8_t>(res[3] - 0x10), .released = static_cast<bool>(!res[4])});
					break;
				case 0xFD:
					if (state_handler)
						state_handler.value()({.state = 0, .active = static_cast<bool>(res[3])});
					break;
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

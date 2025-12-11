#pragma once

#include <functional>
#include <hidapi.h>

#include <optional>
#include <thread>

class EventListener {
  public:
	EventListener(hid_device *dev, uint8_t gen);
	~EventListener();

	struct StateEvent {
		uint8_t state;
		bool active;
	};

	struct KeypressEvent {
		uint8_t key;
		bool released;
	};

	// i have observed unprompted release events, when changing profile
	// no idea if these are common and also appearing somewhere else
	// but it might be a good idea to use press not release events
	struct OSFnEvent {
		uint8_t function;
		uint8_t modifier;
		uint8_t key;
		bool released;
	};

	struct MMEvent {
		uint8_t function;
		bool released;
	};

	struct LightingEvent {
		uint8_t type;
		uint8_t value;
		// only with gen 2 brightness
		// denotes whether the value was previously less or more
		std::optional<bool> positive_change;
	};

	void register_profile_handler(std::function<void(uint8_t)> profile_handler);
	void unregister_profile_handler();

	void register_state_handler(std::function<void(StateEvent)> state_handler);
	void unregister_state_handler();

	void register_keypress_handler(std::function<void(KeypressEvent)> keypress_handler);
	void unregister_keypress_handler();
	
	void register_osfn_handler(std::function<void(OSFnEvent)> osfn_handler);
	void unregister_osfn_handler();

	void register_multimedia_handler(std::function<void(MMEvent)> mm_handler);
	void unregister_multimedia_handler();

	void register_lighting_handler(std::function<void(LightingEvent)> lighting_handler);
	void unregister_lighting_handler();

  private:
	hid_device *hiddev;
	uint8_t gen;
	std::thread *read_thread;

	std::optional<std::function<void(uint8_t)>> profile_handler;
	std::optional<std::function<void(StateEvent)>> state_handler;
	std::optional<std::function<void(KeypressEvent)>> keypress_handler;
	std::optional<std::function<void(OSFnEvent)>> osfn_handler;
	std::optional<std::function<void(MMEvent)>> mm_handler;
	std::optional<std::function<void(LightingEvent)>> lighting_handler;

	bool kill_read_thread = false;
	void read_thread_fn();
};
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

	void register_state_handler(std::function<void(StateEvent)> state_handler);
	void unregister_state_handler();

  private:
	hid_device *hiddev;
	uint8_t gen;
	std::thread *read_thread;

	std::optional<std::function<void(StateEvent)>> state_handler;

	bool kill_read_thread = false;
	void read_thread_fn();
};
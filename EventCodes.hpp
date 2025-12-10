#pragma once

#include <string>
#include <unordered_map>

// clang-format off

namespace AimoEventCodes {
	inline std::unordered_map<std::string, uint8_t> States = {
		{ "EasyShift", 0x00 },
		{ "Sleep", 0x01 },
		{ "Gamemode", 0x02 },
		{ "Fn Lock", 0x03 }
	};
}

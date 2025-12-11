#pragma once

#include <string>
#include <unordered_map>

// clang-format off

namespace AimoEventCodes {
	inline std::unordered_map<std::string, uint8_t> States = {
		{ "EasyShift", 0x00 },
		{ "Sleep", 0x01 },
		{ "Gamemode", 0x02 },
		{ "Fn Lock", 0x03 },
		{ "Fn Win Switch", 0x04 }
	};

	inline std::unordered_map<std::string, uint8_t> Modifieres = {
		{ "EasyShift", 0x01 },
		{ "Fn", 0x02 },
		{ "Gamemode", 0x03 }
	};

	inline std::unordered_map<std::string, uint8_t> OSFunctions = {
		{ "Open Application", 0x01 },
		{ "Start Swarm", 0x02 },
		{ "Start Timer", 0x03 },
		{ "Stop Timer", 0x04 },
		{ "Open Document", 0x05 },
		{ "Open Folder", 0x06 },
		{ "Open Website", 0x07 },
		{ "Mute Microphone", 0x08 },
		{ "Open Desktop", 0x09 },
		{ "Open Favorites", 0x0A },
		{ "Open Fonts", 0x0B },
		{ "Open Documents", 0x0C },
		{ "Open Downloads", 0x0D },
		{ "Open Music", 0x0E },
		{ "Open Pictures", 0x0F },
		{ "Open Network", 0x10 },
		{ "Open Printers", 0x11 },
		{ "Open System", 0x12 },
		{ "System hybernate", 0x23 },
		{ "System restart", 0x24 },
		{ "System lock", 0x25 },
		{ "System log out", 0x26 },
		{ "Open Control Panel", 0x27 },
		{ "Open System Properties", 0x28 },
		{ "Open Task Manager", 0x29 },
		{ "Open Display Settings", 0x2A },
		{ "Open Screen Saver Settings", 0x2B },
		{ "Open Background Settings", 0x2C },
		{ "Open Date & Time Settings", 0x2D },
		{ "Open Network Settings", 0x2E },
		{ "Open Administrative Tools", 0x2F },
		{ "Open Windows Firewall", 0x30 },
		{ "Open Regedit", 0x31 },
		{ "Open Event Viewer", 0x32 },
		{ "Open Resource Monitor", 0x33 },
		{ "Open Sound Settings", 0x34 },
		{ "Open Internet Settings", 0x35 },
		{ "Open DirectX Diagnostics", 0x36 },
		{ "Open Command Line", 0x37 },
	};

	inline std::unordered_map<std::string, uint8_t> MMCodes = {
		// these ones i have written down, but i can not reproduce them
		{ "Previous Track", 0x21 },
		{ "Next Track", 0x22 },
		{ "Play/Pause", 0x23 },
		{ "Stop", 0x24 },
		// until here
		{ "Volume Up", 0x26 },
		{ "Volume Down", 0x27 }
	};

	inline std::unordered_map<std::string, uint8_t> LightingCodes = {
		{ "Mode", 0x00 },
		{ "Brightness", 0x01 }
	};
}

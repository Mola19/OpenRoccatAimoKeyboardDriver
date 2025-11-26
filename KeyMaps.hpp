#pragma once

#include <string>
#include <unordered_map>

// clang-format off

namespace AimoKeyMaps {
	inline std::unordered_map<std::string, uint16_t> Vulcan100LED = {
		{ "ESCAPE",         0x00 },
		{ "BACK_TICK",      0x01 },
		{ "TAB",            0x02 },
		{ "CAPS_LOCK",      0x03 },
		{ "LEFT_SHIFT",     0x04 },
		{ "LEFT_CONTROL",   0x05 },

		{ "1",              0x06 },
		{ "ISO_BACKSLASH",  0x09 },
		{ "LEFT_WINDOWS",   0x0A },

		{ "F1",             0x0B },
		{ "2",              0x0C },
		{ "Q",              0x07 },
		{ "A",              0x08 },
		{ "Z",              0x0F },
		{ "LEFT_ALT",       0x10 },

		{ "F2",             0x11 },
		{ "3",              0x12 },
		{ "W",              0x0D },
		{ "S",              0x0E },
		{ "X",              0x15 },

		{ "F3",             0x17 },
		{ "4",              0x18 },
		{ "E",              0x13 },
		{ "D",              0x14 },
		{ "C",              0x1B },

		{ "F4",             0x1C },
		{ "5",              0x1D },
		{ "R",              0x19 },
		{ "F",              0x1A },
		{ "V",              0x20 },

		{ "6",              0x21 },
		{ "T",              0x1E },
		{ "G",              0x1F },
		{ "B",              0x24 },
		{ "SPACE",          0x25 },

		{ "F5",             0x30 },
		{ "7",              0x31 },
		{ "Y",              0x22 },
		{ "H",              0x23 },
		{ "N",              0x34 },

		{ "F6",             0x35 },
		{ "8",              0x36 },
		{ "U",              0x32 },
		{ "J",              0x33 },
		{ "M",              0x39 },

		{ "F7",             0x3B },
		{ "9",              0x3C },
		{ "I",              0x37 },
		{ "K",              0x38 },
		{ "COMMA",          0x3F },

		{ "F8",             0x41 },
		{ "0",              0x42 },
		{ "O",              0x3D },
		{ "L",              0x3E },
		{ "PERIOD",         0x45 },
		{ "RIGHT_ALT",      0x46 },

		{ "F9",             0x4E },
		{ "MINUS",          0x48 },
		{ "P",              0x43 },
		{ "SEMICOLON",      0x44 },
		{ "FORWARD_SLASH",  0x4B },
		{ "RIGHT_FUNCTION", 0x4C },

		{ "F10",            0x54 },
		{ "EQUALS",         0x4F },
		{ "LEFT_BRACKET",   0x49 },
		{ "QUOTE",          0x4A },
		{ "MENU",           0x53 },

		{ "F11",            0x55 },
		{ "BACKSPACE",      0x57 },
		{ "RIGHT_BRACKET",  0x50 },
		{ "ISO_POUND",      0x60 },
		{ "RIGHT_SHIFT",    0x52 },

		{ "F12",            0x56 },
		{ "ANSI_BACKSLASH", 0x51 },
		{ "ENTER",          0x58 },
		{ "RIGHT_CONTROL",  0x59 },

		{ "PRINT_SCREEN",   0x63 },
		{ "INSERT",         0x64 },
		{ "DELETE",         0x65 },
		{ "LEFT_ARROW",     0x66 },

		{ "SCROLL_LOCK",    0x67 },
		{ "HOME",           0x68 },
		{ "END",            0x69 },
		{ "UP_ARROW",       0x6A },
		{ "DOWN_ARROW",     0x6B },

		{ "PAUSE_BREAK",    0x6C },
		{ "PAGE_UP",        0x6D },
		{ "PAGE_DOWN",      0x6E },
		{ "RIGHT_ARROW",    0x6F },

		{ "NUMPAD_LOCK",    0x71 },
		{ "NUMPAD_7",       0x72 },
		{ "NUMPAD_4",       0x73 },
		{ "NUMPAD_1",       0x74 },
		{ "NUMPAD_0",       0x75 },

		{ "NUMPAD_DIVIDE",  0x77 },
		{ "NUMPAD_8",       0x78 },
		{ "NUMPAD_5",       0x79 },
		{ "NUMPAD_2",       0x7A },

		{ "NUMPAD_TIMES",   0x7C },
		{ "NUMPAD_9",       0x7D },
		{ "NUMPAD_6",       0x7E },
		{ "NUMPAD_3",       0x7F },
		{ "NUMPAD_PERIOD",  0x80 },

		{ "NUMPAD_MINUS",   0x81 },
		{ "NUMPAD_PLUS",    0x82 },
		{ "NUMPAD_ENTER",   0x83 }
	};

	inline std::unordered_map<std::string, uint16_t> VulcanTKLLED = {
		{ "ESCAPE",         0x02 },
		{ "BACK_TICK",      0x03 },
		{ "TAB",            0x04 },
		{ "CAPS_LOCK",      0x05 },
		{ "LEFT_SHIFT",     0x00 },
		{ "LEFT_CONTROL",   0x01 },

		{ "1",              0x08 },
		{ "ISO_BACKSLASH",  0x06 },
		{ "LEFT_WINDOWS",   0x07 },

		{ "F1",             0x0D },
		{ "2",              0x0E },
		{ "Q",              0x09 },
		{ "A",              0x0A },
		{ "Z",              0x0B },
		{ "LEFT_ALT",       0x0C },

		{ "F2",             0x14 },
		{ "3",              0x15 },
		{ "W",              0x0F },
		{ "S",              0x10 },
		{ "X",              0x11 },

		{ "F3",             0x19 },
		{ "4",              0x1A },
		{ "E",              0x16 },
		{ "D",              0x17 },
		{ "C",              0x18 },

		{ "F4",             0x1E },
		{ "5",              0x1F },
		{ "R",              0x1B },
		{ "F",              0x1C },
		{ "V",              0x1D },

		{ "6",              0x24 },
		{ "T",              0x20 },
		{ "G",              0x21 },
		{ "B",              0x22 },
		{ "SPACE",          0x23 },

		{ "F5",             0x28 },
		{ "7",              0x29 },
		{ "Y",              0x25 },
		{ "H",              0x26 },
		{ "N",              0x27 },

		{ "F6",             0x2F },
		{ "8",              0x30 },
		{ "U",              0x2A },
		{ "J",              0x2B },
		{ "M",              0x2C },

		{ "F7",             0x35 },
		{ "9",              0x36 },
		{ "I",              0x31 },
		{ "K",              0x32 },
		{ "COMMA",          0x33 },

		{ "F8",             0x3B },
		{ "0",              0x3C },
		{ "O",              0x37 },
		{ "L",              0x38 },
		{ "PERIOD",         0x39 },
		{ "RIGHT_ALT",      0x3A },

		{ "F9",             0x41 },
		{ "MINUS",          0x42 },
		{ "P",              0x3D },
		{ "SEMICOLON",      0x3E },
		{ "FORWARD_SLASH",  0x3F },
		{ "RIGHT_FUNCTION", 0x40 },

		{ "F10",            0x47 },
		{ "EQUALS",         0x48 },
		{ "LEFT_BRACKET",   0x43 },
		{ "QUOTE",          0x44 },
		{ "MENU",           0x46 },

		{ "F11",            0x4D },
		{ "BACKSPACE",      0x50 },
		{ "RIGHT_BRACKET",  0x49 },
		{ "ISO_POUND",      0x4A },
		{ "RIGHT_SHIFT",    0x4B },

		{ "F12",            0x4F },
		{ "ANSI_BACKSLASH", 0x51 },
		{ "ENTER",          0x52 },
		{ "RIGHT_CONTROL",  0x4C },

		{ "MEDIA_MUTE",     0x5C },
		{ "INSERT",         0x54 },
		{ "DELETE",         0x55 },
		{ "LEFT_ARROW",     0x56 },

		{ "HOME",           0x58 },
		{ "END",            0x59 },
		{ "UP_ARROW",       0x5A },
		{ "DOWN_ARROW",     0x5B },

		{ "PAGE_UP",        0x5D },
		{ "PAGE_DOWN",      0x5E },
		{ "RIGHT_ARROW",    0x5F },
	};

	inline std::unordered_map<std::string, uint16_t> Vulcan100Remap = {
		{ "F1",             0x00 },
		{ "ESCAPE",         0x01 },
		{ "BACK_TICK",      0x02 },
		{ "1",              0x03 },
		{ "TAB",            0x04 },
		{ "CAPS_LOCK",      0x05 },
		{ "LEFT_SHIFT",     0x06 },
		{ "LEFT_CONTROL",   0x07 },

		{ "F2",             0x08 },
		{ "2",              0x09 },
		{ "Q",              0x0A },
		{ "W",              0x0B },
		{ "A",              0x0C },
		{ "ISO_BACKSLASH",  0x0D },
		{ "LEFT_WINDOWS",   0x0F },

		{ "F4",             0x10 },
		{ "F3",             0x11 },
		{ "3",              0x12 },
		{ "4",              0x13 },
		{ "E",              0x14 },
		{ "S",              0x15 },
		{ "Z",              0x16 },
		{ "LEFT_ALT",       0x17 },

		{ "F5",             0x18 },
		{ "6",              0x19 },
		{ "5",              0x1A },
		{ "R",              0x1B },
		{ "D",              0x1C },
		{ "X",              0x1D },
		{ "C",              0x1E },

		{ "F6",             0x20 },
		{ "7",              0x21 },
		{ "T",              0x23 },
		{ "G",              0x24 },
		{ "F",              0x25 },
		{ "V",              0x26 },

		{ "F7",             0x28 },
		{ "F8",             0x29 },
		{ "Y",              0x2B },
		{ "U",              0x2C },
		{ "H",              0x2D },
		{ "B",              0x2E },
		{ "SPACE",          0x2F },
		
		{ "F9",             0x30 },
		{ "9",              0x31 },
		{ "8",              0x32 },
		{ "I",              0x33 },
		{ "J",              0x34 },
		{ "K",              0x35 },
		{ "M",              0x36 },
		{ "N",              0x37 },

		{ "F10",            0x38 },
		{ "BACKSPACE",      0x39 },
		{ "0",              0x3A },
		{ "MINUS",          0x3B },
		{ "O",              0x3C },
		{ "L",              0x3D },
		{ "COMMA",          0x3E },

		{ "F11",            0x40 },
		{ "F12",            0x41 },
		{ "EQUALS",         0x43 },
		{ "P",              0x44 },
		{ "SEMICOLON",      0x45 },
		{ "PERIOD",         0x46 },
		
		{ "PRINT_SCREEN",   0x48 },
		{ "INSERT",         0x49 },
		{ "DELETE",         0x4A },

		{ "LEFT_BRACKET",   0x4B },
		{ "RIGHT_BRACKET",  0x4C },
		{ "QUOTE",          0x4D },
		{ "FORWARD_SLASH",  0x4E },

		{ "SCROLL_LOCK",    0x50 },
		{ "HOME",           0x51 },
		{ "END",            0x52 },

		{ "ISO_POUND",      0x54 },

		{ "RIGHT_ALT",      0x57 },

		{ "PAUSE_BREAK",    0x58 },
		{ "PAGE_UP",        0x59 },
		{ "PAGE_DOWN",      0x5A },

		{ "ENTER",          0x5B },

		{ "UP_ARROW",       0x5C },
		{ "LEFT_ARROW",     0x5D },
		
		{ "RIGHT_SHIFT",    0x5E },

		{ "NUMPAD_LOCK",    0x61 },
		{ "NUMPAD_7",       0x62 },
		{ "NUMPAD_4",       0x63 },
		{ "NUMPAD_1",       0x64 },

		{ "DOWN_ARROW",     0x65 },

		{ "NUMPAD_DIVIDE",  0x69 },
		{ "NUMPAD_8",       0x6A },
		{ "NUMPAD_5",       0x6B },
		{ "NUMPAD_2",       0x6C },

		{ "RIGHT_ARROW",    0x6D },

		{ "MENU",           0x6F },

		{ "NUMPAD_TIMES",   0x71 },
		{ "NUMPAD_9",       0x72 },
		{ "NUMPAD_6",       0x73 },
		{ "NUMPAD_3",       0x74 },
		{ "NUMPAD_0",       0x75 },

		{ "RIGHT_CONTROL",  0x77 },

		{ "NUMPAD_PLUS",    0x79 },
		{ "NUMPAD_MINUS",   0x7A },

		{ "NUMPAD_PERIOD",  0x7C },
		{ "NUMPAD_ENTER",   0x7D }

		// { "ANSI_BACKSLASH", ??? },
	};

	inline std::unordered_map<std::string, uint16_t> VulcanTKLRemap = {
		{ "F1",             0x00 },
		{ "ESCAPE",         0x01 },
		{ "BACK_TICK",      0x02 },
		{ "1",              0x03 },
		{ "TAB",            0x04 },
		{ "CAPS_LOCK",      0x05 },
		{ "LEFT_SHIFT",     0x06 },
		{ "LEFT_CONTROL",   0x07 },

		{ "F2",             0x08 },
		{ "2",              0x09 },
		{ "Q",              0x0A },
		{ "W",              0x0B },
		{ "A",              0x0C },
		{ "ISO_BACKSLASH",  0x0D },
		{ "LEFT_WINDOWS",   0x0F },

		{ "F4",             0x10 },
		{ "F3",             0x11 },
		{ "3",              0x12 },
		{ "4",              0x13 },
		{ "E",              0x14 },
		{ "S",              0x15 },
		{ "Z",              0x16 },
		{ "LEFT_ALT",       0x17 },

		{ "F5",             0x18 },
		{ "6",              0x19 },
		{ "5",              0x1A },
		{ "R",              0x1B },
		{ "D",              0x1C },
		{ "X",              0x1D },
		{ "C",              0x1E },

		{ "F6",             0x20 },
		{ "7",              0x21 },
		{ "T",              0x23 },
		{ "G",              0x24 },
		{ "F",              0x25 },
		{ "V",              0x26 },

		{ "F7",             0x28 },
		{ "F8",             0x29 },
		{ "Y",              0x2B },
		{ "U",              0x2C },
		{ "H",              0x2D },
		{ "B",              0x2E },
		{ "SPACE",          0x2F },
		
		{ "F9",             0x30 },
		{ "9",              0x31 },
		{ "8",              0x32 },
		{ "I",              0x33 },
		{ "J",              0x34 },
		{ "K",              0x35 },
		{ "M",              0x36 },
		{ "N",              0x37 },

		{ "F10",            0x38 },
		{ "BACKSPACE",      0x39 },
		{ "0",              0x3A },
		{ "MINUS",          0x3B },
		{ "O",              0x3C },
		{ "L",              0x3D },
		{ "COMMA",          0x3E },

		{ "F11",            0x40 },
		{ "F12",            0x41 },
		{ "EQUALS",         0x43 },
		{ "P",              0x44 },
		{ "SEMICOLON",      0x45 },
		{ "PERIOD",         0x46 },
		
		{ "INSERT",         0x49 },
		{ "DELETE",         0x4A },

		{ "LEFT_BRACKET",   0x4B },
		{ "RIGHT_BRACKET",  0x4C },
		{ "QUOTE",          0x4D },
		{ "FORWARD_SLASH",  0x4E },

		{ "HOME",           0x51 },
		{ "END",            0x52 },

		{ "ISO_POUND",      0x54 },

		{ "RIGHT_ALT",      0x57 },

		{ "PAGE_UP",        0x59 },
		{ "PAGE_DOWN",      0x5A },

		{ "ENTER",          0x5B },

		{ "UP_ARROW",       0x5C },
		{ "LEFT_ARROW",     0x5D },
		
		{ "RIGHT_SHIFT",    0x5E },

		{ "DOWN_ARROW",     0x65 },
		{ "RIGHT_ARROW",    0x6D },

		{ "MENU",           0x6F },

		{ "RIGHT_CONTROL",  0x77 },

		// { "ANSI_BACKSLASH", ??? },
	};

	inline std::unordered_map<std::string, uint16_t> EasyShift = {
		{ "1",              0x00 },
		{ "2",              0x01 },
		{ "3",              0x02 },
		{ "4",              0x03 },
		{ "5",              0x04 },

		{ "Q",              0x05 },
		{ "W",              0x06 },
		{ "E",              0x07 },
		{ "R",              0x08 },
		{ "T",              0x09 },

		{ "A",              0x0A },
		{ "S",              0x0B },
		{ "D",              0x0C },
		{ "F",              0x0D },
		{ "G",              0x0E },
		
		{ "Z",              0x0F },
		{ "X",              0x10 },
		{ "C",              0x11 },
		{ "V",              0x12 },
		{ "B",              0x13 },
	};

	inline std::unordered_map<std::string, uint16_t> FNRemap = {
		{ "F1 (Gamemode)",              0x00 },
		{ "F2 (Gamemode)",              0x01 },
		{ "F3 (Gamemode)",              0x02 },
		{ "F4 (Gamemode)",              0x03 },
		{ "F5 (Gamemode)",              0x04 },
		{ "F6 (Gamemode)",              0x05 },
		{ "F7 (Gamemode)",              0x06 },
		{ "F8 (Gamemode)",              0x07 },
		{ "F9 (Gamemode)",              0x08 },
		{ "F10 (Gamemode)",             0x09 },
		{ "F11 (Gamemode)",             0x0A },
		{ "F12 (Gamemode)",             0x0B },

		{ "INSERT (Gamemode)",          0x0C },
		{ "DELETE (Gamemode)",          0x0D },
		{ "HOME (Gamemode)",            0x0E },
		{ "END (Gamemode)",             0x0F },
		{ "PAGE_UP (Gamemode)",         0x10 },
		{ "PAGE_DOWN (Gamemode)",       0x11 },

		{ "UP_ARROW (Gamemode)",        0x12 },
		{ "LEFT_ARROW (Gamemode)",      0x13 },
		{ "DOWN_ARROW (Gamemode)",      0x14 },
		{ "RIGHT_ARROW (Gamemode)",     0x15 },
		{ "RIGHT_CONTROL (Gamemode)",   0x16 },

		{ "F1 (FN)",                    0x17 },
		{ "F2 (FN)",                    0x18 },
		{ "F3 (FN)",                    0x19 },
		{ "F4 (FN)",                    0x1A },
		{ "F5 (FN)",                    0x1B },
		{ "F6 (FN)",                    0x1C },
		{ "F7 (FN)",                    0x1D },
		{ "F8 (FN)",                    0x1E },
		{ "F9 (FN)",                    0x1F },
		{ "F10 (FN)",                   0x20 },
		{ "F11 (FN)",                   0x21 },
		{ "F12 (FN)",                   0x22 },

		{ "INSERT (FN)",                0x23 },
		{ "DELETE (FN)",                0x24 },
		{ "HOME (FN)",                  0x25 },
		{ "END (FN)",                   0x26 },
		{ "PAGE_UP (FN)",               0x27 },
		{ "PAGE_DOWN (FN)",             0x28 },

		{ "UP_ARROW (FN)",              0x29 },
		{ "LEFT_ARROW (FN)",            0x2A },
		{ "DOWN_ARROW (FN)",            0x2B },
		{ "RIGHT_ARROW (FN)",           0x2C },
		{ "RIGHT_CONTROL (FN)",         0x2D },
	};
}

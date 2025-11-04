#pragma once

#include <string>
#include <unordered_map>

// clang-format off

namespace AimoKeyMaps {
	inline std::unordered_map<uint16_t, std::string> Vulcan100 = {
		{ 0x00, "ESCAPE"         },
		{ 0x01, "BACK_TICK"      },
		{ 0x02, "TAB"            },
		{ 0x03, "CAPS_LOCK"      },
		{ 0x04, "LEFT_SHIFT"     },
		{ 0x05, "LEFT_CONTROL"   },

		{ 0x06, "1"              },
		{ 0x09, "ISO_BACK_SLASH" },
		{ 0x0A, "LEFT_WINDOWS"   },

		{ 0x0B, "F1"             },
		{ 0x0C, "2"              },
		{ 0x07, "Q"              },
		{ 0x08, "A"              },
		{ 0x0F, "Z"              },
		{ 0x10, "LEFT_ALT"       },

		{ 0x11, "F2"             },
		{ 0x12, "3"              },
		{ 0x0D, "W"              },
		{ 0x0E, "S"              },
		{ 0x15, "X"              },

		{ 0x17, "F3"             },
		{ 0x18, "4"              },
		{ 0x13, "E"              },
		{ 0x14, "D"              },
		{ 0x1B, "C"              },

		{ 0x1C, "F4"             },
		{ 0x1D, "5"              },
		{ 0x19, "R"              },
		{ 0x1A, "F"              },
		{ 0x20, "V"              },

		{ 0x21, "6"              },
		{ 0x1E, "T"              },
		{ 0x1F, "G"              },
		{ 0x24, "B"              },
		{ 0x25, "SPACE"          },

		{ 0x30, "F5"             },
		{ 0x31, "7"              },
		{ 0x22, "Y"              },
		{ 0x23, "H"              },
		{ 0x34, "N"              },

		{ 0x35, "F6"             },
		{ 0x36, "8"              },
		{ 0x32, "U"              },
		{ 0x33, "J"              },
		{ 0x39, "M"              },

		{ 0x3B, "F7"             },
		{ 0x3C, "9"              },
		{ 0x37, "I"              },
		{ 0x38, "K"              },
		{ 0x3F, "COMMA"          },

		{ 0x41, "F8"             },
		{ 0x42, "0"              },
		{ 0x3D, "O"              },
		{ 0x3E, "L"              },
		{ 0x45, "PERIOD"         },
		{ 0x46, "RIGHT_ALT"      },

		{ 0x4E, "F9"             },
		{ 0x48, "MINUS"          },
		{ 0x43, "P"              },
		{ 0x44, "SEMICOLON"      },
		{ 0x4B, "FORWARD_SLASH"  },
		{ 0x4C, "RIGHT_FUNCTION" },

		{ 0x54, "F10"            },
		{ 0x4F, "EQUALS"         },
		{ 0x49, "LEFT_BRACKET"   },
		{ 0x4A, "QUOTE"          },
		{ 0x53, "MENU"           },

		{ 0x55, "F11"            },
		{ 0x57, "BACKSPACE"      },
		{ 0x50, "RIGHT_BRACKET"  },
		{ 0x60, "ISO_POUND"      },
		{ 0x52, "RIGHT_SHIFT"    },

		{ 0x56, "F12"            },
		{ 0x51, "ANSI_BACKSLASH" },
		{ 0x58, "ENTER"          },
		{ 0x59, "RIGHT_CONTROL"  },

		{ 0x63, "PRINT_SCREEN"   },
		{ 0x64, "INSERT"         },
		{ 0x65, "DELETE"         },
		{ 0x66, "LEFT_ARROW"     },

		{ 0x67, "SCROLL_LOCK"    },
		{ 0x68, "HOME"           },
		{ 0x69, "END"            },
		{ 0x6A, "UP_ARROW"       },
		{ 0x6B, "DOWN_ARROW"     },

		{ 0x6C, "PAUSE_BREAK"    },
		{ 0x6D, "PAGE_UP"        },
		{ 0x6E, "PAGE_DOWN"      },
		{ 0x6F, "RIGHT_ARROW"    },

		{ 0x71, "NUMPAD_LOCK"    },
		{ 0x72, "NUMPAD_7"       },
		{ 0x73, "NUMPAD_4"       },
		{ 0x74, "NUMPAD_1"       },
		{ 0x75, "NUMPAD_0"       },

		{ 0x77, "NUMPAD_DIVIDE"  },
		{ 0x78, "NUMPAD_8"       },
		{ 0x79, "NUMPAD_5"       },
		{ 0x7A, "NUMPAD_2"       },

		{ 0x7C, "NUMPAD_TIMES"   },
		{ 0x7D, "NUMPAD_9"       },
		{ 0x7E, "NUMPAD_6"       },
		{ 0x7F, "NUMPAD_3"       },
		{ 0x80, "NUMPAD_PERIOD"  },

		{ 0x81, "NUMPAD_MINUS"   },
		{ 0x82, "NUMPAD_PLUS"    },
		{ 0x83, "NUMPAD_ENTER"   }
	};

	inline std::unordered_map<uint16_t, std::string> VulcanTKL = {
		{ 0x02, "ESCAPE"                },
		{ 0x03, "BACK_TICK"             },
		{ 0x04, "TAB"                   },
		{ 0x05, "CAPS_LOCK"             },
		{ 0x00, "LEFT_SHIFT"            },
		{ 0x01, "LEFT_CONTROL"          },

		{ 0x08, "1"                     },
		{ 0x06, "ISO_BACK_SLASH"        },
		{ 0x07, "LEFT_WINDOWS"          },

		{ 0x0D, "F1"                    },
		{ 0x0E, "2"                     },
		{ 0x09, "Q"                     },
		{ 0x0A, "A"                     },
		{ 0x0B, "Z"                     },
		{ 0x0C, "LEFT_ALT"              },

		{ 0x14, "F2"                    },
		{ 0x15, "3"                     },
		{ 0x0F, "W"                     },
		{ 0x10, "S"                     },
		{ 0x11, "X"                     },

		{ 0x19, "F3"                    },
		{ 0x1A, "4"                     },
		{ 0x16, "E"                     },
		{ 0x17, "D"                     },
		{ 0x18, "C"                     },

		{ 0x1E, "F4"                    },
		{ 0x1F, "5"                     },
		{ 0x1B, "R"                     },
		{ 0x1C, "F"                     },
		{ 0x1D, "V"                     },

		{ 0x24, "6"                     },
		{ 0x20, "T"                     },
		{ 0x21, "G"                     },
		{ 0x22, "B"                     },
		{ 0x23, "SPACE"                 },

		{ 0x28, "F5"                    },
		{ 0x29, "7"                     },
		{ 0x25, "Y"                     },
		{ 0x26, "H"                     },
		{ 0x27, "N"                     },

		{ 0x2F, "F6"                    },
		{ 0x30, "8"                     },
		{ 0x2A, "U"                     },
		{ 0x2B, "J"                     },
		{ 0x2C, "M"                     },

		{ 0x35, "F7"                    },
		{ 0x36, "9"                     },
		{ 0x31, "I"                     },
		{ 0x32, "K"                     },
		{ 0x33, "COMMA"                 },

		{ 0x3B, "F8"                    },
		{ 0x3C, "0"                     },
		{ 0x37, "O"                     },
		{ 0x38, "L"                     },
		{ 0x39, "PERIOD"                },
		{ 0x3A, "RIGHT_ALT"             },

		{ 0x41, "F9"                    },
		{ 0x42, "MINUS"                 },
		{ 0x3D, "P"                     },
		{ 0x3E, "SEMICOLON"             },
		{ 0x3F, "FORWARD_SLASH"         },
		{ 0x40, "RIGHT_FUNCTION"        },

		{ 0x47, "F10"                   },
		{ 0x48, "EQUALS"                },
		{ 0x43, "LEFT_BRACKET"          },
		{ 0x44, "QUOTE"                 },
		{ 0x46, "MENU"                  },

		{ 0x4D, "F11"                   },
		{ 0x50, "BACKSPACE"             },
		{ 0x49, "RIGHT_BRACKET"         },
		{ 0x4A, "ISO_POUND"             },
		{ 0x4B, "RIGHT_SHIFT"           },

		{ 0x4F, "F12"                   },
		{ 0x51, "ANSI_BACK_SLASH"       },
		{ 0x52, "ENTER"                 },
		{ 0x4C, "RIGHT_CONTROL"         },

		{ 0x5C, "MEDIA_MUTE"            },
		{ 0x54, "INSERT"                },
		{ 0x55, "DELETE"                },
		{ 0x56, "LEFT_ARROW"            },

		{ 0x58, "HOME"                  },
		{ 0x59, "END"                   },
		{ 0x5A, "UP_ARROW"              },
		{ 0x5B, "DOWN_ARROW"            },

		{ 0x5D, "PAGE_UP"               },
		{ 0x5E, "PAGE_DOWN"             },
		{ 0x5F, "RIGHT_ARROW"           },
	};
}

// Todo once ???

#include <string>
#include <optional>
#include <vector>
#include <hidapi.h>

struct HIDUsage {
	int32_t					interface;
	std::optional<uint16_t>	usage_page;
	std::optional<uint16_t>	usage;
};

struct HIDDetectionInformation {
	std::string				name;
	uint16_t				vid;
	uint16_t				pid;
	std::vector<HIDUsage>	usages;
	bool compare(hid_device_info* dev_info, size_t usages_index) {
		// if (usages.size() <= usages_index) // TODO decide wether to throw error
		return (
			vid == dev_info->vendor_id
			&& pid == dev_info->product_id
			&& usages[usages_index].interface == dev_info->interface_number
			&& (!usages[usages_index].usage_page || usages[usages_index].usage_page == dev_info->usage_page)
			&& (!usages[usages_index].usage || usages[usages_index].usage == dev_info->usage)
			);
	}
};

static std::vector<HIDDetectionInformation> hid_devices {
	{
		"Roccat Vulcan 100 Aimo",
		0x1E7D,
		0x307A,
		{
			{
				0x0001,
				0x000B,
			},
			{
				0x0001,
				0x000A,
			},
			{
				0x0003,
				0x0001,
			}
		}
	},
	{
		"Roccat Vulcan TKL Pro",
		0x1E7D,
		0x311A,
		{
			{
				0x0001,
				0xFF01,
				0x0001
			},
			{
				0x0001,
				0xFF02,
				0x0001
			},
			{
				0x0003,
				0xFF00,
				0x0001
			}
		}
	}
};
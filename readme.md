# OpenRoccatAimoKeyboardDriver

This is a driver to support proprietary functions of most modern Roccat keyboards ([further information](#general-support)). I do not intent to support any other protocols, like for older Roccat keyboards, i'll make a separate driver for those.  
  
Note: I don't plan to add firmware updates in the near future, as those are very complicated and seemingly not worth the hassle.

- [OpenRoccatAimoKeyboardDriver](#openroccataimokeyboarddriver)
	- [Supported Devices](#supported-devices)
		- [Legend](#legend)
		- [General support](#general-support)
	- [building](#building)

## Supported Devices

### Legend

| Symbol | Meaning                                                     |
| :----: | :---------------------------------------------------------- |
|   ✔    | The device is fully implemented                             |
|   🛠    | The device is partially implemented                         |
|   🚧    | The device is not implemented but supported by the protocol |
|   ❌    | This device does not support this protocol                  |

### General support

[feature support list](supported_devices.md)

Note: Roccat has been bought by Turtle Beach some time ago and now they are rebranding all products, even retroactively, i don't know f there is any way to distinguish between new and old, but for now i am referring to all devices as Roccat

| Device                         | Support |
| ------------------------------ | :-----: |
| Roccat Vulcan 80 Aimo          |    🚧    |
| Roccat Vulcan 100 Aimo         |    🚧    |
| Roccat Vulcan 120/121/122 Aimo |    🚧    |
| Roccat Vulcan TKL              |    🚧    |
| Roccat Vulcan Pro              |    🚧    |
| Roccat Vulcan TKL Pro          |    🚧    |
| Roccat Pyro                    |    🚧    |
| Roccat Magma                   |    🚧    |
| Roccat Magma Mini              |    🚧    |
| Roccat Vulcan II               |    🚧    |
| Roccat Vulcan II Max           |    🚧    |
| Roccat Vulcan II Mini          |    🚧    |
| Roccat Vulcan II Mini Air      |    ❌    |

Right now the only devices i can implement are those i own, but i am missing Roccat Vulcan 80 Aimo, Roccat Vulcan II and Roccat Vulcan II Max.  If i find them cheap i'll buy them, but until that happens i need help. Also i only have one color each.

I am not sure about white versions, in general black versions are implemented, but i suspect, that they work too. I can confirm that 120, 121 and 122 are indistinguishable, not even Roccat Swarm knows which of the 3 is used, so they also work exactly the same.  
  
Roccat Vulcan II Mini Air is not compatible with this protocol and needs a seperate driver (WIP, but not soon).

## building

TODO
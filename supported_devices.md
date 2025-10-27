# Supported Devices

- [Supported Devices](#supported-devices)
		- [Legend](#legend)

## Legend

| Symbol | Meaning                                                     |
| :----: | :---------------------------------------------------------- |
|   ✔    | The feature is fully implemented for this device            |
|   🛠    | The feature is partially implemented for this device        |
|   🚧    | The device supports this feature, but it is not implemented |
|   ❌    | This device does not support this feature                   |

## Overview

| Device                         | Generation | General Information | Profile Management | Key Remapping | Lighting |
| ------------------------------ | :--------: | :-----------------: | :----------------: | :-----------: | :------: |
| Roccat Vulcan 80 Aimo          |     1      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan 100 Aimo         |     1      |          🛠          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan 120/121/122 Aimo |     1      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan TKL              |     1      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan Pro              |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan TKL Pro          |     2      |          🛠          |         🚧          |       🚧       |    🚧     |
| Roccat Pyro                    |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Magma                   |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Magma Mini              |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan II               |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan II Max           |     2      |          🚧          |         🚧          |       🚧       |    🚧     |
| Roccat Vulcan II Mini          |     2      |          🚧          |         🚧          |       🚧       |    🚧     |

## General Information

| Device                         | get_device_info | set_page_to_read | get_software_control | set_software_control | get_mute_light* | set_mute_light* | get_busy_state | get_sleep | set_sleep |
| ------------------------------ | :-------------: | :--------------: | :------------------: | :------------------: | :-------------: | :-------------: | :------------: | :-------: | :-------: |
| Roccat Vulcan 80 Aimo          |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     🚧     |     🚧     |
| Roccat Vulcan 100 Aimo         |        ✔        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       ✔        |     🚧     |     🚧     |
| Roccat Vulcan 120/121/122 Aimo |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     🚧     |     🚧     |
| Roccat Vulcan TKL              |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     🚧     |     🚧     |
| Roccat Vulcan Pro              |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Vulcan TKL Pro          |        ✔        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       ✔        |     ❌     |     ❌     |
| Roccat Pyro                    |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Magma                   |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Magma Mini              |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Vulcan II               |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Vulcan II Max           |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |
| Roccat Vulcan II Mini          |        🚧        |        🚧         |          🚧           |          🚧           |        🚧        |        🚧        |       🚧        |     ❌     |     ❌     |

* Gen1 supports setting the mute light, but directly via set_software_control

## Profile Management

| Device                         | get_active_profile | set_active_profile |
| ------------------------------ | :----------------: | :----------------: |
| Roccat Vulcan 80 Aimo          |         🚧          |         🚧          |
| Roccat Vulcan 100 Aimo         |         🚧          |         🚧          |
| Roccat Vulcan 120/121/122 Aimo |         🚧          |         🚧          |
| Roccat Vulcan TKL              |         🚧          |         🚧          |
| Roccat Vulcan Pro              |         🚧          |         🚧          |
| Roccat Vulcan TKL Pro          |         🚧          |         🚧          |
| Roccat Pyro                    |         🚧          |         🚧          |
| Roccat Magma                   |         🚧          |         🚧          |
| Roccat Magma Mini              |         🚧          |         🚧          |
| Roccat Vulcan II               |         🚧          |         🚧          |
| Roccat Vulcan II Max           |         🚧          |         🚧          |
| Roccat Vulcan II Mini          |         🚧          |         🚧          |

## Key Remapping

| Device                         | get_remapped_keys | set_remapped_keys | get_easy_shift | set_easy_shift | get_fn_remap | set_fn_remap | get_fn_extra | set_fn_extra | get_easy_shift_remap | set_easy_shift_remap | get_capslock_remap | set_capslock_remap | get_macro | set_macro |
| ------------------------------ | :---------------: | :---------------: | :------------: | :------------: | :----------: | :----------: | :----------: | :----------: | :------------------: | :------------------: | :----------------: | :----------------: | :-------: | :-------: |
| Roccat Vulcan 80 Aimo          |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      🚧       |      🚧       |          🚧           |          🚧           |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan 100 Aimo         |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      🚧       |      🚧       |          🚧           |          🚧           |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan 120/121/122 Aimo |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      🚧       |      🚧       |          🚧           |          🚧           |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan TKL              |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌*      |      ❌*      |          🚧           |          🚧           |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan Pro              |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan TKL Pro          |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Pyro                    |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Magma                   |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Magma Mini              |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II               |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II Max           |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II Mini          |         🚧         |         🚧         |       🚧        |       🚧        |      🚧       |      🚧       |      ❌       |      ❌       |          🚧           |          🚧           |         🚧          |         🚧          |     🚧     |     🚧     |

* techincally supported, but the package is ignored, as it is a remnant of the fullsize vulcans

## Lighting

| Device                         | get_lighting | set_lighting | set_direct_lighting |
| ------------------------------ | :----------: | :----------: | :-----------------: |
| Roccat Vulcan 80 Aimo*         |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan 100 Aimo         |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan 120/121/122 Aimo |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan TKL              |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan Pro              |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan TKL Pro          |      🚧       |      🚧       |          🚧          |
| Roccat Pyro                    |      🚧       |      🚧       |          🚧          |
| Roccat Magma                   |      🚧       |      🚧       |          🚧          |
| Roccat Magma Mini              |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan II               |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan II Max           |      🚧       |      🚧       |          🚧          |
| Roccat Vulcan II Mini          |      🚧       |      🚧       |          🚧          |

* this device only supports blue lighting

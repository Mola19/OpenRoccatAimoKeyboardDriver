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

| Device                         | Generation | General Information | Profile Management | Key Remapping | Lighting | Events |
| ------------------------------ | :--------: | :-----------------: | :----------------: | :-----------: | :------: | :----: |
| Roccat Vulcan 80 Aimo          |     1      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan 100 Aimo         |     1      |          ✔          |         ✔          |       🛠       |    ✔     |   🚧    |
| Roccat Vulcan 120/121/122 Aimo |     1      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan TKL              |     1      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan Pro              |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan TKL Pro          |     2      |          ✔          |         ✔          |       🛠       |    ✔     |   🚧    |
| Roccat Pyro                    |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Magma                   |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Magma Mini              |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan II               |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan II Max           |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |
| Roccat Vulcan II Mini          |     2      |          🚧          |         🚧          |       🚧       |    🚧     |   🚧    |

## General Information

| Device                         | get_device_info | set_page_to_read | get_busy_state |
| ------------------------------ | :-------------: | :--------------: | :------------: |
| Roccat Vulcan 80 Aimo          |        🚧        |        ❌         |       🚧        |
| Roccat Vulcan 100 Aimo         |        ✔        |        ❌         |       ✔        |
| Roccat Vulcan 120/121/122 Aimo |        🚧        |        ❌         |       🚧        |
| Roccat Vulcan TKL              |        🚧        |        ❌         |       🚧        |
| Roccat Vulcan Pro              |        🚧        |        🚧         |       🚧        |
| Roccat Vulcan TKL Pro          |        ✔        |        ✔         |       ✔        |
| Roccat Pyro                    |        🚧        |        🚧         |       🚧        |
| Roccat Magma                   |        🚧        |        🚧         |       🚧        |
| Roccat Magma Mini              |        🚧        |        🚧         |       🚧        |
| Roccat Vulcan II               |        🚧        |        🚧         |       🚧        |
| Roccat Vulcan II Max           |        🚧        |        🚧         |       🚧        |
| Roccat Vulcan II Mini          |        🚧        |        🚧         |       🚧        |

These functions are not the same for Generation 1 and 2. For convenience use the wrapper functions `get_software_state` and `set_software_state` in combination with the config variable `has_adjustable_time_to_sleep`.

Generation 1 devices don't support setting time to sleep (as far as we know). 

| Device                         | get_software_state | set_software_state | get_software_state_gen1 | set_software_state_gen1 | get_software_control_state_gen2 | set_software_control_state_gen2 | get_software_state_gen2 | set_software_state_gen2 |
| ------------------------------ | :----------------: | :----------------: | :---------------------: | :---------------------: | :-----------------------------: | :-----------------------------: | :---------------------: | :---------------------: |
| Roccat Vulcan 80 Aimo          |         🚧          |         🚧          |            🚧            |            🚧            |                ❌                |                ❌                |            ❌            |            ❌            |
| Roccat Vulcan 100 Aimo         |         ✔          |         ✔          |            ✔            |            ✔            |                ❌                |                ❌                |            ❌            |            ❌            |
| Roccat Vulcan 120/121/122 Aimo |         🚧          |         🚧          |            🚧            |            🚧            |                ❌                |                ❌                |            ❌            |            ❌            |
| Roccat Vulcan TKL              |         🚧          |         🚧          |            🚧            |            🚧            |                ❌                |                ❌                |            ❌            |            ❌            |
| Roccat Vulcan Pro              |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Vulcan TKL Pro          |         ✔          |         ✔          |            ❌            |            ❌            |                ✔                |                ✔                |            ✔            |            ✔            |
| Roccat Pyro                    |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Magma                   |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Magma Mini              |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Vulcan II               |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Vulcan II Max           |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |
| Roccat Vulcan II Mini          |         🚧          |         🚧          |            ❌            |            ❌            |                🚧                |                🚧                |            🚧            |            🚧            |

* Gen1 supports setting the mute light, but directly via set_software_control

## Profile Management

| Device                         | get_active_profile | set_active_profile |
| ------------------------------ | :----------------: | :----------------: |
| Roccat Vulcan 80 Aimo          |         🚧          |         🚧          |
| Roccat Vulcan 100 Aimo         |         ✔          |         ✔          |
| Roccat Vulcan 120/121/122 Aimo |         🚧          |         🚧          |
| Roccat Vulcan TKL              |         🚧          |         🚧          |
| Roccat Vulcan Pro              |         🚧          |         🚧          |
| Roccat Vulcan TKL Pro          |         ✔          |         ✔          |
| Roccat Pyro                    |         🚧          |         🚧          |
| Roccat Magma                   |         🚧          |         🚧          |
| Roccat Magma Mini              |         🚧          |         🚧          |
| Roccat Vulcan II               |         🚧          |         🚧          |
| Roccat Vulcan II Max           |         🚧          |         🚧          |
| Roccat Vulcan II Mini          |         🚧          |         🚧          |

## Key Remapping

| Device                         | get_gamemode_remap | set_gamemode_remap | get_easyshift | set_easyshift | get_fn_remap | set_fn_remap | get_fn_extra | set_fn_extra | get_easyshift_remap | set_easyshift_remap | get_capslock_remap | set_capslock_remap | get_macro | set_macro |
| ------------------------------ | :----------------: | :----------------: | :-----------: | :-----------: | :----------: | :----------: | :----------: | :----------: | :-----------------: | :-----------------: | :----------------: | :----------------: | :-------: | :-------: |
| Roccat Vulcan 80 Aimo          |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      🚧       |      🚧       |          🚧          |          🚧          |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan 100 Aimo         |         ✔          |         ✔          |       ✔       |       ✔       |      ✔       |      ✔       |      ✔       |      ✔       |          ✔          |          ✔          |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan 120/121/122 Aimo |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      🚧       |      🚧       |          🚧          |          🚧          |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan TKL              |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌*      |      ❌*      |          🚧          |          🚧          |         🚧          |         🚧          |     ❌     |     ❌     |
| Roccat Vulcan Pro              |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan TKL Pro          |         ✔          |         ✔          |       ✔       |       ✔       |      ✔       |      ✔       |      ❌       |      ❌       |          ✔          |          ✔          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Pyro                    |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Magma                   |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Magma Mini              |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II               |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II Max           |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |
| Roccat Vulcan II Mini          |         🚧          |         🚧          |       🚧       |       🚧       |      🚧       |      🚧       |      ❌       |      ❌       |          🚧          |          🚧          |         🚧          |         🚧          |     🚧     |     🚧     |

* techincally supported, but the package is ignored, as it is a remnant of the fullsize vulcans

## Lighting

| Device                         | get_lighting | set_lighting | set_direct_lighting | get_lighting_state | set_lighting_state |
| ------------------------------ | :----------: | :----------: | :-----------------: | :----------------: | :----------------: |
| Roccat Vulcan 80 Aimo*         |      🚧       |      🚧       |          🚧          |         🚧          |         🚧          |
| Roccat Vulcan 100 Aimo         |      ✔       |      ✔       |          ✔          |         ✔          |         ✔          |
| Roccat Vulcan 120/121/122 Aimo |      🚧       |      🚧       |          🚧          |         🚧          |         🚧          |
| Roccat Vulcan TKL              |      🚧       |      🚧       |          🚧          |         🚧          |         🚧          |
| Roccat Vulcan Pro              |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Vulcan TKL Pro          |      ✔       |      ✔       |          ✔          |         ❌          |         ❌          |
| Roccat Pyro                    |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Magma                   |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Magma Mini              |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Vulcan II               |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Vulcan II Max           |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |
| Roccat Vulcan II Mini          |      🚧       |      🚧       |          🚧          |         ❌          |         ❌          |

* this device only supports blue lighting

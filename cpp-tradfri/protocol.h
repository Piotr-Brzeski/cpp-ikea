//
//  protocol.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-20.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include <cstdint>

namespace protocol {

namespace device {

using type = std::uint8_t;
constexpr type button = 1;

using state = std::uint8_t;
constexpr state click = 1;

struct id {
	type    type;
	uint8_t number;
};

}

// TODO: Move to other file
class device_state_id {
public:
	device_state_id(device::type type, uint8_t number, device::state state)
		: m_value(((type * 256) + number)*256 + state)
	{
	}
	device_state_id(device::id id, device::state state)
		: device_state_id(id.type, id.number, state)
	{
	}
	
	bool operator<(device_state_id other) const {
		return m_value < other.m_value;
	}
private:
	std::uint32_t m_value;
};


}

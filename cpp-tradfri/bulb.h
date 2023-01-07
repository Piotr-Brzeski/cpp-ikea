//
//  bulb.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"

namespace tradfri {

class bulb: public device {
public:
	bulb(std::string const& id, std::string const& name);
	
private:
	bool         m_enabled;
	std::uint8_t m_brightness;   // 1, 44, 87, 130, 173, 216, 254
	std::uint8_t m_themperature; // 0-4
};

}

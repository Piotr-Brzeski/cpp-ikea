//
//  bulb.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device_with_brightness.h"

namespace tradfri {

class bulb: public device_with_brightness {
public:
	static constexpr int system_id = 2;
	static bulb load(std::string const& id, coap_connection& coap, json const& json);
	
private:
	bulb(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json) override;
	std::string const& command(std::uint8_t brightness) override;
};

}

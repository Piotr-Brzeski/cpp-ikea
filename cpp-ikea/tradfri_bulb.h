//
//  tradfri_bulb.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "tradfri_device_with_brightness.h"

namespace ikea {

class tradfri_bulb: public tradfri_device_with_brightness {
public:
	static constexpr int system_id = 2;
	static tradfri_bulb load(std::string const& id, coap_connection& coap, json const& json);
	
private:
	tradfri_bulb(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json) override;
	std::string const& command(std::uint8_t brightness) override;
};

}

//
//  tradfri_group.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "tradfri_device_with_brightness.h"

namespace ikea {

class tradfri_group: public tradfri_device_with_brightness {
public:
	static std::string load(coap_connection& coap, std::string const& id);
	static tradfri_group load(std::string const& id, coap_connection& coap, json const& json);
	
private:
	tradfri_group(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json) override;
	std::string const& command(std::uint8_t brightness) override;
};

}
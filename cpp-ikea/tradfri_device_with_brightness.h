//
//  tradfri_device_with_brightness.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "tradfri_device.h"
#include "device_with_brightness.h"

namespace ikea {

class tradfri_device_with_brightness: public tradfri_device, public device_with_brightness {
public:
	tradfri_device_with_brightness(std::string&& uri, coap_connection& coap, json const& json);
	
protected:
	static std::string const& get_command(std::uint8_t brightness);
	
	void update_brightness(json_value const& status);
//	virtual std::string const& command(std::uint8_t brightness) = 0;
};

}

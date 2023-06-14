//
//  device_with_brightness.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"

namespace tradfri {

class device_with_brightness: public device {
public:
	device_with_brightness(std::string&& uri, coap_connection& coap, json const& json);
	
	std::uint8_t brightness();
	
	void set(bool enabled);
	void set(std::uint8_t brightness);
	void toggle();
	void increase();
	void decrease();
	
	static std::string const& get_command(std::uint8_t brightness);
	static std::uint8_t max_brightness();
	
protected:
	void update_brightness(json_value const& status);
	virtual void update() = 0;
	virtual std::string const& command(std::uint8_t brightness) = 0;
	
private:
	std::uint8_t m_brightness = 0;
};

}

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
	static constexpr int system_id = 2;
	static bulb load(std::string&& id, coap_connection& coap, json const& json);
	
	std::uint8_t brightness();
	
	void set(std::uint8_t brightness);
	void toggle();
	void increase();
	void decrease();
	
private:
	bulb(std::string&& id, coap_connection& coap, json const& json);
	void update(json const& json);
	void update();
	
	std::uint8_t m_brightness;
};

}

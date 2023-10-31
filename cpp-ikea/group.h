//
//  group.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device_with_brightness.h"

namespace ikea {

class group: public device_with_brightness {
public:
	static std::string load(coap_connection& coap, std::string const& id);
	static group load(std::string const& id, coap_connection& coap, json const& json);
	
private:
	group(std::string const& id, coap_connection& coap, json const& json);
	void update(json const& json) override;
	std::string const& command(std::uint8_t brightness) override;
};

}

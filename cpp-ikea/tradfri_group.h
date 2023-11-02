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
	tradfri_group(std::string const& id, coap_connection& coap, json const& json);
	
private:
	void update_state(json const& json);
	void get_state() override;
	void send_state() override;
	
};

}

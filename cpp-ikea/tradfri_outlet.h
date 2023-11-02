//
//  tradfri_outlet.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "tradfri_device.h"
#include "device_without_brightness.h"

namespace ikea {

class tradfri_outlet: public tradfri_device, public device_without_brightness {
public:
	static constexpr int system_id = 3;
	tradfri_outlet(std::string const& id, coap_connection& coap, json const& json);
	
private:
	void update_state(json const& json);
	void get_state() override;
	void send_state() override;
};

}

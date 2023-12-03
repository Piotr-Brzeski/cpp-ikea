//
//  dirigera_bulb.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "dirigera_device.h"
#include "device_with_brightness.h"

namespace ikea {

class dirigera_bulb: public dirigera_device, public device_with_brightness {
public:
	static std::string const device_type;
	
	dirigera_bulb(std::string const& devices_uri, http_get& get_connection, json_value const& json);
	dirigera_bulb(dirigera_bulb&& bulb);
	
	void update_state(json_value const& json) override;
	
private:
	void get_state() override;
	void send_state() override;
	
	std::atomic<int> m_raw_brighness = 0;
};

}


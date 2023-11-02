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
	
	dirigera_bulb(std::string const& devices_uri, http_connection& connection, json_value const& json);
	
private:
	void update_state(json_value const& json);
	void get_state() override;
	void send_state() override;
};

}


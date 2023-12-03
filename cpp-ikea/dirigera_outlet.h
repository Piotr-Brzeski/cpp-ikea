//
//  dirigera_outlet.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "dirigera_device.h"
#include "device_without_brightness.h"

namespace ikea {

class dirigera_outlet: public dirigera_device, public device_without_brightness {
public:
	static std::string const device_type;
	
	dirigera_outlet(std::string const& devices_uri, http_get& get_connection, json_value const& json);
	
	void update_state(json_value const& json) override;
	
private:
	void get_state() override;
	void send_state() override;
};

}


//
//  system.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "coap_connection.h"

namespace tradfri {

class system {
public:
	system(std::string const& ip, std::string const& identity, std::string const& key);
	
	void enumerate_devices();
	
private:
	void load_device(std::string const& id);
	
	coap_connection m_coap;
};

}

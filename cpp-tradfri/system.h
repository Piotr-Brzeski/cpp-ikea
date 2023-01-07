//
//  system.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "coap_connection.h"
#include "bulb.h"
#include "plug.h"
#include <vector>

namespace tradfri {

class system {
public:
	system(std::string const& ip, std::string const& identity, std::string const& key);
	
	void enumerate_devices();
	
	std::vector<bulb>& bulbs() {
		return m_bulbs;
	}
	std::vector<plug>& plugs() {
		return m_plugs;
	}

private:
	void load_device(std::string id);
	
	coap_connection   m_coap;
	std::vector<bulb> m_bulbs;
	std::vector<plug> m_plugs;
};

}

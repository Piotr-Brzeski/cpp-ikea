//
//  tradfri.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "coap_connection.h"
#include "tradfri_bulb.h"
#include "tradfri_outlet.h"
#include <vector>

namespace ikea {

class tradfri {
public:
	struct configuration {
		std::string ip;
		std::string identity;
		std::string key;
	};
	
	tradfri(configuration const& configuration);
	tradfri(std::string const& ip, std::string const& identity, std::string const& key);
	
	void enumerate_devices();
	
	std::vector<tradfri_bulb>& bulbs() {
		return m_bulbs;
	}
	std::vector<tradfri_outlet>& outlets() {
		return m_outlets;
	}
	
private:
	void load_device(std::string const& id);
	
	coap_connection             m_coap;
	std::vector<tradfri_bulb>   m_bulbs;
	std::vector<tradfri_outlet> m_outlets;
};

}

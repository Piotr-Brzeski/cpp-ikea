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
#include "tradfri_group.h"
#include <vector>
#include <functional>

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
	std::vector<tradfri_group>& groups() {
		return m_groups;
	}
	
	std::function<void(bool)> set_operation(std::string const& device_name);
	std::function<std::uint8_t()> brightness_operation(std::string const& device_name);
	std::function<void()> toggle_operation(std::string const& device_name);
	std::function<void()> increase_operation(std::string const& device_name);
	std::function<void()> decrease_operation(std::string const& device_name);
	
private:
	void load_device(std::string const& id);
	void load_group(std::string const& id);
	
	coap_connection             m_coap;
	std::vector<tradfri_bulb>   m_bulbs;
	std::vector<tradfri_outlet> m_outlets;
	std::vector<tradfri_group>  m_groups;
};

}

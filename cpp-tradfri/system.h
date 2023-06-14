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
#include "group.h"
#include <vector>
#include <functional>

namespace tradfri {

class system {
public:
	struct configuration {
		std::string ip;
		std::string identity;
		std::string key;
	};
	
	system(configuration const& configuration);
	system(std::string const& ip, std::string const& identity, std::string const& key);
	
	void enumerate_devices();
	
	std::vector<bulb>& bulbs() {
		return m_bulbs;
	}
	std::vector<plug>& plugs() {
		return m_plugs;
	}
	std::vector<group>& groups() {
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
	
	coap_connection    m_coap;
	std::vector<bulb>  m_bulbs;
	std::vector<plug>  m_plugs;
	std::vector<group> m_groups;
};

}

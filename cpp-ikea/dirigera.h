//
//  dirigera.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "http_connection.h"
#include "dirigera_outlet.h"
#include "dirigera_bulb.h"
#include "ws_connection.h"
#include <algorithm>
#include <vector>
#include <map>

namespace ikea {

class dirigera {
public:
	struct configuration {
		std::string ip;
		std::string access_token;
	};
	
	dirigera(configuration const& configuration);
	dirigera(std::string const& ip, std::string const& access_token);
	
	dirigera(dirigera const&) = delete;
	dirigera(dirigera&&) = delete;
	dirigera& operator=(dirigera const&) = delete;
	dirigera& operator=(dirigera&&) = delete;
	
	void enumerate_devices();
	
	auto bulbs() {
		auto v = std::vector<device_with_brightness*>();
		std::ranges::for_each(m_bulbs, [&v](auto& x){ v.push_back(&x); });
		return v;
	}
	auto outlets() {
		auto v = std::vector<device_without_brightness*>();
		std::ranges::for_each(m_outlets, [&v](auto& x){ v.push_back(&x); });
		return v;
	}
	
private:
	void update(std::string message);
	template<class Devices>
	void register_devices(Devices& devices);
	
	std::vector<dirigera_bulb>              m_bulbs;
	std::vector<dirigera_outlet>            m_outlets;
	std::map<std::string, dirigera_device*> m_devices;
	std::string const                       m_uri;
	http_connection                         m_connection;
	ws_connection                           m_ws;
};

}

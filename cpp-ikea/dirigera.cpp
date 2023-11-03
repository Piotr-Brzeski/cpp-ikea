//
//  dirigera.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera.h"
#include <iostream>

using namespace ikea;

dirigera::dirigera(configuration const& configuration)
	: dirigera(configuration.ip, configuration.access_token)
{
}

dirigera::dirigera(std::string const& ip, std::string const& access_token)
	: m_uri("https://" + ip + ":8443/v1/devices/")
	, m_connection(access_token)
{
}

void dirigera::enumerate_devices() {
	auto response = m_connection.get(m_uri);
	std::cout << response << std::endl;
	auto devices_json = json(std::move(response));
	auto devices = devices_json.get();
	for(std::size_t i = 0; i < devices.size(); ++i) {
		auto device_json = devices[i];
		auto type = device_json["deviceType"].get_string();
		if(type == dirigera_outlet::device_type) {
			m_outlets.emplace_back(m_uri, m_connection, device_json);
		}
		else if(type == dirigera_bulb::device_type) {
			m_bulbs.emplace_back(m_uri, m_connection, device_json);
		}
//		else {
//			std::cout << "Type: " << type << std::endl;
//		}
	}
}


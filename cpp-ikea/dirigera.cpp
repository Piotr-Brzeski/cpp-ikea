//
//  dirigera.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera.h"
#include "exception.h"
//#include <iostream>

using namespace ikea;

dirigera::dirigera(configuration const& configuration)
	: dirigera(configuration.ip, configuration.access_token)
{
}

dirigera::dirigera(std::string const& ip, std::string const& access_token)
	: m_uri("https://" + ip + ":8443/v1/devices/")
	, m_connection(access_token)
	, m_ws("wss://" + ip + ":8443/", access_token)
{
}

void dirigera::enumerate_devices() {
	auto response = m_connection.get(m_uri);
//	std::cout << response << std::endl;
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
	register_devices(m_outlets);
	register_devices(m_bulbs);
	m_ws.start(std::bind(&dirigera::update, this, std::placeholders::_1));
}

void dirigera::update(std::string message) {
	auto message_json = json(std::move(message));
	if(message_json["type"].get_string() != "deviceStateChanged") {
		return;
	}
	auto device_json = message_json["data"];
	auto id = device_json["id"].get_string();
	auto it = m_devices.find(id);
	if(it == m_devices.end()) {
		return;
	}
//	auto type = device_json["deviceType"].get_string();
	it->second->update_state(device_json);
}

template<class Devices>
void dirigera::register_devices(Devices& devices) {
	for(auto& device : devices) {
		if(!m_devices.emplace(device.id(), &device).second) {
			throw exception("Can not register device \"" + device.name() + "\" with id = " + device.id());
		}
	}
}

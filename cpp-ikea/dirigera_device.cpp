//
//  dirigera_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_device.h"
#include <cpp-log/log.h>

using namespace ikea;

dirigera_device::dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json)
	: device(devices_uri + json["id"].get_string(), json["attributes"]["customName"].get_string())
	, m_connection(connection)
{
}

void dirigera_device::update_state() {
//	try {
		update(json(dirigera_device::load()).get());
//	}
//	catch(std::exception &e) {
//		logger::log("Update device state failed");
//		// Sometimes tradfri gateway returns json without current state.
//		// Ignore for now.
//	}
}

std::string dirigera_device::load() {
	auto response = m_connection.get_response(uri());
	updated();
	return response;
}

void dirigera_device::set(std::string const& state) {
	m_connection.send_patch(uri(), state);
	updated();
}

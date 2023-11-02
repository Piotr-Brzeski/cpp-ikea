//
//  dirigera_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_device.h"
#include <cpp-log/log.h>
#include <chrono>
#include <thread>

using namespace ikea;

//std::string device::load(http_connection& connection, std::string const& id) {
//	return connection.get_response(uri_prefix + id);
//}

dirigera_device::dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json)
	: m_uri(devices_uri + json["id"].get_string())
	, m_name(json["attributes"]["customName"].get_string())
	, m_connection(connection)
	, m_last_update_time(std::chrono::steady_clock::now())
{
}

bool dirigera_device::needs_update() const {
	constexpr auto threshold = std::chrono::milliseconds(500);
	auto now = std::chrono::steady_clock::now();
	return (now - m_last_update_time) > threshold;
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
	auto response = m_connection.get_response(m_uri);
	m_last_update_time = std::chrono::steady_clock::now();
	return response;
}

void dirigera_device::set(std::string const& state) {
	m_connection.send_patch(m_uri, state);
	m_last_update_time = std::chrono::steady_clock::now();
}

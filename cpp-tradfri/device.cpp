//
//  device.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device.h"
#include <cpp-log/log.h>
#include <chrono>
#include <thread>

using namespace tradfri;

namespace {

std::string load_name(json const& json) {
	static const auto name_key = std::string("9001");
	return json[name_key].get_string();
}

}

std::string device::load(coap_connection& coap, std::string const& id) {
	return coap.get(uri_prefix + id);
}

std::string const device::uri_prefix = "15001/";

device::device(std::string&& uri, coap_connection& coap, json const& json)
	: m_uri(std::move(uri))
	, m_name(load_name(json))
	, m_coap(coap)
	, m_last_update_time(std::chrono::steady_clock::now())
{
}

bool device::needs_update() const {
	constexpr auto threshold = std::chrono::milliseconds(500);
	auto now = std::chrono::steady_clock::now();
	return (now - m_last_update_time) > threshold;
}

void device::update_state() {
//	try {
		update(json(device::load()));
//	}
//	catch(std::exception &e) {
//		logger::log("Update device state failed");
//		// Sometimes tradfri gateway returns json without current state.
//		// Ignore for now.
//	}
}

std::string device::load() {
	auto response = m_coap.get(m_uri);
	m_last_update_time = std::chrono::steady_clock::now();
	return response;
}

void device::set(std::string const& state) {
	m_coap.put(m_uri, state);
	m_last_update_time = std::chrono::steady_clock::now();
}

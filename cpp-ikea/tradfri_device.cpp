//
//  tradfri_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_device.h"
#include <cpp-log/log.h>

using namespace ikea;

namespace {

std::string load_name(json const& json) {
	static const auto name_key = std::string("9001");
	return json[name_key].get_string();
}

}

std::string tradfri_device::load(coap_connection& coap, std::string const& id) {
	return coap.get(uri_prefix + id);
}

std::string const tradfri_device::uri_prefix = "15001/";

tradfri_device::tradfri_device(std::string&& uri, coap_connection& coap, json const& json)
	: device(std::move(uri), load_name(json))
	, m_coap(coap)
{
}

void tradfri_device::update_state() {
//	try {
		update(json(tradfri_device::load()));
//	}
//	catch(std::exception &e) {
//		logger::log("Update device state failed");
//		// Sometimes tradfri gateway returns json without current state.
//		// Ignore for now.
//	}
}

std::string tradfri_device::load() {
	auto response = m_coap.get(uri());
	updated();
	return response;
}

void tradfri_device::set(std::string const& state) {
	m_coap.put(uri(), state);
	updated();
}

//
//  tradfri_outlet.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_outlet.h"

using namespace ikea;

namespace {

std::string const& command(bool enabled) {
	static auto on  = std::string("{\"3311\":[{\"5850\":1}]}");
	static auto off = std::string("{\"3311\":[{\"5850\":0}]}");
	if(enabled) {
		return on;
	}
	return off;
}

}

tradfri_outlet::tradfri_outlet(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device(tradfri_device::uri_prefix + id, coap, json)
{
	update_state(json);
}

void tradfri_outlet::update_state(json const& json) {
	static const auto status_key = std::string("3312");
	static const auto enabled_key = std::string("5850");
	auto enabled = json[status_key][0][enabled_key].get_int();
	internal_update(enabled);
}

void tradfri_outlet::get_state() {
	auto status_json = json(m_coap.get(m_uri));
	update_state(status_json);
}

void tradfri_outlet::send_state() {
	auto& state = command(m_enabled_to_send);
	m_coap.put(m_uri, state);
}

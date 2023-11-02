//
//  group.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_group.h"
#include "exception.h"
#include <vector>

using namespace ikea;

namespace {
	auto constexpr group_uri_prefix = "15004/";
}

std::string tradfri_group::load(coap_connection& coap, std::string const& id) {
	return coap.get(group_uri_prefix + id);
}

tradfri_group::tradfri_group(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device_with_brightness(group_uri_prefix + id, coap, json)
{
	update_state(json);
}

void tradfri_group::update_state(json const& json) {
	update_brightness(json.get());
}

void tradfri_group::get_state() {
	auto status_json = json(m_coap.get(m_uri));
	update_state(status_json);
}

void tradfri_group::send_state() {
	auto& state = get_command(m_brightness_to_send);
	m_coap.put(m_uri, state);
}

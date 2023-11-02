//
//  tradfri_bulb.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_bulb.h"
#include "exception.h"
#include <vector>

using namespace ikea;

tradfri_bulb::tradfri_bulb(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device_with_brightness(tradfri_device::uri_prefix + id, coap, json)
{
	update_state(json);
}

void tradfri_bulb::update_state(json const& json) {
	static const auto status_key = std::string("3311");
	auto status_json = json[status_key][0];
	update_brightness(status_json);
}

void tradfri_bulb::get_state() {
	auto state_json = json(m_coap.get(m_uri));
	update_state(state_json);
}

void tradfri_bulb::send_state() {
	auto& state = get_command(m_brightness_to_send);
	m_coap.put(m_uri, state);
}

std::string const& tradfri_bulb::get_command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		command = "{\"3311\":[" + tradfri_device_with_brightness::get_command(brightness) + "]}";
	}
	return command;
}

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

std::string const& tradfri_bulb::command(std::uint8_t brightness) {
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

tradfri_bulb::tradfri_bulb(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device_with_brightness(tradfri_device::uri_prefix + id, coap, json)
{
}

tradfri_bulb tradfri_bulb::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_bulb = tradfri_bulb(id, coap, json);
	new_bulb.update(json);
	return new_bulb;
}

void tradfri_bulb::update(json const& json) {
	static const auto status_key = std::string("3311");
	auto status = json[status_key][0];
	update_brightness(status);
}

//
//  bulb.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "bulb.h"
#include "exception.h"
#include <array>
#include <vector>

using namespace tradfri;

std::string const& bulb::command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		command = "{\"3311\":[" + device_with_brightness::get_command(brightness) + "]}";
	}
	return command;
}

bulb::bulb(std::string const& id, coap_connection& coap, json const& json)
	: device_with_brightness(device::uri_prefix + id, coap, json)
{
}

bulb bulb::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_bulb = bulb(id, coap, json);
	new_bulb.update(json);
	return new_bulb;
}

void bulb::update(json const& json) {
	static const auto status_key = std::string("3311");
	auto status = json[status_key][0];
	update_brightness(status);
}

void bulb::update() {
	update(json(device::load()));
}

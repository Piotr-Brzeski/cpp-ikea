//
//  group.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "group.h"
#include "exception.h"
#include <vector>

using namespace tradfri;

namespace {
	auto constexpr group_uri_prefix = "15004/";
}

std::string const& group::command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		command = device_with_brightness::get_command(brightness);
	}
	return command;
}

group::group(std::string const& id, coap_connection& coap, json const& json)
	: device_with_brightness(group_uri_prefix + id, coap, json)
{
}

std::string group::load(coap_connection& coap, std::string const& id) {
	return coap.get(group_uri_prefix + id);
}

group group::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_group = group(id, coap, json);
	new_group.update(json);
	return new_group;
}

void group::update(json const& json) {
	update_brightness(json.get());
}

void group::update() {
	update(json(device::load()));
}

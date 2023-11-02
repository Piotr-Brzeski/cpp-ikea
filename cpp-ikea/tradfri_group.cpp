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

std::string const& tradfri_group::command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		command = tradfri_device_with_brightness::get_command(brightness);
	}
	return command;
}

tradfri_group::tradfri_group(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device_with_brightness(group_uri_prefix + id, coap, json)
{
}

std::string tradfri_group::load(coap_connection& coap, std::string const& id) {
	return coap.get(group_uri_prefix + id);
}

tradfri_group tradfri_group::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_group = tradfri_group(id, coap, json);
	new_group.update(json);
	return new_group;
}

void tradfri_group::update(json const& json) {
	update_brightness(json.get());
}

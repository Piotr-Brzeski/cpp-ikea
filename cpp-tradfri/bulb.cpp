//
//  bulb.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "bulb.h"
#include "exception.h"
#include "json.h"
#include <array>
#include <vector>

using namespace tradfri;

namespace {

constexpr auto brightness_levels = std::array<int, 8>{0, 1, 44, 87, 130, 173, 216, 254};

std::uint8_t brightness(int raw_value) {
	if(raw_value < brightness_levels[1]) return 0;
	if(raw_value < brightness_levels[2]) return 1;
	if(raw_value < brightness_levels[3]) return 2;
	if(raw_value < brightness_levels[4]) return 3;
	if(raw_value < brightness_levels[5]) return 4;
	if(raw_value < brightness_levels[6]) return 5;
	if(raw_value < brightness_levels[7]) return 6;
	return 7;
}

std::string const& command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		if(brightness == 0) {
			command = "{\"3311\":[{\"5850\":0}]}"; // off
		}
		else {
			command =
				"{\"3311\":[{\"5850\":1,\"5851\":" +            // on
				std::to_string(brightness_levels[brightness]) + // brightness
				",\"5706\":\"f5faf6\"}]}";                      // white lighs
		}
	}
	return command;
}

}

bulb::bulb(std::string&& id, coap_connection& coap, json const& json)
	: device(std::move(id), coap, json)
{
}

bulb bulb::load(std::string&& id, coap_connection& coap, json const& json) {
	auto new_bulb = bulb(std::move(id), coap, json);
	new_bulb.update(json);
	return new_bulb;
}

void bulb::update() {
	update(json(device::load()));
}

void bulb::set(std::uint8_t brightness) {
	auto& data = command(brightness);
	m_coap.put(m_uri, data);
	m_brightness = brightness;
}

void bulb::update(json const& json) {
	static const auto status_key = std::string("3311");
	static const auto enabled_key = std::string("5850");
	static const auto brightness_key = std::string("5851");
	auto status = json[status_key][0];
	auto enabled = status[enabled_key].get_int();
	if(enabled == 1) {
		auto raw_brightness = status[brightness_key].get_int();
		m_brightness = ::brightness(raw_brightness);
	}
	else {
		m_brightness = 0;
	}
//	auto temperature = status["5706"].get_string();
}

//
//  device_with_brightness.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "bulb.h"
#include "exception.h"
#include <array>
#include <vector>

using namespace tradfri;

namespace {

constexpr auto brightness_levels = std::array<int, 8>{0, 1, 44, 87, 130, 173, 216, 254};
constexpr auto max_brightness = brightness_levels.size() - 1;

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

}

std::string const& device_with_brightness::get_command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		if(brightness == 0) {
			command = "{\"5850\":0}"; // off
		}
		else {
			command =
				"{\"5850\":1,\"5851\":" +                       // on
				std::to_string(brightness_levels[brightness]) + // brightness
				",\"5706\":\"f5faf6\"}";                        // white lights
		}
	}
	return command;
}

device_with_brightness::device_with_brightness(std::string&& uri, coap_connection& coap, json const& json)
	: device(std::move(uri), coap, json)
{
}

std::uint8_t device_with_brightness::brightness(){
	if(needs_update()) {
		update();
	}
	return m_brightness;
}

void device_with_brightness::set(std::uint8_t brightness) {
	auto& state_command = command(brightness);
	device::set(state_command);
	m_brightness = brightness;
}

void device_with_brightness::toggle() {
	set(brightness() == 0 ? max_brightness : 0);
}

void device_with_brightness::increase() {
	if(brightness() < max_brightness) {
		set(m_brightness + 1);
	}
}

void device_with_brightness::decrease() {
	if(brightness() > 0) {
		set(m_brightness - 1);
	}
}

void device_with_brightness::update_brightness(json_value const& status) {
	static const auto enabled_key = std::string("5850");
	static const auto brightness_key = std::string("5851");
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
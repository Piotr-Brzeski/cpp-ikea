//
//  tradfri_device_with_brightness.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-06-10.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_device_with_brightness.h"
#include "exception.h"
#include <cpp-log/log.h>
#include <array>
#include <vector>

using namespace ikea;

namespace {

constexpr auto brightness_levels = std::array<int, 8>{0, 2, 44, 87, 130, 173, 216, 254};
constexpr auto max_brightness_value = static_cast<std::uint8_t>(brightness_levels.size() - 1);
constexpr auto zero_brightness_value = static_cast<std::uint8_t>(0);

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

std::string const& tradfri_device_with_brightness::get_command(std::uint8_t brightness) {
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

std::uint8_t tradfri_device_with_brightness::max_brightness() {
	return max_brightness_value;
}

tradfri_device_with_brightness::tradfri_device_with_brightness(std::string&& uri, coap_connection& coap, json const& json)
	: tradfri_device(std::move(uri), coap, json)
{
}

std::uint8_t tradfri_device_with_brightness::brightness() {
	if(needs_update()) {
		update_state();
	}
	return m_brightness;
}

void tradfri_device_with_brightness::set(bool enabled) {
	set(enabled ? max_brightness_value : zero_brightness_value);
}

void tradfri_device_with_brightness::set(std::uint8_t brightness) {
	auto& state_command = command(brightness);
	tradfri_device::set(state_command);
	logger::log("[" + name() + "] set brightness: " + std::to_string(m_brightness) + " -> " + std::to_string(brightness));
	m_brightness = brightness;
}

void tradfri_device_with_brightness::toggle() {
	set(brightness() == 0 ? max_brightness_value : zero_brightness_value);
}

void tradfri_device_with_brightness::increase() {
	if(brightness() < max_brightness_value) {
		set(static_cast<std::uint8_t>(m_brightness + 1));
	}
}

void tradfri_device_with_brightness::decrease() {
	if(brightness() > 0) {
		set(static_cast<std::uint8_t>(m_brightness - 1));
	}
}

void tradfri_device_with_brightness::update_brightness(json_value const& status) {
	static const auto enabled_key = std::string("5850");
	static const auto brightness_key = std::string("5851");
	auto previous_brightness = m_brightness;
	auto enabled = status[enabled_key].get_int();
	if(enabled == 1) {
		auto raw_brightness = status[brightness_key].get_int();
		m_brightness = ::brightness(raw_brightness);
	}
	else {
		m_brightness = zero_brightness_value;
	}
	//	auto temperature = status["5706"].get_string();
	logger::log("[" + name() + "] update brightness: " + std::to_string(previous_brightness) + " -> " + std::to_string(m_brightness));
}

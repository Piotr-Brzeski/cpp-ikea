//
//  dirigera_bulb.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-11-02.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_bulb.h"
#include "exception.h"
#include <cpp-log/log.h>

// #include <chrono>
// #include <thread>

using namespace ikea;

namespace {

constexpr auto raw_brightness_levels = std::array<int, 8>{0, 1, 16, 33, 50, 66, 83, 100};

std::uint8_t brightness_from_raw(int raw_value) {
	if(raw_value < raw_brightness_levels[1]) return 0;
	if(raw_value < raw_brightness_levels[2]) return 1;
	if(raw_value < raw_brightness_levels[3]) return 2;
	if(raw_value < raw_brightness_levels[4]) return 3;
	if(raw_value < raw_brightness_levels[5]) return 4;
	if(raw_value < raw_brightness_levels[6]) return 5;
	if(raw_value < raw_brightness_levels[7]) return 6;
	return 7;
}

std::string const& command(bool enabled) {
	static auto on  = std::string("[{\"attributes\": {\"isOn\": true}}]");
	static auto off = std::string("[{\"attributes\": {\"isOn\": false}}]");
	if(enabled) {
		return on;
	}
	return off;
}

std::string const& command(std::uint8_t brightness) {
	static auto commands = std::vector<std::string>(8);
	if(brightness >= commands.size()) {
		throw exception("Invalid brightness value");
	}
	auto& command = commands[brightness];
	if(command.empty()) {
		command = "[{\"attributes\": {\"lightLevel\": " + std::to_string(raw_brightness_levels[brightness]) + "}}]";
	}
	return command;
}

}

std::string const dirigera_bulb::device_type = "light";

dirigera_bulb::dirigera_bulb(std::string const& devices_uri, http_get& get_connection, json_value const& json)
	: device(get_name(json))
	, dirigera_device(devices_uri, get_connection, json)
{
	update_state(json);
}

dirigera_bulb::dirigera_bulb(dirigera_bulb&& bulb)
	: device(std::move(bulb))
	, dirigera_device(std::move(bulb))
	, device_with_brightness(std::move(bulb))
	, m_raw_brighness(bulb.m_raw_brighness.load())
{
}

void dirigera_bulb::update_state(json_value const& json) {
	static const auto attributes_key = std::string("attributes");
	static const auto ison_key = std::string("isOn");
	static const auto level_key = std::string("lightLevel");
	auto attributes = json.get(attributes_key);
	if(attributes) {
		auto raw_brighness_value = attributes->get(level_key);
		if(raw_brighness_value) {
			m_raw_brighness = raw_brighness_value->get_int();
		}
		auto enabled_value = attributes->get(ison_key);
		auto enabled = enabled_value ? enabled_value->get_bool() : true;
		if(enabled) {
			auto brightness = brightness_from_raw(m_raw_brighness);
			internal_update(brightness);
		}
		else {
			internal_update(device_with_brightness::zero_brightness);
		}
	}
}

void dirigera_bulb::get_state() {
	auto response = m_get_connection.get(m_uri);
	auto state_json = json(std::move(response));
	update_state(state_json.get());
}

void dirigera_bulb::send_state() {
	std::vector<std::string> commands;
	if(m_brightness_to_send > device_with_brightness::zero_brightness) {
		if(!enabled()) {
			commands.push_back(command(true));
		}
		commands.push_back(command(m_brightness_to_send));
	}
	else {
		if(brightness() > min_brightness) {
			commands.push_back(command(min_brightness));
		}
		commands.push_back(command(false));
	}
	m_patch_connection.send(commands);
//		static auto const temperature = std::string("[{\"attributes\": {\"colorTemperature\": 4000}}]");
//		m_connection.patch(temperature);
}

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

constexpr auto raw_brightness_levels = std::array<int, 8>{0, 2, 44, 87, 130, 173, 216, 254};

std::uint8_t brightness(int raw_value) {
	if(raw_value < raw_brightness_levels[1]) return 0;
	if(raw_value < raw_brightness_levels[2]) return 1;
	if(raw_value < raw_brightness_levels[3]) return 2;
	if(raw_value < raw_brightness_levels[4]) return 3;
	if(raw_value < raw_brightness_levels[5]) return 4;
	if(raw_value < raw_brightness_levels[6]) return 5;
	if(raw_value < raw_brightness_levels[7]) return 6;
	return 7;
}

}

tradfri_device_with_brightness::tradfri_device_with_brightness(std::string&& uri, coap_connection& coap, json const& json)
	: tradfri_device(std::move(uri), coap, json)
{
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
				"{\"5850\":1,\"5851\":" +                           // on
				std::to_string(raw_brightness_levels[brightness]) + // brightness
				",\"5706\":\"f5faf6\"}";                            // white lights
		}
	}
	return command;
}

void tradfri_device_with_brightness::update_brightness(json_value const& status) {
	static const auto enabled_key = std::string("5850");
	static const auto brightness_key = std::string("5851");
	auto enabled = status[enabled_key].get_int();
	if(enabled == 1) {
		auto raw_brightness = status[brightness_key].get_int();
		internal_update(::brightness(raw_brightness));
	}
	else {
		internal_update(device_with_brightness::zero_brightness);
	}
	//	auto temperature = status["5706"].get_string();
}

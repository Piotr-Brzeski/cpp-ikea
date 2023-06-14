//
//  plug.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "plug.h"
#include "device_with_brightness.h"
#include "exception.h"
#include "json.h"
#include "logger.h"

using namespace tradfri;

namespace {

std::string const& command(bool enabled) {
	static auto on  = std::string("{\"3311\":[{\"5850\":1}]}");
	static auto off = std::string("{\"3311\":[{\"5850\":0}]}");
	if(enabled) {
		return on;
	}
	return off;
}

std::string const& state_name(bool state) {
	static std::string const on = "on";
	static std::string const off = "off";
	return state ? on : off;
}

}

plug::plug(std::string const& id, coap_connection& coap, json const& json)
	: device(device::uri_prefix + id, coap, json)
{
}

plug plug::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_plug = plug(id, coap, json);
	new_plug.update(json);
	return new_plug;
}

bool plug::enabled() {
	if(needs_update()) {
		update();
	}
	return m_enabled;
}

std::uint8_t plug::brightness() {
	return enabled() ? device_with_brightness::max_brightness() : 0;
}

void plug::set(bool enabled) {
	auto& state_command = command(enabled);
	device::set(state_command);
	log("[" + name() + "] set state: " + state_name(m_enabled) + " -> " + state_name(enabled) + " : " + state_command);
	m_enabled = enabled;
}

void plug::toggle() {
	set(!enabled());
}

void plug::increase() {
	if(!enabled()) {
		set(true);
	}
}

void plug::decrease() {
	if(enabled()) {
		set(false);
	}
}

void plug::update(json const& json) {
	static const auto status_key = std::string("3312");
	static const auto enabled_key = std::string("5850");
	auto enabled = json[status_key][0][enabled_key].get_int();
	m_enabled = enabled == 1;
}

void plug::update() {
	update(json(device::load()));
}

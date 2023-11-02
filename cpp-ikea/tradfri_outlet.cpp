//
//  tradfri_outlet.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_outlet.h"
#include "tradfri_device_with_brightness.h"
#include "exception.h"
#include "json.h"
#include <cpp-log/log.h>

using namespace ikea;

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

tradfri_outlet::tradfri_outlet(std::string const& id, coap_connection& coap, json const& json)
	: tradfri_device(tradfri_device::uri_prefix + id, coap, json)
{
}

tradfri_outlet tradfri_outlet::load(std::string const& id, coap_connection& coap, json const& json) {
	auto new_plug = tradfri_outlet(id, coap, json);
	new_plug.update(json);
	return new_plug;
}

bool tradfri_outlet::enabled() {
	if(needs_update()) {
		update_state();
	}
	return m_enabled;
}

std::uint8_t tradfri_outlet::brightness() {
	return enabled() ? tradfri_device_with_brightness::max_brightness() : 0;
}

void tradfri_outlet::set(bool enabled) {
	auto& state_command = command(enabled);
	tradfri_device::set(state_command);
	logger::log("[" + name() + "] set plug state: " + state_name(m_enabled) + " -> " + state_name(enabled));
	m_enabled = enabled;
}

void tradfri_outlet::toggle() {
	set(!enabled());
}

void tradfri_outlet::increase() {
	if(!enabled()) {
		set(true);
	}
}

void tradfri_outlet::decrease() {
	if(enabled()) {
		set(false);
	}
}

void tradfri_outlet::update(json const& json) {
	static const auto status_key = std::string("3312");
	static const auto enabled_key = std::string("5850");
	auto enabled = json[status_key][0][enabled_key].get_int();
	logger::log("[" + name() + "] update plug state: " + std::to_string(m_enabled) + " -> " + std::to_string(enabled));
	m_enabled = enabled == 1;
}

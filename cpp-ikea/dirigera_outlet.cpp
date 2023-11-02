//
//  dirigera_outlet.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_outlet.h"
//#include "device_with_brightness.h"
#include "exception.h"
#include "json.h"
#include <cpp-log/log.h>

using namespace ikea;

namespace {

std::string const& command(bool enabled) {
	static auto on  = std::string("[{\"attributes\": {\"isOn\": true}}]");
	static auto off = std::string("[{\"attributes\": {\"isOn\": false}}]");
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

std::string const dirigera_outlet::device_type = "outlet";

dirigera_outlet::dirigera_outlet(std::string const& devices_uri, http_connection& connection, json_value const& json)
	: dirigera_device(devices_uri, connection, json)
{
}

//plug plug::load(std::string const& id, coap_connection& coap, json const& json) {
//	auto new_plug = plug(id, coap, json);
//	new_plug.update(json);
//	return new_plug;
//}

bool dirigera_outlet::enabled() {
	if(needs_update()) {
		update_state();
	}
	return m_enabled;
}

std::uint8_t dirigera_outlet::brightness() {
	return enabled() ? /*device_with_brightness::max_brightness()*/1 : 0;
}

void dirigera_outlet::set(bool enabled) {
	auto& state_command = command(enabled);
	dirigera_device::set(state_command);
	logger::log("[" + name() + "] set dirigera outlet state: " + state_name(m_enabled) + " -> " + state_name(enabled));
	m_enabled = enabled;
}

void dirigera_outlet::toggle() {
	set(!enabled());
}

void dirigera_outlet::increase() {
	if(!enabled()) {
		set(true);
	}
}

void dirigera_outlet::decrease() {
	if(enabled()) {
		set(false);
	}
}

void dirigera_outlet::update(json_value const& json) {
	auto enabled = json["attributes"]["isOn"].get_bool();
	logger::log("[" + name() + "] update plug state: " + std::to_string(m_enabled) + " -> " + std::to_string(enabled));
	m_enabled = enabled;
}

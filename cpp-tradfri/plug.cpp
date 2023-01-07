//
//  plug.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-07.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "plug.h"
#include "exception.h"
#include "json.h"

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

}

plug::plug(std::string&& id, coap_connection& coap, json const& json)
	: device(std::move(id), coap, json)
{
}

plug plug::load(std::string&& id, coap_connection& coap, json const& json) {
	auto new_plug = plug(std::move(id), coap, json);
	new_plug.update(json);
	return new_plug;
}

void plug::update() {
	update(json(device::load()));
}

void plug::set(bool enabled) {
	auto& data = command(enabled);
	m_coap.put(m_uri, data);
	m_enabled = enabled;
}

void plug::toggle() {
	set(!m_enabled);
}

void plug::update(json const& json) {
	static const auto status_key = std::string("3312");
	static const auto enabled_key = std::string("5850");
	auto enabled = json[status_key][0][enabled_key].get_int();
	m_enabled = enabled == 1;
}

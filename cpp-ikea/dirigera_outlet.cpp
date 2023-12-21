//
//  dirigera_outlet.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_outlet.h"
#include "exception.h"
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

}

std::string const dirigera_outlet::device_type = "outlet";

dirigera_outlet::dirigera_outlet(std::string const& devices_uri, http_get& get_connection, json_value const& json)
	: device(get_name(json))
	, dirigera_device(devices_uri, get_connection, json)
{
	update_state(json);
}

void dirigera_outlet::update_state(json_value const& json) {
	static const auto attributes_key = std::string("attributes");
	static const auto ison_key = std::string("isOn");
	auto attributes = json.get(attributes_key);
	if(attributes) {
		auto status = attributes->get(ison_key);
		if(status) {
			auto enabled = status->get_bool();
			internal_update(enabled);
		}
	}
}

void dirigera_outlet::get_state() {
	auto response = m_get_connection.get(m_uri);
	auto state_json = json(std::move(response));
	update_state(state_json.get());
}

void dirigera_outlet::send_state() {
	auto& state = command(m_enabled_to_send);
	m_patch_connection.send({state});
}

//
//  device.cpp
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "device.h"
#include "json.h"

using namespace tradfri;

namespace {

const auto uri_prefix = std::string("15001/");

std::string load_name(json const& json) {
	static const auto name_key = std::string("9001");
	return json[name_key].get_string();
}

}

std::string device::load(coap_connection& coap, std::string const& id) {
	return coap.get(uri_prefix + id);
}

std::string device::load() {
	return m_coap.get(m_uri);
}

device::device(std::string&& id, coap_connection& coap, json const& json)
	: m_id(std::move(id))
	, m_name(load_name(json))
	, m_coap(coap)
	, m_uri(uri_prefix + m_id)
{
}


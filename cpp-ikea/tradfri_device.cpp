//
//  tradfri_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "tradfri_device.h"
#include <cpp-log/log.h>

using namespace ikea;

std::string const tradfri_device::uri_prefix = "15001/";

std::string tradfri_device::load_name(json const& json) {
	static const auto name_key = "9001";
	return json[name_key].get_string();
}

std::string tradfri_device::load(coap_connection& coap, std::string const& id) {
	return coap.get(uri_prefix + id);
}

tradfri_device::tradfri_device(std::string&& uri, coap_connection& coap, json const& json)
	: m_coap(coap)
	, m_uri(std::move(uri))
{
}

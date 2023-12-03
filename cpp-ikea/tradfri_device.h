//
//  tradfri_device.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"
#include "coap_connection.h"
#include "json.h"
#include <chrono>

namespace ikea {

class tradfri_device: public virtual device {
public:
	tradfri_device(tradfri_device const&) = delete;
	tradfri_device(tradfri_device&&) = default;
	
	static std::string load(coap_connection& coap, std::string const& id);
	
protected:
	static std::string const uri_prefix;
	tradfri_device(std::string&& uri, coap_connection& coap, json const& json);
	
	static std::string load_name(json const& json);
	
	coap_connection& m_coap;
	std::string      m_uri;
};

}

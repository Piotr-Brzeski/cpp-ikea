//
//  device.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "coap_connection.h"

namespace tradfri {

class json;

class device {
public:
	static std::string load(coap_connection& coap, std::string const& id);
	std::string load();
	
	std::string const& name() const {
		return m_name;
	}
	
protected:
	device(std::string&& id, coap_connection& coap, json const& json);
	
	std::string      m_id;
	std::string      m_name;
	coap_connection& m_coap;
	std::string      m_uri;
};

}

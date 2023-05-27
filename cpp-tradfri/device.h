//
//  device.h
//  cpp-tradfri
//
//  Created by Piotr Brzeski on 2023-01-06.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "coap_connection.h"
#include "json.h"
#include <chrono>

namespace tradfri {

class device {
public:
	device(device const&) = delete;
	device(device&&) = default;
	
	static std::string load(coap_connection& coap, std::string const& id);
	
	std::string const& name() const {
		return m_name;
	}
	
protected:
	device(std::string&& id, coap_connection& coap, json const& json);
	bool needs_update() const;
	std::string load();
	void set(std::string const& state);
	
private:
	std::string                           m_id;
	std::string                           m_name;
	std::string                           m_uri;
	coap_connection&                      m_coap;
	std::chrono::steady_clock::time_point m_last_update_time;
};

}

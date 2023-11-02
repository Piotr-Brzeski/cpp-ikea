//
//  dirigera_device.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "http_connection.h"
#include "json.h"
#include <chrono>

namespace ikea {

class dirigera_device {
public:
	dirigera_device(dirigera_device const&) = delete;
	dirigera_device(dirigera_device&&) = default;
	
//	static std::string load(http_connection& connection, std::string const& id);
	
	std::string const& name() const {
		return m_name;
	}
	
protected:
	dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json);
	bool needs_update() const;
	void update_state();
	virtual void update(json_value const& json) = 0;
	std::string load();
	void set(std::string const& state);
	
private:
	std::string                           m_uri;
	std::string                           m_name;
	http_connection&                      m_connection;
	std::chrono::steady_clock::time_point m_last_update_time;
};

}

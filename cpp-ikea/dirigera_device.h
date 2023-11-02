//
//  dirigera_device.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "device.h"
#include "http_connection.h"
#include "json.h"
#include <chrono>

namespace ikea {

class dirigera_device: public device {
public:
	dirigera_device(dirigera_device const&) = delete;
	dirigera_device(dirigera_device&&) = default;
	
protected:
	dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json);
	void update_state();
	virtual void update(json_value const& json) = 0;
	std::string load();
	void set(std::string const& state);
	
private:
	http_connection&                      m_connection;
};

}

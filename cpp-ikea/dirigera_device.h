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

namespace ikea {

class dirigera_device: public virtual device {
public:
	dirigera_device(dirigera_device const&) = delete;
	dirigera_device(dirigera_device&&) = default;
	
protected:
	dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json);
	
	http_connection& m_connection;
	std::string      m_uri;
};

}

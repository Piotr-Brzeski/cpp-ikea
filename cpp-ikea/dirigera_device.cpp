//
//  dirigera_device.cpp
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#include "dirigera_device.h"
#include <cpp-log/log.h>

using namespace ikea;

dirigera_device::dirigera_device(std::string const& devices_uri, http_connection& connection, json_value const& json)
	: m_connection(connection)
	, m_uri(devices_uri + json["id"].get_string())
{
	m_name = json["attributes"]["customName"].get_string();
}

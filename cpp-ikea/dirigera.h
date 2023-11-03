//
//  dirigera.h
//  cpp-ikea
//
//  Created by Piotr Brzeski on 2023-10-31.
//  Copyright © 2023 Brzeski.net. All rights reserved.
//

#pragma once

#include "http_connection.h"
#include "dirigera_outlet.h"
#include "dirigera_bulb.h"
#include <vector>

namespace ikea {

class dirigera {
public:
	struct configuration {
		std::string ip;
		std::string access_token;
	};
	
	dirigera(configuration const& configuration);
	dirigera(std::string const& ip, std::string const& access_token);
	
	void enumerate_devices();
	
	std::vector<dirigera_bulb>& bulbs() {
		return m_bulbs;
	}
	std::vector<dirigera_outlet>& outlets() {
		return m_outlets;
	}
	
private:
	std::string const            m_uri;
	http_connection              m_connection;
	std::vector<dirigera_bulb>   m_bulbs;
	std::vector<dirigera_outlet> m_outlets;
};

}
